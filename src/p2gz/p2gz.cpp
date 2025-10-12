#include <p2gz/p2gz.h>
#include <Game/CameraMgr.h>
#include <Game/gamePlayData.h>
#include <Game/Navi.h>
#include <Game/NaviState.h>
#include <JSystem/J2D/J2DPrint.h>
#include <og/Sound.h>
#include <Game/MapMgr.h>
#include <Game/PikiMgr.h>

using namespace gz;

P2GZ* p2gz;

P2GZ::P2GZ()
{
	menu = new GZMenu();
}

void P2GZ::update()
{
	menu->update();
}

void P2GZ::draw()
{
	Graphics* gfx = sys->getGfx();
	menu->draw(gfx);
}

void P2GZ::die_painfully(int navi_id)
{
	Game::NaviMgr* mgr = Game::naviMgr;
	Game::Navi* navi   = mgr->getAt(navi_id);
	if (navi) {
		navi->addDamage(9999.0, true /* this is the painful part */);
	}
}

void P2GZ::boing(int navi_id)
{
	Game::NaviMgr* mgr = Game::naviMgr;
	Game::Navi* navi   = mgr->getAt(navi_id);
	if (navi) {
		navi->mVelocity.add(Vector3f(0.0, 1000.0, 0.0));
	}
}

