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

// alpha is stored per triangle as a 4-bit level so transitions survive across frames;
// ring targets are levels 0/5/10 (blocker/ring1/ring2) and opaque is 15, i.e.
// q_target = ring * 5. TRANS_STEP levels per frame -> a full opaque<->blocker swing
// takes ~8 frames, ring-to-ring transitions less
const u8 QALPHA[16]  = { 96, 107, 117, 128, 138, 149, 159, 170, 181, 191, 202, 212, 223, 233, 244, 255 };
const int TRANS_STEP = 2;

// Working buffers, lazily heap-allocated while the viewer is enabled and freed when it
// is off (so a feature that is usually off costs no memory). The per-index arrays are
// sized to the loaded map's collision tables; they're tiny next to the tables
// themselves (under 1 byte per triangle), so if the map fits, they fit.
//   sTriVisited - one bit per triangle index: findTriLists returns the same triangle
//     from every grid cell it touches (and from both captains' spheres), and drawing a
//     translucent triangle twice doubles its opacity.
//   sTriRing/sVertRing - 2-bit ring marks (0-2 = ring, 3 = unmarked). sVertRing holds
//     the lowest ring among triangles using that vertex, which is how rings propagate
//     to vertex-sharing neighbors with no adjacency data.
//   sTriAlphaQ - persistent 4-bit alpha level per triangle (survives across frames for
//     the fade smoothing).
//   sEdgeSet - fixed-size open-addressed set of vertex-index pairs packed into u32
//     keys, holding only the *drawn* edges, so an edge shared by two triangles is
//     recorded once; the line pass draws straight out of it. Fixed size because it only
//     ever holds on-screen edges (performance bounds the drawn count anyway).
static u8* sTriVisited;
static u8* sTriRing;
static u8* sVertRing;
static u8* sTriAlphaQ;
static int sTriCount;  // == triangle table mLimit (valid index bound)
static int sVertCount; // == vertex table mLimit

const u32 EDGE_SET_SIZE = 4096; // 2^12
static u32* sEdgeSet;
static int sEdgeSetLoad;

// the unique in-range triangles, gathered ONCE per frame from the spatial grid. all
// the per-frame passes (blocker/ring marking, alpha easing, fills) then iterate this
// flat list instead of re-querying findTriLists per pass - the grid query is the
// expensive part and was previously run once per pass
static u16* sGatheredTris;
static int sGatheredCount;

static void free_buffers()
{
	delete[] sTriVisited;
	delete[] sTriRing;
	delete[] sVertRing;
	delete[] sTriAlphaQ;
	delete[] sEdgeSet;
	delete[] sGatheredTris;
	sTriVisited   = nullptr;
	sTriRing      = nullptr;
	sVertRing     = nullptr;
	sTriAlphaQ    = nullptr;
	sEdgeSet      = nullptr;
	sGatheredTris = nullptr;
	sTriCount     = 0;
	sVertCount    = 0;
}

static void alloc_buffers(int tri_count, int vert_count)
{
	free_buffers();
	sTriCount  = tri_count;
	sVertCount = vert_count;

	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	sTriVisited        = new u8[(tri_count + 7) / 8];
	sTriRing           = new u8[(tri_count + 3) / 4];
	sTriAlphaQ         = new u8[(tri_count + 1) / 2];
	sVertRing          = new u8[(vert_count + 3) / 4];
	sEdgeSet           = new u32[EDGE_SET_SIZE];
	sGatheredTris      = new u16[tri_count];
	prev_heap->becomeCurrentHeap();

	// start every triangle fully opaque rather than mid-fade
	memset(sTriAlphaQ, 0xFF, (tri_count + 1) / 2);
}

