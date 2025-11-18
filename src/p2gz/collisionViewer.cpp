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

const f32 RENDER_RADIUS = 1024.0f;

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

	if (!triLists) {
		return;
	}

	for (triLists; triLists; triLists = static_cast<Sys::TriIndexList*>(triLists->mNext)) {
		for (int i = 0; i < triLists->getNum(); i++) {
			Sys::Triangle* tri = triTable->getTriangle(triLists->mObjects[i]);
			Color4 color       = Color4(200, 200, 200, 128);
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

			GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
			for (int i = 0; i < 3; i++) {
				Vector3f* vertex = vertTable->getVertex(tri->mVertices[i]);
				GXPosition3f32(vertex->x, vertex->y, vertex->z);
				GXColor4u8(color.r, color.g, color.b, color.a);
			}
			GXEnd();
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

	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (navi) {
		draw_triangles(navi->mNaviIndex == NAVIID_Olimar ? olimarSphere : louieSphere);
	} else {
		// Minimize duplicate triangles while switching captains by only drawing triangles for both
		// if they are arbitrarily far apart.
		draw_triangles(olimarSphere);
		if (sqrDistanceXZ(olimarSphere.mPosition, louieSphere.mPosition) > RENDER_RADIUS / 3) {
			draw_triangles(louieSphere);
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
