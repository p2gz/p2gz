#include <p2gz/CollisionViewer.h>
#include <Game/Navi.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <Graphics.h>
#include <Viewport.h>
#include <Camera.h>
#include <stl/math.h>
#include <string.h>
#include <Game/MapMgr.h>
#include <Game/mapParts.h>
#include <p2gz/p2gz.h>

using namespace gz;

const f32 RENDER_RADIUS = 1024.0f;
const f32 LOS_RADIUS    = 1.0f; // thickness of the camera->captain sight ray

// the passes draw_triangles runs in, in order: sight-blockers and the two rings of
// adjacent triangles are marked first, every triangle's alpha then eases one step
// toward its ring's target, and finally the settled-opaque and translucent sets draw
enum {
	MODE_MARK_BLOCKERS    = 0,
	MODE_MARK_RING1       = 1,
	MODE_MARK_RING2       = 2,
	MODE_UPDATE_ALPHA     = 3,
	MODE_DRAW_OPAQUE      = 4,
	MODE_DRAW_TRANSLUCENT = 5,
	MODE_COUNT            = 6,
};

// alpha is stored per triangle as a 4-bit level so transitions survive across frames;
// ring targets are levels 0/5/10 (blocker/ring1/ring2) and opaque is 15, i.e.
// q_target = ring * 5. TRANS_STEP levels per frame -> a full opaque<->blocker swing
// takes ~8 frames, ring-to-ring transitions less
const u8 QALPHA[16]  = { 96, 107, 117, 128, 138, 149, 159, 170, 181, 191, 202, 212, 223, 233, 244, 255 };
const int TRANS_STEP = 2;

// per-frame dedup state, static so it costs DOL bss instead of the tight sys heap.
// sTriVisited has one bit per triangle index: findTriLists returns the same triangle
// from every grid cell it touches (and from both captains' spheres), and drawing a
// translucent triangle twice doubles its opacity.
// sEdgeSet is an open-addressed set of vertex-index pairs packed into u32 keys, so an
// edge shared by two triangles is recorded once; the line pass then draws straight out
// of the table.
static u8 sTriVisited[65536 / 8];
const u32 EDGE_SET_SIZE = 8192;
static u32 sEdgeSet[EDGE_SET_SIZE];
static int sEdgeSetLoad;

// per-frame ring marks, 2 bits each: 0-2 = ring, 3 = unmarked/opaque. sVertRing holds
// the lowest ring among the triangles using that vertex, which is how rings propagate
// to vertex-sharing neighbors without any adjacency data
static u8 sTriRing[65536 / 4];
static u8 sVertRing[65536 / 4];

// persistent 4-bit alpha level per triangle (survives across frames for smoothing)
static u8 sTriAlphaQ[65536 / 2];

// lowest ring among the triangle's three vertices (3 = none marked)
static u32 min_vert_ring(Sys::Triangle* tri)
{
	u32 ring = 3;
	for (int i = 0; i < 3; i++) {
		int v = tri->mVertices[i];
		if ((u32)v < sizeof(sVertRing) * 4 && get2(sVertRing, v) < ring) {
			ring = get2(sVertRing, v);
		}
	}
	return ring;
}

static void mark_tri_ring(Sys::Triangle* tri, int triIdx, u32 ring)
{
	min2(sTriRing, triIdx, ring);
	for (int i = 0; i < 3; i++) {
		int v = tri->mVertices[i];
		if ((u32)v < sizeof(sVertRing) * 4) {
			min2(sVertRing, v, ring);
		}
	}
}

static void edge_set_insert(int a, int b)
{
	if (a > b) {
		int tmp = a;
		a       = b;
		b       = tmp;
	}
	// pack as ((min+1) << 16) | max so a valid key is never 0 (= empty slot)
	if (a >= 0xFFFF || b > 0xFFFF) {
		return;
	}
	// if the table is nearly full just drop lines rather than probe forever
	if (sEdgeSetLoad >= (int)(EDGE_SET_SIZE - EDGE_SET_SIZE / 4)) {
		return;
	}

	u32 key = ((u32)(a + 1) << 16) | (u32)b;
	u32 idx = (key * 0x9E3779B9u) >> 19; // top 13 bits, EDGE_SET_SIZE = 2^13
	while (sEdgeSet[idx]) {
		if (sEdgeSet[idx] == key) {
			return;
		}
		idx = (idx + 1) & (EDGE_SET_SIZE - 1);
	}
	sEdgeSet[idx] = key;
	sEdgeSetLoad++;
}

