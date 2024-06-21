#include "Game/gamePlayData.h"
#include "og/newScreen/SMenu.h"
#include "og/Screen/anime.h"
#include "og/Screen/ogScreen.h"
#include "og/Sound.h"

namespace og {
namespace newScreen {

static void _Print(char* format, ...) { OSReport(format, __FILE__); }

ObjSMenuWarp::ObjSMenuWarp(char const* name)
{
	mDisp       = nullptr;
	mScreenWarp = nullptr;
	mAnimGroup  = nullptr;
	mName       = name;
}

ObjSMenuWarp::~ObjSMenuWarp() { }

void ObjSMenuWarp::doCreate(JKRArchive* arc)
{
	og::Screen::DispMemberSMenuAll* dispfull = static_cast<og::Screen::DispMemberSMenuAll*>(getDispMember());
	mDisp = static_cast<og::Screen::DispMemberSMenuWarp*>(dispfull->getSubMember(OWNER_OGA, MEMBER_P2GZ_WARP));
	if (!mDisp) {
		og::Screen::DispMemberSMenuAll* newdisp = new og::Screen::DispMemberSMenuAll;
		mDisp = static_cast<og::Screen::DispMemberSMenuWarp*>(newdisp->getSubMember(OWNER_OGA, MEMBER_P2GZ_WARP));
	}

	mScreenWarp = new P2DScreen::Mgr_tuning;
	mScreenWarp->set("s_menu_warp.blo", 0x1040000, arc);

	mAnimGroup = new og::Screen::AnimGroup(2);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenWarp, "s_menu_controller.btk", msBaseVal.mAnimSpeed);
	og::Screen::registAnimGroupScreen(mAnimGroup, arc, mScreenWarp, "s_menu_controller_02.btk", msBaseVal.mAnimSpeed);

	doCreateAfter(arc, mScreenWarp);
}

void ObjSMenuWarp::doUpdateLAction()
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

void ObjSMenuWarp::doUpdateRAction()
{
	// RIGHT = SQUAD
	::Screen::SetSceneArg arg(SCENE_P2GZ_SQUAD, getDispMember());
	jump_R(arg);
}

void ObjSMenuWarp::commonUpdate()
{
	commonUpdateBase();
	setSMenuScale(msVal.mScaleX, msVal.mScaleY);
	mAnimGroup->update();
	mScreenWarp->setXY(mMovePos, 0.0f);
	mScreenWarp->update();
}

bool ObjSMenuWarp::doUpdate()
{
	commonUpdate();

	bool ret = ObjSMenuBase::doUpdate();
	mScreenWarp->animation();
	return ret;
}

void ObjSMenuWarp::doDraw(Graphics& gfx)
{
	J2DPerspGraph* graf = &gfx.mPerspGraph;
	graf->setPort();
	if (mScreenWarp) {
		mScreenWarp->draw(gfx, *graf);
	}
	drawYaji(gfx);
}

void ObjSMenuWarp::in_L()
{
	mState = MENUSTATE_OpenL;
	mAngle = 15.0;
}

void ObjSMenuWarp::in_R()
{
	mState = MENUSTATE_OpenR;
	mAngle = 15.0;
}

void ObjSMenuWarp::wait() { mState = MENUSTATE_Default; }

void ObjSMenuWarp::out_L()
{
	mState = MENUSTATE_CloseL;
	ogSound->setSMenuLR();
}

void ObjSMenuWarp::out_R()
{
	mState = MENUSTATE_CloseR;
	ogSound->setSMenuLR();
}

bool ObjSMenuWarp::doStart(::Screen::StartSceneArg const* arg)
{
	mAnimGroup->setFrame(0.0f);
	mAnimGroup->setRepeat(true);
	mAnimGroup->setSpeed(1.0f);
	mAnimGroup->start();
	setYajiName('6052_00', '3003_00', '3006_00'); // @P2GZ: change to "Menu" "Squad" "Warp"
	stopYaji();
	return start_LR(arg);
}

bool ObjSMenuWarp::doEnd(::Screen::EndSceneArg const*) { return true; }

void ObjSMenuWarp::doUpdateFinish() { ObjSMenuBase::doUpdateFinish(); }

bool ObjSMenuWarp::doUpdateFadeout()
{
	commonUpdate();
	return updateFadeOut();
}

void ObjSMenuWarp::doUpdateCancelAction() { }

ObjSMenuWarp::StaticValues ObjSMenuWarp::msVal;

} // namespace newScreen
} // namespace og
