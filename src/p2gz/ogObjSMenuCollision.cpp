#include "Game/gamePlayData.h"
#include "Game/MapMgr.h"
#include "og/newScreen/SMenu.h"
#include "og/Screen/anime.h"
#include "og/Screen/ogScreen.h"
#include "og/Sound.h"

namespace og {
namespace newScreen {

static void _Print(char* format, ...) { OSReport(format, __FILE__); }

ObjSMenuCollision::ObjSMenuCollision(char const* name)
{
	mDisp       = nullptr;
	mScreenCollision = nullptr;
	mAnimGroup  = nullptr;
	mName       = name;
}

ObjSMenuCollision::~ObjSMenuCollision() { }

void ObjSMenuCollision::doCreate(JKRArchive* arc)
{
	og::Screen::DispMemberSMenuAll* dispfull = static_cast<og::Screen::DispMemberSMenuAll*>(getDispMember());
	mDisp = static_cast<og::Screen::DispMemberSMenuCollision*>(dispfull->getSubMember(OWNER_OGA, MEMBER_P2GZ_COLLISION));
	if (!mDisp) {
		og::Screen::DispMemberSMenuAll* newdisp = new og::Screen::DispMemberSMenuAll;
		mDisp = static_cast<og::Screen::DispMemberSMenuCollision*>(newdisp->getSubMember(OWNER_OGA, MEMBER_P2GZ_COLLISION));
	}

	mScreenCollision = new P2DScreen::Mgr_tuning;
	mScreenCollision->set("s_menu_collision.blo", 0x1040000, arc);

	mAnimGroup = new og::Screen::AnimGroup(2);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenCollision, "s_menu_controller.btk", msBaseVal.mAnimSpeed);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenCollision, "s_menu_controller_02.btk", msBaseVal.mAnimSpeed);

	mCurrentSetting = 0;
	mNumSettings = 4;
	mIsToggling = false;

	mLabels[0] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Twp'));
	mLabels[1] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Tscore'));
	mLabels[2] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Tcoll'));
	mLabels[3] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Thb'));

	mOn[0] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Twp_y'));
	mOn[1] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Tscore_y'));
	mOn[2] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Tcoll_y'));
	mOn[3] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Thb_y'));

	mOff[0] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Twp_n'));
	mOff[1] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Tscore_n'));
	mOff[2] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Tcoll_n'));
	mOff[3] = static_cast<J2DTextBoxEx*>(og::Screen::TagSearch(mScreenCollision, 'Thb_n'));

	mToggles[0] = Game::gameSystem->mIsWaypointsEnabled;
	mToggles[1] = Game::gameSystem->mIsEnemyStateEnabled;
	mToggles[2] = Game::gameSystem->mIsCollisionEnabled;
	mToggles[3] = false;

	mLabels[0]->setAlpha(255);
	mOn[0]->setAlpha(mToggles[0] ? 255 : 128);
	mOff[0]->setAlpha(mToggles[0] ? 128 : 255);

	for (int i = 1; i < mNumSettings; i++) {
		mLabels[i]->setAlpha(128);
		mOn[i]->setAlpha(mToggles[i] ? 255 : 128);
		mOff[i]->setAlpha(mToggles[i] ? 128 : 255);
	}

	doCreateAfter(arc, mScreenCollision);
}

void ObjSMenuCollision::doUpdateLAction()
{
	// LEFT = PAUSE MENU
	og::Screen::DispMemberSMenuAll* disp = static_cast<og::Screen::DispMemberSMenuAll*>(getDispMember());
	if (disp->mSMenuMap.mInCave) {
		::Screen::SetSceneArg arg(SCENE_PAUSE_MENU_DOUKUTU, getDispMember());
		jump_L(arg);
	} else {
		::Screen::SetSceneArg arg(SCENE_PAUSE_MENU, getDispMember());
		jump_L(arg);
	}
}

void ObjSMenuCollision::doUpdateRAction()
{
	// RIGHT = SQUAD
	::Screen::SetSceneArg arg(SCENE_P2GZ_SQUAD, getDispMember());
	jump_R(arg);
}