namespace gz {
bool CollisionViewer::is_navi_on_triangle(Sys::Triangle* tri, Sys::Triangle* naviTriangle, Sys::VertexTable* vertTable)
{
	if (!tri || !naviTriangle) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		Vector3f* naviVertex = vertTable->getVertex(naviTriangle->mVertices[i]);
		Vector3f* triVertex  = vertTable->getVertex(tri->mVertices[i]);
		if (naviVertex != triVertex) {
			return false;
		}
	}
	return true;
}

void CollisionViewer::draw_triangles(Sys::Sphere& sphere, int mode)
{
	Sys::TriIndexList* triLists;
	Sys::VertexTable* vertTable;
	Sys::TriangleTable* triTable;
	if (Game::gameSystem->mIsInCave) {
		Game::RoomMapMgr* roomMapMgr = static_cast<Game::RoomMapMgr*>(Game::mapMgr);
		triLists                     = roomMapMgr->mMapCollision->mDivider->findTriLists(sphere);
		vertTable                    = roomMapMgr->mMapCollision->mDivider->mVertexTable;
		triTable                     = roomMapMgr->mMapCollision->mDivider->mTriangleTable;
	} else {
		Game::ShapeMapMgr* shapeMapMgr = static_cast<Game::ShapeMapMgr*>(Game::mapMgr);
		triLists                       = shapeMapMgr->mMapCollision.mDivider->findTriLists(sphere);
		vertTable                      = shapeMapMgr->mMapCollision.mDivider->mVertexTable;
		triTable                       = shapeMapMgr->mMapCollision.mDivider->mTriangleTable;
	}

	if (!triLists) {
		return;
	}

	for (triLists; triLists; triLists = static_cast<Sys::TriIndexList*>(triLists->mNext)) {
		for (int i = 0; i < triLists->getNum(); i++) {
			int triIdx = triLists->mObjects[i];
			if ((u32)triIdx >= sizeof(sTriRing) * 4) {
				continue; // out of dedup/ring range; nothing sane to do with it
			}
			Sys::Triangle* tri = triTable->getTriangle(triIdx);

			if (mode == MODE_MARK_BLOCKERS) {
				if (get2(sTriRing, triIdx) == 3 && los_valid) {
					Vector3f hit;
					if (tri->intersect(losEdge, LOS_RADIUS, hit)) {
						mark_tri_ring(tri, triIdx, 0);
					}
				}
				continue;
			}

			if (mode == MODE_MARK_RING1 || mode == MODE_MARK_RING2) {
				// pull triangles touching a marked vertex into the next ring out
				u32 ring = (mode == MODE_MARK_RING1) ? 1 : 2;
				if (get2(sTriRing, triIdx) == 3 && min_vert_ring(tri) < ring) {
					mark_tri_ring(tri, triIdx, ring);
				}
				continue;
			}

			// remaining modes must process each triangle exactly once (it appears in
			// several grid cells and possibly both captains' spheres)
			if (sTriVisited[triIdx >> 3] & (1 << (triIdx & 7))) {
				continue;
			}

			if (mode == MODE_UPDATE_ALPHA) {
				sTriVisited[triIdx >> 3] |= (u8)(1 << (triIdx & 7));
				u32 target = get2(sTriRing, triIdx) * 5; // rings 0/1/2 -> q 0/5/10, opaque -> 15
				int q      = (int)get4(sTriAlphaQ, triIdx);
				if (q < (int)target) {
					q = (q + TRANS_STEP < (int)target) ? q + TRANS_STEP : (int)target;
				} else if (q > (int)target) {
					q = (q - TRANS_STEP > (int)target) ? q - TRANS_STEP : (int)target;
				}
				set4(sTriAlphaQ, triIdx, (u32)q);
				continue;
			}

			// draw modes: anything mid-transition still needs blending, so the split
			// is by current alpha level, not by ring
			u32 q = get4(sTriAlphaQ, triIdx);
			if ((q == 15) != (mode == MODE_DRAW_OPAQUE)) {
				continue;
			}
			sTriVisited[triIdx >> 3] |= (u8)(1 << (triIdx & 7));

			Color4 color = Color4(200, 200, 200, 128);
			if (!is_navi_on_triangle(tri, olimarTriangle, vertTable) && !is_navi_on_triangle(tri, louieTriangle, vertTable)) {
				switch (tri->mCode.getSlipCode()) {
				case MapCode::Code::SlipCode_NoSlip:
					color = Color4(0, 50 + 150 * fabs(tri->mTrianglePlane.mNormal.y), 0, 128);
					break;
				case MapCode::Code::SlipCode_Gradual:
					color = Color4(0, 0, 50 + 150 * fabs(tri->mTrianglePlane.mNormal.y), 128);
					break;
				case MapCode::Code::SlipCode_Steep:
					color = Color4(50 + 150 * fabs(tri->mTrianglePlane.mNormal.y), 0, 0, 128);
					break;
				}
			}
			color.a = QALPHA[q];

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
			for (int j = 0; j < 3; j++) {
				Vector3f* vertex = vertTable->getVertex(tri->mVertices[j]);
				GXPosition3f32(vertex->x, vertex->y, vertex->z);
				GXColor4u8(color.r, color.g, color.b, color.a);
			}
			GXEnd();

			// record the outline edges; the set collapses edges shared between triangles
			edge_set_insert(tri->mVertices[0], tri->mVertices[1]);
			edge_set_insert(tri->mVertices[1], tri->mVertices[2]);
			edge_set_insert(tri->mVertices[2], tri->mVertices[0]);
		}
	}
}