void P2GZ::teleport()
{
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (navi != nullptr) {
		Game::PlayCamera* camera = Game::cameraMgr->mCameraObjList[navi->getNaviID()];

		Graphics* gfx = sys->getGfx();
		gfx->initPerspPrintf(gfx->mCurrentViewport);
		gfx->initPrimDraw(nullptr);
		gfx->mOrthoGraph.setPort();

		J2DPrint inactive(JFWSystem::systemFont, 0.0f);
		inactive.initiate();
		inactive.mCharColor.set(JUtility::TColor(255, 255, 255, 128));
		inactive.mGradientColor.set(JUtility::TColor(255, 255, 255, 128));
		inactive.mGlyphWidth  = 16.0f;
		inactive.mGlyphHeight = 16.0f;

		J2DPrint active(JFWSystem::systemFont, 0.0f);
		active.initiate();
		active.mCharColor.set(JUtility::TColor(255, 255, 255, 255));
		active.mGradientColor.set(JUtility::TColor(255, 255, 255, 255));
		active.mGlyphWidth  = 16.0f;
		active.mGlyphHeight = 16.0f;

		if (p2gz->mIsScrollingCamera && p2gz->mIsSaveLoadPosition) {
			active.print(82, 92, "Save Position");
			active.print(82, 116, "Load Position");
			active.print(82, 140, "Go Back");

			for (int i = 0; i < 4; i++) {
				char position[32];
				sprintf(position, "(%d, %d)", int(p2gz->mSavedPositions[i].x), int(p2gz->mSavedPositions[i].z));
				if (p2gz->mSelectedPositionSlot == i) {
					active.print(50, 172 + (24 * i), "Slot %d - %s", i + 1,
					             p2gz->mSavedPositions[i] == Vector3f::zero ? "unused" : position);
				} else {
					inactive.print(50, 172 + (24 * i), "Slot %d - %s", i + 1,
					               p2gz->mSavedPositions[i] == Vector3f::zero ? "unused" : position);
				}
			}

			// NB: drawing J2DPictures must occur after all text is printed
			p2gz->mAButtonPicture->draw(50.0f, 76.0f, 16, 16, false, false, false);
			p2gz->mXButtonPicture->draw(50.0f, 100.0f, 16, 16, false, false, false);
			p2gz->mBButtonPicture->draw(50.0f, 124.0f, 16, 16, false, false, false);

			camera->mGoalPosition                            = camera->mGoalPosition;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 0.0f;

			if (navi->mController1->getButtonDown() & Controller::PRESS_UP
			    || navi->mController1->getButtonDown() & Controller::PRESS_DPAD_UP) {
				p2gz->mSelectedPositionSlot = (p2gz->mSelectedPositionSlot - 1 + 4) % 4;
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_DOWN
			           || navi->mController1->getButtonDown() & Controller::PRESS_DPAD_DOWN) {
				p2gz->mSelectedPositionSlot = (p2gz->mSelectedPositionSlot + 1) % 4;
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_A) {
				Vector3f naviPos                                   = camera->mGoalPosition;
				naviPos.y                                          = mapMgr->getMinY(camera->mGoalPosition);
				p2gz->mSavedPositions[p2gz->mSelectedPositionSlot] = naviPos;
				og::ogSound->setDecide();
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_B) {
				p2gz->mIsSaveLoadPosition = false;
			} else if (navi->mController1->getButtonDown() & Controller::PRESS_X) {
				// don't allow warping to an unused slot
				if (p2gz->mSavedPositions[p2gz->mSelectedPositionSlot] == Vector3f::zero) {
					og::ogSound->setError();
					return;
				}

				// exit scroll state
				p2gz->mIsScrollingCamera  = false;
				p2gz->mIsSaveLoadPosition = false;
				gameSystem->setPause(false, "cameraScroll", 3);

				// reset camera parameters
				camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);
				camera->mGoalVerticalAngle                       = PI / 8;
				camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

				// set navi position
				navi->setPosition(p2gz->mSavedPositions[p2gz->mSelectedPositionSlot], false);

				// set squad position
				Iterator<Piki> iterator(pikiMgr);
				CI_LOOP(iterator)
				{
					Piki* piki = *iterator;
					if (piki->mNavi == navi) {
						piki->setPosition(p2gz->mSavedPositions[p2gz->mSelectedPositionSlot], false);
					}
				}

				og::ogSound->setDecide();
			}

			return;
		}

		// enter scroll state
		if (!p2gz->mIsScrollingCamera && !Game::gameSystem->paused() && navi->getStateID() != NSID_ThrowWait
		    && navi->mController1->getButtonDown() & Controller::PRESS_DPAD_LEFT) {
			p2gz->mIsScrollingCamera = true;
			gameSystem->setPause(true, "cameraScroll", 3);

			// set camera parameters
			camera->mGoalPosition += Vector3f(0, p2gz->mScrollCameraZoom, 0);
			camera->mGoalVerticalAngle = PI / 2;

			og::ogSound->setOpen();
			return;
		}

		// switch captains
		if (p2gz->mIsScrollingCamera && naviMgr->getActiveNavi() != nullptr && navi->mController1->getButtonDown() & Controller::PRESS_Y) {
			Game::Navi* otherNavi = Game::naviMgr->getAt(GET_OTHER_NAVI(navi));
			int otherNaviID       = otherNavi->getStateID();

			if (otherNavi->isAlive() && otherNaviID != NSID_Nuku && otherNaviID != NSID_NukuAdjust && otherNaviID != NSID_Punch) {
				// play captain switch sound effect
				if (navi->mNaviIndex == NAVIID_Olimar) {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_LUI, 0);
				} else if (Game::playData->isStoryFlag(STORY_DebtPaid)) {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_SHACHO, 0);
				} else {
					PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_ORIMA, 0);
				}

				// reset camera position for current captain
				camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);

				Game::gameSystem->mSection->pmTogglePlayer();

				// update parameters for other navi's camera
				Game::cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)]->mGoalPosition += Vector3f(0, p2gz->mScrollCameraZoom, 0);
				Game::cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)]->mGoalVerticalAngle                       = PI / 2;
				Game::cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)]->mCameraParms->mSettingChangeSpeed.mValue = 1.0f;
				return;
			}
		}

		// cancel
		if (p2gz->mIsScrollingCamera && navi->mController1->getButtonDown() & Controller::PRESS_B) {
			// exit scrolling state
			p2gz->mIsScrollingCamera = false;
			gameSystem->setPause(false, "cameraScroll", 3);

			// reset camera parameters
			camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);
			camera->mGoalVerticalAngle                       = PI / 8;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

			og::ogSound->setCancel();
		}

		// warp
		if (p2gz->mIsScrollingCamera && navi->mController1->getButtonDown() & Controller::PRESS_A) {
			// exit scrolling state
			p2gz->mIsScrollingCamera = false;
			Game::gameSystem->setPause(false, "cameraScroll", 3);

			// reset camera parameters
			camera->mGoalPosition -= Vector3f(0, p2gz->mScrollCameraZoom, 0);
			camera->mGoalVerticalAngle                       = PI / 8;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

			// set navi position
			Vector3f naviPos = camera->mGoalPosition;
			naviPos.y        = Game::mapMgr->getMinY(camera->mGoalPosition);
			navi->setPosition(naviPos, false);

			// set squad position
			Iterator<Game::Piki> iterator(Game::pikiMgr);
			CI_LOOP(iterator)
			{
				Game::Piki* piki = *iterator;
				if (piki->mNavi == navi) {
					piki->setPosition(naviPos, false);
				}
			}

			og::ogSound->setDecide();
		}

		// open saved positions menu
		if (p2gz->mIsScrollingCamera && navi->mController1->getButtonDown() & Controller::PRESS_R) {
			p2gz->mIsSaveLoadPosition = true;
			return;
		}

		// calculate new camera position
		if (p2gz->mIsScrollingCamera) {
			// set camera parameters
			camera->mGoalVerticalAngle                       = PI / 2;
			camera->mCameraParms->mSettingChangeSpeed.mValue = 1.0f;

			// calculate control stick vector
			f32 ax = 0.0f;
			f32 az = ax;
			if (navi->mController1) {
				ax = -navi->mController1->getMainStickX();
				az = navi->mController1->getMainStickY();
			}
			Vector3f inputPos(ax, 0.0f, az);
			navi->reviseController(inputPos);

			f32 x = inputPos.x;
			f32 z = inputPos.z;

			Vector3f side = camera->getSideVector();
			Vector3f up   = camera->getUpVector();
			Vector3f view = camera->getViewVector();
			side.y        = 0.0f;

			side.qNormalise();

			if (up.y > view.y) {
				view.x = view.x;
				view.z = view.z;
			} else {
				view.x = up.x;
				view.z = up.z;
			}
			Vector3f view2D(view.x, 0.0f, view.z);
			view2D.qNormalise();

			Vector3f result(side * x + view2D * z);

			if (result != Vector3f::zero) {
				active.print(82, 92, "Move");
			} else {
				inactive.print(82, 92, "Move");
			}
			f32 cStickX = navi->mController1->getSubStickX();
			f32 cStickY = navi->mController1->getSubStickY();
			if (cStickX != 0 || cStickY != 0) {
				active.print(82, 116, "Camera");
			} else {
				inactive.print(82, 116, "Camera");
			}

			inactive.print(82, 140, "Warp");
			inactive.print(82, 164, "Cancel");
			inactive.print(82, 188, "Switch");

			if (navi->mController1->getButton() & Controller::PRESS_L) {
				active.print(82, 212, "Move Faster");
			} else {
				inactive.print(82, 212, "Move Faster");
			}

			inactive.print(82, 236, "Saved Positions");

			p2gz->mControlStickPicture->draw(50.0f, 76.0f, 16, 16, false, false, false);
			p2gz->mCStickPicture->draw(50.0f, 100.0f, 16, 16, false, false, false);
			p2gz->mAButtonPicture->draw(50.0f, 124.0f, 16, 16, false, false, false);
			p2gz->mBButtonPicture->draw(50.0f, 148.0f, 16, 16, false, false, false);
			p2gz->mYButtonPicture->draw(50.0f, 172.0f, 16, 16, false, false, false);
			p2gz->mLButtonPicture->draw(50.0f, 196.0f, 16, 16, false, false, false);
			p2gz->mRButtonPicture->draw(50.0f, 220.0f, 16, 16, false, false, false);

			// calculate and set new position, unless there is no collision
			int speed = (navi->mController1->getButton() & Controller::PRESS_L) ? 32 : 16;
			Vector3f goalPosition(camera->mGoalPosition.x + result.x * speed, camera->mGoalPosition.y,
			                      camera->mGoalPosition.z + result.z * speed);

			// circle color
			int rgb      = p2gz->getAnimationCoefficient() * 255;
			Color4 color = Color4(rgb, rgb, rgb, 255);

			if (mapMgr->getMinY(goalPosition) > -1000) {
				camera->mGoalPosition = goalPosition;
			} else {
				gfx->mOrthoGraph.setPort();
				J2DPrint print(JFWSystem::systemFont, 0.0f);
				print.initiate();
				print.mCharColor.set(JUtility::TColor(rgb, 0, 0, 255));
				print.mGradientColor.set(JUtility::TColor(rgb, 0, 0, 255));
				print.mGlyphWidth  = 16.0f;
				print.mGlyphHeight = 16.0f;
				print.print(100, 200, "Can't move out of bounds!");
				color = Color4(rgb, 0, 0, 255);
			}

			// change camera angle with dampening on c-stick value
			camera->mCameraAngleTarget -= cStickX * 0.05f;

			// set zoom level with clamping
			if (p2gz->mScrollCameraZoom > 0 && p2gz->mScrollCameraZoom - cStickY * 25 > 0 && p2gz->mScrollCameraZoom < 1700
			    && p2gz->mScrollCameraZoom - cStickY * 25 < 1700) {
				p2gz->mScrollCameraZoom -= cStickY * 25;
				camera->mGoalPosition -= Vector3f(0, cStickY * 25, 0);
				if (cStickY > 0) {
					og::ogSound->setZoomIn();
				} else if (cStickY < 0) {
					og::ogSound->setZoomOut();
				}
			}

			// draw circle
			gfx->initPerspPrintf(gfx->mCurrentViewport);

			f32 radius       = 4 + 4 * p2gz->getAnimationCoefficient();
			Vector3f naviPos = camera->mGoalPosition;
			Vector3f vertices[3];
			vertices[0] = naviPos;

			for (int i = 0; i < 32; i++) {
				f32 theta   = -HALF_PI - (TAU * i / 32);
				vertices[1] = Vector3f(radius * sinf(theta), 0.0f, radius * cosf(theta)) + naviPos;

				f32 nextTheta = -HALF_PI - (TAU * (i + 1) / 32);
				vertices[2]   = Vector3f(radius * sinf(nextTheta), 0.0f, radius * cosf(nextTheta)) + naviPos;

				GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 3);
				for (int j = 0; j < 3; j++) {
					GXPosition3f32(vertices[j].x, vertices[j].y, vertices[j].z);
					GXColor4u8(color.r, color.g, color.b, 255);
				}
				GXEnd();
			}
		}
	}
}