// lowest ring among the triangle's three vertices (3 = none marked)
static u32 min_vert_ring(Sys::Triangle* tri)
{
	u32 ring = 3;
	for (int i = 0; i < 3; i++) {
		int v = tri->mVertices[i];
		if (v >= 0 && v < sVertCount && get2(sVertRing, v) < ring) {
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
		if (v >= 0 && v < sVertCount) {
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
	if (a < 0 || a >= 0xFFFF || b > 0xFFFF) {
		return;
	}
	// if the table is nearly full just drop lines rather than probe forever
	if (sEdgeSetLoad >= (int)(EDGE_SET_SIZE - EDGE_SET_SIZE / 4)) {
		return;
	}

	u32 key = ((u32)(a + 1) << 16) | (u32)b;
	u32 idx = (key * 0x9E3779B9u) >> 20; // top 12 bits, EDGE_SET_SIZE = 2^12
	while (sEdgeSet[idx]) {
		if (sEdgeSet[idx] == key) {
			return;
		}
		idx = (idx + 1) & (EDGE_SET_SIZE - 1);
	}
	sEdgeSet[idx] = key;
	sEdgeSetLoad++;
}

// resolve the loaded map's collision tables (caves and above-ground store the divider
// differently). returns false if no map / collision is loaded yet
static bool get_collision_tables(Sys::TriangleTable** tri, Sys::VertexTable** vert)
{
	if (!Game::mapMgr || !Game::gameSystem) {
		return false;
	}
	if (Game::gameSystem->mIsInCave) {
		Game::RoomMapMgr* m = static_cast<Game::RoomMapMgr*>(Game::mapMgr);
		if (!m->mMapCollision || !m->mMapCollision->mDivider) {
			return false;
		}
		*tri  = m->mMapCollision->mDivider->mTriangleTable;
		*vert = m->mMapCollision->mDivider->mVertexTable;
	} else {
		Game::ShapeMapMgr* m = static_cast<Game::ShapeMapMgr*>(Game::mapMgr);
		if (!m->mMapCollision.mDivider) {
			return false;
		}
		*tri  = m->mMapCollision.mDivider->mTriangleTable;
		*vert = m->mMapCollision.mDivider->mVertexTable;
	}
	return true;
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

// query the spatial grid ONCE and append the unique in-range triangle indices to the
// flat gathered list. this is the expensive call (grid traversal) - doing it once and
// iterating the flat list for every subsequent pass is the whole point of the rewrite
static void gather_triangles(Sys::Sphere& sphere)
{
	Sys::TriIndexList* triLists;
	if (Game::gameSystem->mIsInCave) {
		triLists = static_cast<Game::RoomMapMgr*>(Game::mapMgr)->mMapCollision->mDivider->findTriLists(sphere);
	} else {
		triLists = static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapCollision.mDivider->findTriLists(sphere);
	}

	for (; triLists; triLists = static_cast<Sys::TriIndexList*>(triLists->mNext)) {
		for (int i = 0; i < triLists->getNum(); i++) {
			int triIdx = triLists->mObjects[i];
			if (triIdx < 0 || triIdx >= sTriCount) {
				continue;
			}
			// dedup: the same triangle is returned from every grid cell it touches and
			// from both captains' spheres
			if (sTriVisited[triIdx >> 3] & (1 << (triIdx & 7))) {
				continue;
			}
			sTriVisited[triIdx >> 3] |= (u8)(1 << (triIdx & 7));
			sGatheredTris[sGatheredCount++] = (u16)triIdx;
		}
	}
}

Color4 CollisionViewer::fill_color(Sys::Triangle* tri, Sys::VertexTable* vertTable, u32 q)
{
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
	return color;
}

// draw the gathered triangles of one alpha class (settled-opaque, or mid-fade
// translucent) in batched GXBegin calls. one GXBegin per triangle is as fatal as one
// per line; this batches them, chunked to stay under GXBegin's u16 vertex-count limit
void CollisionViewer::emit_fills(Sys::TriangleTable* triTable, Sys::VertexTable* vertTable, bool opaque)
{
	int total = 0;
	for (int g = 0; g < sGatheredCount; g++) {
		if ((get4(sTriAlphaQ, sGatheredTris[g]) == 15) == opaque) {
			total++;
		}
	}
	if (total == 0) {
		return;
	}

	const int MAX_TRIS_PER_BATCH = 21000; // 3 verts each, under GXBegin's u16 limit
	int g                        = 0;     // cursor persists across chunks
	int remaining                = total;
	while (remaining > 0) {
		int batch = (remaining < MAX_TRIS_PER_BATCH) ? remaining : MAX_TRIS_PER_BATCH;

		GXBegin(GX_TRIANGLES, GX_VTXFMT0, (u16)(batch * 3));
		int emitted = 0;
		while (emitted < batch) {
			int triIdx = sGatheredTris[g++];
			u32 q      = get4(sTriAlphaQ, triIdx);
			if ((q == 15) != opaque) {
				continue;
			}
			Sys::Triangle* tri = triTable->getTriangle(triIdx);
			Color4 color       = fill_color(tri, vertTable, q);
			for (int j = 0; j < 3; j++) {
				Vector3f* vertex = vertTable->getVertex(tri->mVertices[j]);
				GXPosition3f32(vertex->x, vertex->y, vertex->z);
				GXColor4u8(color.r, color.g, color.b, color.a);
			}
			emitted++;
		}
		GXEnd();
		remaining -= batch;
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
		// size the working buffers to this map's collision tables
		Sys::TriangleTable* triTable;
		Sys::VertexTable* vertTable;
		if (get_collision_tables(&triTable, &vertTable)) {
			alloc_buffers(triTable->mLimit, vertTable->mLimit);
		}
	} else if (!enabled_) {
		free_buffers();
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

	// buffers may not have been allocated yet if the viewer was toggled on before the
	// map finished loading; allocate now that we're drawing
	if (!sTriVisited) {
		Sys::TriangleTable* triTable;
		Sys::VertexTable* vertTable;
		if (!get_collision_tables(&triTable, &vertTable)) {
			return;
		}
		alloc_buffers(triTable->mLimit, vertTable->mLimit);
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

	memset(sTriVisited, 0, (sTriCount + 7) / 8);
	memset(sEdgeSet, 0, EDGE_SET_SIZE * sizeof(u32));
	memset(sTriRing, 0xFF, (sTriCount + 3) / 4);
	memset(sVertRing, 0xFF, (sVertCount + 3) / 4);
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

	Sys::TriangleTable* triTable;
	Sys::VertexTable* vertTable;
	if (!get_collision_tables(&triTable, &vertTable)) {
		return;
	}

	// Gather the in-range triangles ONCE (the grid query is the expensive part). Every
	// pass below iterates this flat list instead of re-querying per pass.
	sGatheredCount = 0;
	if (navi) {
		gather_triangles(navi->mNaviIndex == NAVIID_Olimar ? olimarSphere : louieSphere);
	} else {
		// while switching captains, gather both only if they're far apart
		gather_triangles(olimarSphere);
		if (sqrDistanceXZ(olimarSphere.mPosition, louieSphere.mPosition) > RENDER_RADIUS / 3) {
			gather_triangles(louieSphere);
		}
	}

	// mark the sight-blockers (ring 0), then propagate two rings of adjacency outward
	if (los_valid) {
		for (int g = 0; g < sGatheredCount; g++) {
			int triIdx         = sGatheredTris[g];
			Sys::Triangle* tri = triTable->getTriangle(triIdx);
			Vector3f hit;
			if (get2(sTriRing, triIdx) == 3 && tri->intersect(losEdge, LOS_RADIUS, hit)) {
				mark_tri_ring(tri, triIdx, 0);
			}
		}
		for (u32 ring = 1; ring <= 2; ring++) {
			for (int g = 0; g < sGatheredCount; g++) {
				int triIdx         = sGatheredTris[g];
				Sys::Triangle* tri = triTable->getTriangle(triIdx);
				if (get2(sTriRing, triIdx) == 3 && min_vert_ring(tri) < ring) {
					mark_tri_ring(tri, triIdx, ring);
				}
			}
		}
	}

	// ease every gathered triangle's alpha one step toward its ring's target
	for (int g = 0; g < sGatheredCount; g++) {
		int triIdx = sGatheredTris[g];
		u32 target = get2(sTriRing, triIdx) * 5; // rings 0/1/2 -> q 0/5/10, opaque -> 15
		int q      = (int)get4(sTriAlphaQ, triIdx);
		if (q < (int)target) {
			q = (q + TRANS_STEP < (int)target) ? q + TRANS_STEP : (int)target;
		} else if (q > (int)target) {
			q = (q - TRANS_STEP > (int)target) ? q - TRANS_STEP : (int)target;
		}
		set4(sTriAlphaQ, triIdx, (u32)q);
	}

	// settled-opaque fills first (blend still off from initPrimDraw), then the
	// translucent set on top. vertex alpha only matters once blending is switched on
	emit_fills(triTable, vertTable, true);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE); // translucent fills shouldn't occlude anything
	emit_fills(triTable, vertTable, false);

	// collect outline edges for every gathered triangle (same radius as the fills); the
	// set dedups edges shared between triangles
	for (int g = 0; g < sGatheredCount; g++) {
		Sys::Triangle* tri = triTable->getTriangle(sGatheredTris[g]);
		edge_set_insert(tri->mVertices[0], tri->mVertices[1]);
		edge_set_insert(tri->mVertices[1], tri->mVertices[2]);
		edge_set_insert(tri->mVertices[2], tri->mVertices[0]);
	}

	// thin outlines along the (deduplicated) triangle edges
	GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE); // LEQUAL so lines coplanar with fills pass the z test
	GXSetLineWidth(6, GX_TO_ZERO);

	if (sEdgeSetLoad == 0) {
		return;
	}

	// Emit ALL line segments in a single GXBegin batch. A per-edge GXBegin/GXEnd makes
	// each 2-vertex line its own GP primitive setup - thousands of those tank the frame
	// rate (worst-case setup-to-pixel ratio). One batch is one setup for the whole set.
	GXBegin(GX_LINES, GX_VTXFMT0, (u16)(sEdgeSetLoad * 2));
	for (u32 i = 0; i < EDGE_SET_SIZE; i++) {
		u32 key = sEdgeSet[i];
		if (!key) {
			continue;
		}
		Vector3f* va = vertTable->getVertex((int)(key >> 16) - 1);
		Vector3f* vb = vertTable->getVertex((int)(key & 0xFFFF));
		GXPosition3f32(va->x, va->y, va->z);
		GXColor4u8(20, 20, 20, 255);
		GXPosition3f32(vb->x, vb->y, vb->z);
		GXColor4u8(20, 20, 20, 255);
	}
	GXEnd();
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
