#include <p2gz/FreeCam.h>
#include <p2gz/p2gz.h>
#include <Game/CameraMgr.h>
#include <Game/Navi.h>
#include <Game/NaviState.h>
#include <og/Sound.h>
#include <Game/MapMgr.h>
#include <Game/PikiMgr.h>

using namespace gz;

const int MIN_FREECAM_ZOOM       = 0;
const int DEFAULT_FREECAM_ZOOM   = 768;
const int MAX_FREECAM_ZOOM       = 1700;
const int OUT_OF_BOUNDS_MIN_Y    = -1000;
const f32 C_STICK_X_AXIS_SCALING = 0.05f;
const f32 C_STICK_Y_AXIS_SCALING = 25.0f;
const f32 ANIMATION_INCREMENT    = 1.0f / 30.0f;
const int NORMAL_SPEED           = 16;
const int FAST_SPEED             = 32;
char* FREECAM_PAUSE_IDENTIFIER   = "freecam";

// Enable freecam on the active Navi's camera.
void FreeCam::enable()
{
	if (enabled)
		return;

	enabled = true;
	p2gz->menu->close();

	navi   = Game::naviMgr->getActiveNavi();
	camera = Game::cameraMgr->mCameraObjList[navi->getNaviID()];
	Game::gameSystem->setPause(true, FREECAM_PAUSE_IDENTIFIER, 3);

	zoom = DEFAULT_FREECAM_ZOOM;
	camera->mGoalPosition += Vector3f(0, zoom, 0);
	camera->mGoalVerticalAngle = PI / 2;

	og::ogSound->setOpen();
}

// Disable freecam on the active Navi's camera.
void FreeCam::disable()
{
	if (!enabled)
		return;

	enabled = false;
	Game::gameSystem->setPause(false, FREECAM_PAUSE_IDENTIFIER, 3);

	camera->mGoalPosition -= Vector3f(0, zoom, 0);
	camera->mGoalVerticalAngle                       = PI / 8;
	camera->mCameraParms->mSettingChangeSpeed.mValue = 0.1f;

	navi   = nullptr;
	camera = nullptr;

	og::ogSound->setCancel();
}

// Perform all updates to the freecam based on the controller inputs on this frame.
void FreeCam::update()
{
	if (!enabled)
		return;

	navi = Game::naviMgr->getActiveNavi();
	if (navi == nullptr) {
		return;
	}
	camera = Game::cameraMgr->mCameraObjList[navi->getNaviID()];

	update_position();
	update_zoom();

	if (navi->mController1->getButtonDown() & Controller::PRESS_A) {
		warp_to_current_position();
		return;
	}

	if (navi->mController1->getButtonDown() & Controller::PRESS_B) {
		if (!p2gz->menu->is_open()) {
			p2gz->menu->navigate_to("tools/freecam");
		}

		disable();
		return;
	}

	if (navi->mController1->getButtonDown() & Controller::PRESS_Y) {
		switch_captains();
		return;
	}

	if (is_coefficient_positive) {
		animation_coefficient += 1.0f / 30.0f;
		if (animation_coefficient >= 1.0f) {
			is_coefficient_positive = false;
		}
	} else {
		animation_coefficient -= 1.0f / 30.0f;
		if (animation_coefficient <= 0.0f) {
			is_coefficient_positive = true;
		}
	}
}

