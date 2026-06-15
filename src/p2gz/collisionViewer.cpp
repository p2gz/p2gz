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
const u32 EDGE_SET_SIZE = 4096;

const f32 FLOOR_NORMAL_Y = 0.7f;
const f32 CAPTAIN_HEIGHT = 30.0f;
const u8 OCCLUDER_ALPHA  = 32;
const int FADE_STEP      = 24;

namespace gz {

bool CollisionViewer::get_collision_tables(Sys::TriangleTable** tri, Sys::VertexTable** vert)
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

// Allocate the working buffers, sized to this map's triangle/vertex counts.
void CollisionViewer::alloc_buffers(int tris)
{
	free_buffers();
	tri_count = tris;

	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	tri_visited        = new u8[(tris + 7) / 8];
	tri_alpha          = new u8[tris];
	edge_set           = new u32[EDGE_SET_SIZE];
	gathered_tris      = new u16[tris];
	prev_heap->becomeCurrentHeap();

	// initialize every triangle to be opaque
	memset(tri_alpha, 0xFF, tris);
}

// Release the working buffers.
void CollisionViewer::free_buffers()
{
	delete[] tri_visited;
	delete[] tri_alpha;
	delete[] edge_set;
	delete[] gathered_tris;
	tri_visited   = nullptr;
	tri_alpha     = nullptr;
	edge_set      = nullptr;
	gathered_tris = nullptr;
	tri_count     = 0;
}

// Collect the triangles near the captain into `gathered_tris`, without duplicates.
void CollisionViewer::gather_triangles(Sys::Sphere& sphere)
{
	Sys::TriIndexList* lists;
	if (Game::gameSystem->mIsInCave) {
		lists = static_cast<Game::RoomMapMgr*>(Game::mapMgr)->mMapCollision->mDivider->findTriLists(sphere);
	} else {
		lists = static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapCollision.mDivider->findTriLists(sphere);
	}

	// We can't use `findTriLists` directly because it may contain duplicates. Collision is stored in a grid of cells,
	// and `findTriLists` returns the list of triangles for every cell enclosed by `sphere`. Triangles that span multiple
	// cells are included in each cell's list, so we need to deduplicate them.
	for (; lists; lists = static_cast<Sys::TriIndexList*>(lists->mNext)) {
		for (int i = 0; i < lists->getNum(); i++) {
			int idx = lists->mObjects[i];
			if (idx < 0 || idx >= tri_count) {
				continue;
			}

			// This is just a memory optimization: `tri_visited` stores each triangle as a single bit.
			// If bit (idx >> 3) in byte (idx & 7) is set, we've already visited that triangle.
			if (tri_visited[idx >> 3] & (1 << (idx & 7))) {
				continue;
			}

			tri_visited[idx >> 3] |= (u8)(1 << (idx & 7));
			gathered_tris[gathered_count++] = (u16)idx;
		}
	}
}

// Add an undirected edge to the set so an edge shared by two triangles is only stored, and therefore drawn, once.
// https://en.wikipedia.org/wiki/Hash_function#Fibonacci_hashing
void CollisionViewer::edge_set_insert(int a, int b)
{
	// order the pair so (a, b) and (b, a) are the same edge
	if (a > b) {
		int tmp = a;
		a = b;
		b = tmp;
	}

	u32 key = ((u32)(a + 1) << 16) | (u32)b;
	u32 hash = (key * 0x9E3779B9u) >> 20;
	while (edge_set[hash]) {
		if (edge_set[hash] == key) {
			return;
		}
		hash = (hash + 1) & (EDGE_SET_SIZE - 1);
	}
	edge_set[hash] = key;
	edge_count++;
}

bool CollisionViewer::is_navi_on_triangle(Sys::Triangle* tri, Sys::Triangle* naviTriangle, Sys::VertexTable* vertTable)
{
	if (!tri || !naviTriangle) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		if (vertTable->getVertex(naviTriangle->mVertices[i]) != vertTable->getVertex(tri->mVertices[i])) {
			return false;
		}
	}
	return true;
}