void ObjSMenuCollision::commonUpdate()
{
	commonUpdateBase();
	setSMenuScale(msVal.mScaleX, msVal.mScaleY);
	mAnimGroup->update();
	mScreenCollision->setXY(mMovePos, 0.0f);
	mScreenCollision->update();
}

bool ObjSMenuCollision::doUpdate()
{
	commonUpdate();

	Controller* pad = getGamePad();
	u32 input = pad->getButtonDown();

	if (input & Controller::PRESS_UP) {
		mLabels[mCurrentSetting]->setAlpha(128);
		mCurrentSetting = (mCurrentSetting - 1 + mNumSettings) % mNumSettings;
		mLabels[mCurrentSetting]->setAlpha(255);
		ogSound->setPlusMinus(false);
	} else if (input & Controller::PRESS_DOWN) {
		mLabels[mCurrentSetting]->setAlpha(128);
		mCurrentSetting = (mCurrentSetting + 1) % mNumSettings;
		mLabels[mCurrentSetting]->setAlpha(255);
		ogSound->setPlusMinus(false);
	} else if (input & Controller::PRESS_LEFT || input & Controller::PRESS_RIGHT) {
		mToggles[mCurrentSetting] = !mToggles[mCurrentSetting];
		mOn[mCurrentSetting]->setAlpha(mToggles[mCurrentSetting] ? 255 : 128);
		mOff[mCurrentSetting]->setAlpha(mToggles[mCurrentSetting] ? 128 : 255);

		// TODO: Make this pointers to the booleans lol
		switch (mCurrentSetting) {
			case 0:
			Game::gameSystem->mIsWaypointsEnabled = mToggles[mCurrentSetting];
			break;

			case 1:
			Game::gameSystem->mIsEnemyStateEnabled = mToggles[mCurrentSetting];
			break;

			case 2:
			Game::gameSystem->mIsCollisionEnabled = mToggles[mCurrentSetting];
			if (mToggles[mCurrentSetting]) {
				static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapModel->hide();
			} else {
				static_cast<Game::ShapeMapMgr*>(Game::mapMgr)->mMapModel->show();
			}
			break;
		}
		ogSound->setPlusMinus(false);
	}

	bool ret = ObjSMenuBase::doUpdate();
	mScreenCollision->animation();
	return ret;
}

void ObjSMenuCollision::doDraw(Graphics& gfx)
{
	J2DPerspGraph* graf = &gfx.mPerspGraph;
	graf->setPort();
	if (mScreenCollision) {
		mScreenCollision->draw(gfx, *graf);
	}
	drawYaji(gfx);
}

void ObjSMenuCollision::in_L()
{
	mState = MENUSTATE_OpenL;
	mAngle = 15.0;
}

void ObjSMenuCollision::in_R()
{
	mState = MENUSTATE_OpenR;
	mAngle = 15.0;
}

void ObjSMenuCollision::wait() { mState = MENUSTATE_Default; }

void ObjSMenuCollision::out_L()
{
	mState = MENUSTATE_CloseL;
	ogSound->setSMenuLR();
}

void ObjSMenuCollision::out_R()
{
	mState = MENUSTATE_CloseR;
	ogSound->setSMenuLR();
}

bool ObjSMenuCollision::doStart(::Screen::StartSceneArg const* arg)
{
	mAnimGroup->setFrame(0.0f);
	mAnimGroup->setRepeat(true);
	mAnimGroup->setSpeed(1.0f);
	mAnimGroup->start();
	setYajiName('6052_00', '3003_00', '3006_00'); // @P2GZ: change to "Menu" "Squad" "Collision"
	stopYaji();
	return start_LR(arg);
}

bool ObjSMenuCollision::doEnd(::Screen::EndSceneArg const*) { return true; }

void ObjSMenuCollision::doUpdateFinish() { ObjSMenuBase::doUpdateFinish(); }

bool ObjSMenuCollision::doUpdateFadeout()
{
	commonUpdate();
	return updateFadeOut();
}

void ObjSMenuCollision::doUpdateCancelAction() { }

ObjSMenuCollision::StaticValues ObjSMenuCollision::msVal;

} // namespace newScreen
} // namespace og
