#include <p2gz/CollisionViewer.h>
#include <Game/Navi.h>
#include <Sys/TriIndexList.h>
#include <Sys/Triangle.h>
#include <Sys/TriangleTable.h>
#include <Graphics.h>
#include <stl/math.h>
#include <Game/MapMgr.h>
#include <Game/mapParts.h>
#include <p2gz/p2gz.h>

const int RENDER_DISTANCE = 16;

namespace gz {
bool CollisionViewer::is_navi_on_triangle(Sys::Triangle* tri, Sys::VertexTable* vertTable)
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
		Vector3f naviVertex = *vertTable->getVertex(naviTriangle->mVertices[i]);
		Vector3f triVertex  = *vertTable->getVertex(tri->mVertices[i]);
		if (naviVertex != triVertex) {
			return false;
		}
	}
	return true;
}

void CollisionViewer::draw_triangles(Sys::Sphere& sphere)
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

	if (triLists == nullptr) {
		return;
	}

	for (int i = 0; i < triLists->mCount; i++) {
		Sys::Triangle* tri = triTable->getTriangle(triLists->mObjects[i]);
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
			Vector3f vertex = *vertTable->getVertex(tri->mVertices[i]);
			GXPosition3f32(vertex.x, vertex.y, vertex.z);
			GXColor4u8(color.r, color.g, color.b, color.a);
		}
		GXEnd();
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

	enabled = enabled_;
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