void CollisionViewer::toggle(bool enabled_)
{
	if (!Game::gameSystem->mIsInCave) {
		SysShape::Model* mapModel = static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapModel;
		if (enabled_) {
			mapModel->hide();
		} else {
			mapModel->show();
		}
	}

	if (enabled_ && !enabled) {
		// start everything fully opaque rather than mid-fade from a previous session
		memset(sTriAlphaQ, 0xFF, sizeof(sTriAlphaQ));
	}

	enabled = enabled_;
}

void CollisionViewer::draw()
{
	if (Game::naviMgr->getActiveNavi() && need_to_reenable) {
		toggle(true);
		need_to_reenable = false;
	}

	if (!enabled) {
		return;
	}

	Game::Navi* olimar = Game::naviMgr->getAt(NAVIID_Olimar);
	if (olimar) {
		Vector3f olimarPos = olimar->getPosition();
		olimarSphere       = Sys::Sphere(olimarPos, RENDER_RADIUS);
		olimarTriangle     = olimar->mFloorTriangle;
	}
	Game::Navi* louie = Game::naviMgr->getAt(NAVIID_Louie);
	if (louie) {
		Vector3f louiePos = louie->getPosition();
		louieSphere       = Sys::Sphere(louiePos, RENDER_RADIUS);
		louieTriangle     = louie->mFloorTriangle;
	}

	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);

	memset(sTriVisited, 0, sizeof(sTriVisited));
	memset(sEdgeSet, 0, sizeof(sEdgeSet));
	memset(sTriRing, 0xFF, sizeof(sTriRing));
	memset(sVertRing, 0xFF, sizeof(sVertRing));
	sEdgeSetLoad = 0;

	// sight line from the camera to the active captain; triangles crossing it (and,
	// fading back to opaque, their two rings of neighbors) are drawn translucent so
	// they don't hide him
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	Viewport* vp     = sys->mGfx->mCurrentViewport;
	los_valid        = false;
	if (navi && vp && vp->getCamera()) {
		Vector3f camPos  = vp->getCamera()->getPosition();
		Vector3f naviPos = navi->getPosition();
		naviPos.y += 15.0f; // aim at his body, not his feet
		losEdge.setStartEnd(camPos, naviPos);
		los_valid = true;
	}

	// mark blockers + adjacency rings, ease every triangle's alpha one step toward
	// its target, then draw: settled-opaque fills first, the translucent set on top.
	// vertex alpha only has an effect once blending is switched on - initPrimDraw
	// leaves GX_BM_NONE, which renders everything opaque regardless of alpha
	for (int mode = 0; mode < MODE_COUNT; mode++) {
		if (mode == MODE_DRAW_OPAQUE) {
			// the alpha-update sweep consumed the visited bits; draw passes re-dedup
			memset(sTriVisited, 0, sizeof(sTriVisited));
		}
		if (mode == MODE_DRAW_TRANSLUCENT) {
			GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
			GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE); // translucent fills shouldn't occlude anything
		}
		if (navi) {
			draw_triangles(navi->mNaviIndex == NAVIID_Olimar ? olimarSphere : louieSphere, mode);
		} else {
			// Minimize duplicate triangles while switching captains by only drawing triangles for both
			// if they are arbitrarily far apart.
			draw_triangles(olimarSphere, mode);
			if (sqrDistanceXZ(olimarSphere.mPosition, louieSphere.mPosition) > RENDER_RADIUS / 3) {
				draw_triangles(louieSphere, mode);
			}
		}
	}

	// thin outlines along the (deduplicated) triangle edges
	GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE); // LEQUAL so lines coplanar with fills pass the z test
	GXSetLineWidth(6, GX_TO_ZERO);

	Sys::VertexTable* vertTable = nullptr;
	if (Game::gameSystem->mIsInCave) {
		vertTable = static_cast<Game::RoomMapMgr*>(Game::mapMgr)->mMapCollision->mDivider->mVertexTable;
	}
	vertTable = static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapCollision.mDivider->mVertexTable;

	for (u32 i = 0; i < EDGE_SET_SIZE; i++) {
		u32 key = sEdgeSet[i];
		if (!key) {
			continue;
		}

		Vector3f* va = vertTable->getVertex((int)(key >> 16) - 1);
		Vector3f* vb = vertTable->getVertex((int)(key & 0xFFFF));

		GXBegin(GX_LINES, GX_VTXFMT0, 2);
		GXPosition3f32(va->x, va->y, va->z);
		GXColor4u8(20, 20, 20, 255);
		GXPosition3f32(vb->x, vb->y, vb->z);
		GXColor4u8(20, 20, 20, 255);
		GXEnd();
	}
}
} // namespace gz