// Switch captains and transfer the freecam state to the other Navi's camera.
void FreeCam::switch_captains()
{
	GZASSERTLINE(enabled);

	Game::Navi* otherNavi         = Game::naviMgr->getAt(GET_OTHER_NAVI(navi));
	Game::PlayCamera* otherCamera = Game::cameraMgr->mCameraObjList[GET_OTHER_NAVI(navi)];
	int otherNaviID               = otherNavi->getStateID();

	if (otherNavi->isAlive() && otherNaviID != Game::NSID_Nuku && otherNaviID != Game::NSID_NukuAdjust && otherNaviID != Game::NSID_Punch) {
		if (navi->mNaviIndex == NAVIID_Olimar) {
			PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_LUI, 0);
		} else if (Game::playData->isStoryFlag(Game::STORY_DebtPaid)) {
			PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_SHACHO, 0);
		} else {
			PSSystem::spSysIF->playSystemSe(PSSE_SY_CHANGE_ORIMA, 0);
		}

		camera->mGoalPosition -= Vector3f(0, zoom, 0);

		Game::gameSystem->mSection->pmTogglePlayer();

		otherCamera->mGoalPosition += Vector3f(0, zoom, 0);
		otherCamera->mGoalVerticalAngle                       = PI / 2;
		otherCamera->mCameraParms->mSettingChangeSpeed.mValue = 1.0f;
	}
}

// Disable freecam and warp the active Navi and all Pikmin in their squad to the current position.
void FreeCam::warp_to_current_position()
{
	GZASSERTLINE(enabled);

	Vector3f naviPos = camera->mGoalPosition;
	naviPos.y        = Game::mapMgr->getMinY(camera->mGoalPosition);
	navi->setPosition(naviPos, false);

	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mNavi == navi) {
			piki->setPosition(naviPos, false);
		}
	}

	disable();

	og::ogSound->setDecide();
}

// Update the freecam's position based on the control stick inputs on this frame.
void FreeCam::update_position()
{
	GZASSERTLINE(enabled);

	camera->mGoalVerticalAngle                       = PI / 2;
	camera->mCameraParms->mSettingChangeSpeed.mValue = 1.0f;

	f32 ax = 0.0f;
	f32 az = 0.0f;
	if (navi->mController1) {
		ax = -navi->mController1->getMainStickX();
		az = navi->mController1->getMainStickY();
	}
	Vector3f inputPos(ax, 0.0f, az);
	navi->reviseController(inputPos);

	f32 x = inputPos.x;
	f32 z = inputPos.z;

	Vector3f side = camera->getSideVector();
	side.y        = 0.0f;
	side.qNormalise();

	Vector3f up   = camera->getUpVector();
	Vector3f view = camera->getViewVector();
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

	int speed = NORMAL_SPEED;
	if (navi->mController1 && navi->mController1->getButton() & Controller::PRESS_L) {
		speed = FAST_SPEED;
	}
	Vector3f goalPosition(camera->mGoalPosition.x + result.x * speed, camera->mGoalPosition.y, camera->mGoalPosition.z + result.z * speed);
	if (Game::mapMgr->getMinY(goalPosition) > OUT_OF_BOUNDS_MIN_Y) {
		camera->mGoalPosition = goalPosition;
	}
}

// Update the freecam's zoom based on the C-stick inputs on this frame.
void FreeCam::update_zoom()
{
	GZASSERTLINE(enabled);

	f32 cStickX = navi->mController1->getSubStickX() * C_STICK_X_AXIS_SCALING;
	f32 cStickY = navi->mController1->getSubStickY() * C_STICK_Y_AXIS_SCALING;

	camera->mCameraAngleTarget -= cStickX;
	if (zoom > MIN_FREECAM_ZOOM && zoom - cStickY > MIN_FREECAM_ZOOM && zoom < MAX_FREECAM_ZOOM && zoom - cStickY < MAX_FREECAM_ZOOM) {
		zoom -= cStickY;
		camera->mGoalPosition -= Vector3f(0, cStickY, 0);
		if (cStickY > 0) {
			og::ogSound->setZoomIn();
		} else if (cStickY < 0) {
			og::ogSound->setZoomOut();
		}
	}
}

void FreeCam::draw()
{
	if (!enabled) {
		return;
	}
	draw_current_position();
}

// Draw a circle on the ground indicating the freecam's current position.
void FreeCam::draw_current_position()
{
	Graphics* gfx = sys->getGfx();
	gfx->initPerspPrintf(gfx->mCurrentViewport);

	int rgb      = 255 * animation_coefficient;
	Color4 color = Color4(rgb, rgb, rgb, 255);

	f32 radius       = 4 + 4 * animation_coefficient;
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
