#include <p2gz/CollisionViewer.h>
#include <Game/Navi.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <Graphics.h>
#include <stl/math.h>
#include <Game/MapMgr.h>

using namespace gz;

const int RENDER_DISTANCE = 16;

bool CollisionViewer::is_navi_on_triangle(Sys::Triangle* tri, Sys::VertexTable& vertTable)
{
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (navi == nullptr || tri == nullptr) {
		return false;
	}

	Sys::Triangle* naviTriangle = Game::naviMgr->getActiveNavi()->mFloorTriangle;
	if (naviTriangle == nullptr) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		Vector3f naviVertex = *vertTable.getVertex(naviTriangle->mVertices[i]);
		Vector3f triVertex  = *vertTable.getVertex(tri->mVertices[i]);
		if (naviVertex != triVertex) {
			return false;
		}
	}
	return true;
}

void CollisionViewer::draw_triangles(Sys::Sphere& sphere)
{
	Game::ShapeMapMgr* shapeMapMgr = static_cast<Game::ShapeMapMgr*>(Game::mapMgr);
	Sys::TriIndexList* triLists    = shapeMapMgr->mMapCollision.mDivider->findTriLists(sphere);
	Sys::VertexTable& vertTable    = *shapeMapMgr->mMapCollision.mDivider->mVertexTable;
	Sys::TriangleTable& triTable   = *shapeMapMgr->mMapCollision.mDivider->mTriangleTable;

	if (triLists == nullptr) {
		return;
	}

	for (int i = 0; i < triLists->mCount; i++) {
		Sys::Triangle* tri = triTable.getTriangle(triLists->mObjects[i]);
		Color4 color       = Color4(200, 200, 200, 128);
		if (!is_navi_on_triangle(tri, vertTable)) {
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

		GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
		for (int i = 0; i < 3; i++) {
			Vector3f vertex = *vertTable.getVertex(tri->mVertices[i]);
			GXPosition3f32(vertex.x, vertex.y, vertex.z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
		GXEnd();
	}
}

void CollisionViewer::draw()
{
	if (!enabled) {
		return;
	}

	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (navi == nullptr) {
		return;
	}

	Graphics* gfx = sys->getGfx();
	gfx->initPerspPrintf(gfx->mCurrentViewport);
	gfx->initPrimDraw(nullptr);

	Vector3f naviPos = navi->getPosition();
	for (int i = -RENDER_DISTANCE; i <= RENDER_DISTANCE; i++) {
		for (int j = -RENDER_DISTANCE; j <= RENDER_DISTANCE; j++) {
			Vector3f scoutPos = naviPos + Vector3f(32 * i, 0, 32 * j);
			Sys::Sphere scout(scoutPos, 0.0f);
			draw_triangles(scout);
		}
	}
}
