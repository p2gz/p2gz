#include "Game/gamePlayData.h"
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