namespace Game {
// @Extracted: gameMapParts.s doEntry__Q24Game7MapRoomFv
void MapRoom::doEntry()
{
	// @P2GZ: collision viewer
	// hide room model when enabled
	if (p2gz->collision_viewer->is_enabled()) {
		mModel->hide();
		mModel->hidePackets();
		return;
	}

	if (RoomMapMgr::mUseCylinderViewCulling) {
		Graphics* gfx  = sys->getGfx();
		bool isVisible = false;
		for (int i = 0; i < gfx->mActiveViewports; i++) {
			Viewport* vp = gfx->getViewport(i);
			if (vp->viewable() && vp->mCamera->isCylinderVisible(mRoomVisibilityCylinder)) {
				isVisible = true;
				break;
			}
		}
		if (isVisible) {
			mModel->mJ3dModel->entry();
		}
	} else {
		bool isVisible = false;
		Graphics* gfx  = sys->getGfx();

		for (int i = 0; i < gfx->mActiveViewports; i++) {
			Viewport* vp = gfx->getViewport(i);
			if (vp->viewable() && vp->mCamera->isVisible(mRoomVisibilitySphere)) {
				isVisible = true;
				break;
			}
		}

		if (isVisible) {
			if (!gameSystem->paused()) {
				for (int i = 0; i < mAnimationCount; i++) {
					mAnimators[i].animate(30.0f);
				}
			}

			mModel->show();
		} else {
			if (BaseHIOParms::sEntryOptMapRoom && !gameSystem->isMultiplayerMode()) {
				return;
			}

			mModel->hide();
		}

		mModel->mJ3dModel->entry();
	}

	mModel->mJ3dModel->calcMaterial();
	mModel->mJ3dModel->diff();
}

// @Extracted: gameMapParts.s doEntry__Q24Game10RoomMapMgrFv
void RoomMapMgr::doEntry()
{
	sys->mTimers->_start("ENT-MAP", true);

	if (gameSystem) {
		BaseGameSection* section = gameSystem->getSection();
		if (mSeaMgr) {
			mSeaMgr->doEntry();
		}

		section->setDrawBuffer(DB_MapLayer);
		mRoomMgr.doEntry();

		if (mVRBox.mModel) {
			// @P2GZ: collision viewer
			// hide VRBox model when enabled
			if (!p2gz->collision_viewer->is_enabled()) {
				section->setDrawBuffer(DB_FirstLayer);
				mVRBox.mModel->mJ3dModel->entry();
			}
		}

		section->setDrawBuffer(DB_NormalLayer);
	}

	sys->mTimers->_stop("ENT-MAP");
}
} // namespace Game
