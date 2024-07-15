#include "og/newScreen/Cave.h"
#include "og/Screen/DopingScreen.h"
#include "og/Screen/NaviLifeGauge.h"
#include "og/Screen/PikminCounter.h"
#include "og/Screen/TotalPokoScreen.h"
#include "og/Screen/BloGroup.h"
#include "og/Screen/ogScreen.h"
#include "trig.h"
#include "System.h"
#include "nans.h"

namespace og {
namespace newScreen {

ObjHoleIn::ObjHoleIn(const char* name)
{
	mName            = name;
	mDisp            = nullptr;
	mScreenControls  = nullptr;
}

ObjHoleIn::~ObjHoleIn() { }

void ObjHoleIn::doCreate(JKRArchive* arc)
{
	og::Screen::DispMemberCave* dispfull = static_cast<og::Screen::DispMemberCave*>(getDispMember());
	mDisp = static_cast<og::Screen::DispMemberHoleIn*>(dispfull->getSubMember(OWNER_OGA, MEMBER_P2GZ_HOLE_IN));
	if (!mDisp) {
		og::Screen::DispMemberHoleIn* newdisp = new og::Screen::DispMemberHoleIn;
		mDisp = static_cast<og::Screen::DispMemberHoleIn*>(newdisp->getSubMember(OWNER_OGA, MEMBER_P2GZ_HOLE_IN));
	}

	mScreenControls = new P2DScreen::Mgr_tuning;
	mScreenControls->set("cavecontrols.blo", 0x1040000, arc);
}

void ObjHoleIn::commonUpdate()
{
	mScreenControls->update();
}

bool ObjHoleIn::doUpdate()
{
	commonUpdate();
	return false;
}

void ObjHoleIn::doDraw(Graphics& gfx)
{
	J2DPerspGraph* graf = &gfx.mPerspGraph;
	graf->setPort();
	if (mScreenControls) {
		mScreenControls->draw(gfx, *graf);
	}
}

bool ObjHoleIn::doStart(::Screen::StartSceneArg const*)
{
	return true;
}

bool ObjHoleIn::doEnd(::Screen::EndSceneArg const*)
{
	return true;
}

bool ObjHoleIn::doUpdateFadein()
{
	return true;
}

void ObjHoleIn::doUpdateFadeinFinish() { }

void ObjHoleIn::doUpdateFinish() { }

bool ObjHoleIn::doUpdateFadeout()
{
	return true;
}

void ObjHoleIn::doUpdateFadeoutFinish() { }

ObjHoleIn::StaticValues ObjHoleIn::msVal;

} // namespace newScreen
} // namespace og