Color4 CollisionViewer::get_fill_color(Sys::Triangle* tri, Sys::VertexTable* vertTable, u8 alpha)
{
	Color4 color = Color4(200, 200, 200, 128);
	if (!is_navi_on_triangle(tri, olimar_triangle, vertTable) && !is_navi_on_triangle(tri, louie_triangle, vertTable)) {
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
	color.a = alpha;
	return color;
}

void CollisionViewer::draw_triangles(Sys::TriangleTable* triTable, Sys::VertexTable* vertTable, bool opaque)
{
	int total = 0;
	for (int i = 0; i < gathered_count; i++) {
		if ((tri_alpha[gathered_tris[i]] == 255) == opaque) {
			total++;
		}
	}
	if (total == 0) {
		return;
	}

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, (u16)(total * 3));
	for (int i = 0; i < gathered_count; i++) {
		int idx   = gathered_tris[i];
		u8 alpha  = tri_alpha[idx];
		if ((alpha == 255) != opaque) {
			continue;
		}
		Sys::Triangle* tri = triTable->getTriangle(idx);
		Color4 color       = get_fill_color(tri, vertTable, alpha);
		for (int j = 0; j < 3; j++) {
			Vector3f* vertex = vertTable->getVertex(tri->mVertices[j]);
			GXPosition3f32(vertex->x, vertex->y, vertex->z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
	}
	GXEnd();
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
		Sys::TriangleTable* triTable;
		Sys::VertexTable* vertTable;
		if (get_collision_tables(&triTable, &vertTable)) {
			alloc_buffers(triTable->mLimit);
		}
	} else if (!enabled_) {
		free_buffers();
	}

	enabled = enabled_;
}

void CollisionViewer::draw()
{
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (navi && need_to_reenable) {
		toggle(true);
		need_to_reenable = false;
	}

	if (!enabled) {
		return;
	}

	Sys::TriangleTable* triTable;
	Sys::VertexTable* vertTable;
	if (!get_collision_tables(&triTable, &vertTable)) {
		return;
	}

	if (!tri_visited) {
		alloc_buffers(triTable->mLimit);
	}

	Game::Navi* olimar = Game::naviMgr->getAt(NAVIID_Olimar);
	if (olimar) {
		Vector3f pos   = olimar->getPosition();
		olimar_sphere   = Sys::Sphere(pos, RENDER_RADIUS);
		olimar_triangle = olimar->mFloorTriangle;
	}
	Game::Navi* louie = Game::naviMgr->getAt(NAVIID_Louie);
	if (louie) {
		Vector3f pos  = louie->getPosition();
		louie_sphere   = Sys::Sphere(pos, RENDER_RADIUS);
		louie_triangle = louie->mFloorTriangle;
	}

	Graphics* gfx = sys->getGfx();
	gfx->initPrimDraw(nullptr);

	memset(tri_visited, 0, (tri_count + 7) / 8);
	memset(edge_set, 0, EDGE_SET_SIZE * sizeof(u32));

	gathered_count = 0;
	if (navi) {
		gather_triangles(navi->mNaviIndex == NAVIID_Olimar ? olimar_sphere : louie_sphere);
	} else {
		// while switching captains, gather both only if they're far apart
		gather_triangles(olimar_sphere);
		if (sqrDistanceXZ(olimar_sphere.mPosition, louie_sphere.mPosition) > RENDER_RADIUS / 3) {
			gather_triangles(louie_sphere);
		}
	}

	// fade triangles that occlude the captain
	Vector3f camPos   = sys->mGfx->mCurrentViewport->getCamera()->getPosition();
	f32 sqr_navi_dist = 0.0f;
	f32 navi_head_y   = 0.0f;
	if (navi) {
		Vector3f naviPos = navi->getPosition();
		sqr_navi_dist    = camPos.sqrDistance(naviPos);
		navi_head_y      = naviPos.y + CAPTAIN_HEIGHT;
	}
	for (int g = 0; g < gathered_count; g++) {
		int idx            = gathered_tris[g];
		Sys::Triangle* tri = triTable->getTriangle(idx);

		bool occluder = false;
		if (navi) {
			bool not_floor  = tri->mTrianglePlane.mNormal.y < FLOOR_NORMAL_Y;
			bool above_head = tri->mSphere.mPosition.y > navi_head_y;
			if (not_floor || above_head) {
				occluder = camPos.sqrDistance(tri->mSphere.mPosition) < sqr_navi_dist;
			}
		}

		int target = occluder ? OCCLUDER_ALPHA : 255;
		int a      = tri_alpha[idx];
		if (a < target) {
			a = (a + FADE_STEP < target) ? a + FADE_STEP : target;
		} else if (a > target) {
			a = (a - FADE_STEP > target) ? a - FADE_STEP : target;
		}
		tri_alpha[idx] = (u8)a;
	}

	// opaque triangles
	draw_triangles(triTable, vertTable, true);

	// translucent triangles
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
	draw_triangles(triTable, vertTable, false);

	// find unique opaque triangle edges
	edge_count = 0;
	for (int g = 0; g < gathered_count; g++) {
		int idx = gathered_tris[g];
		if (tri_alpha[idx] != 255) {
			continue;
		}
		Sys::Triangle* tri = triTable->getTriangle(idx);
		edge_set_insert(tri->mVertices[0], tri->mVertices[1]);
		edge_set_insert(tri->mVertices[1], tri->mVertices[2]);
		edge_set_insert(tri->mVertices[2], tri->mVertices[0]);
	}

	if (edge_count == 0) {
		return;
	}

	// edges
	GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetLineWidth(6, GX_TO_ZERO);

	GXBegin(GX_LINES, GX_VTXFMT0, (u16)(edge_count * 2));
	for (u32 i = 0; i < EDGE_SET_SIZE; i++) {
		u32 key = edge_set[i];
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
