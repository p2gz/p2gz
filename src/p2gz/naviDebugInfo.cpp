#include <p2gz/NaviDebugInfo.h>
#include <p2gz/Utility.h>
#include <P2JME/P2JME.h>
#include <JSystem/J2D/J2DPrint.h>
#include <Game/gamePlayData.h>

using namespace gz;
using namespace Game;

#define NAVI_DEBUG_DRAW_X       (415.0f)
#define NAVI_DEBUG_DRAW_Z       (100.0f)
#define NAVI_DEBUG_LINE_HEIGHT  (15.0f)
#define NAVI_DEBUG_COLUMN_WIDTH (110.0f)

#define COLOR(color)           \
	j2d.mCharColor.set(color); \
	j2d.mGradientColor.set(color)

NaviDebugInfo::NaviDebugInfo()
    : olimarTextColor(230, 90, 130, 200) // pink, for olimar
    , louieTextColor(90, 130, 230, 200)  // blue, for louie (also for president)
    , enabled(false)
    , draw_position(true)
    , draw_velocity(true)
    , draw_target_velocity(false)
    , draw_state(true)
{
}

void NaviDebugInfo::draw()
{
	if (!enabled) {
		return;
	}

	if (!naviMgr) {
		return;
	}

	if (!in_play()) {
		return;
	}

	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !naviMgr || !naviMgr->getActiveNavi()) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 12.0f;
	j2d.mGlyphHeight = 12.0f;

	f32 x = NAVI_DEBUG_DRAW_X;
	f32 z = NAVI_DEBUG_DRAW_Z;

	Navi* activeNavi = naviMgr->getActiveNavi();
	if (activeNavi) {
		JUtility::TColor color = (activeNavi->getNaviID() == NAVIID_Olimar) ? olimarTextColor : louieTextColor;
		COLOR(color);
		if (draw_position || draw_velocity || draw_target_velocity || draw_state) {
			j2d.print(x, z, "%s:",
			          (activeNavi->getNaviID() == NAVIID_Olimar)            ? "OLIMAR"
			          : (playData && playData->isStoryFlag(STORY_DebtPaid)) ? "PRESIDENT"
			                                                                : "LOUIE");
			z += NAVI_DEBUG_LINE_HEIGHT;
		}
		if (draw_position) {
			j2d.print(x, z, "Position");
			j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "x: %.2f", activeNavi->mPosition.x);
			z += NAVI_DEBUG_LINE_HEIGHT;
			j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "y: %.2f", activeNavi->mPosition.y);
			z += NAVI_DEBUG_LINE_HEIGHT;
			j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "z: %.2f", activeNavi->mPosition.z);
			z += NAVI_DEBUG_LINE_HEIGHT;
		}
		if (draw_velocity) {
			if (activeNavi->getCurrState() && activeNavi->getCurrState()->getCurrStateID() == NSID_Pellet && activeNavi->mPellet) {
				j2d.print(x, z, "Velocity (Pel)");
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "x: %.2f", activeNavi->mPellet->mRigid.mConfigs[0].mVelocity.x);
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "y: %.2f", activeNavi->mPellet->mRigid.mConfigs[0].mVelocity.y);
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "z: %.2f", activeNavi->mPellet->mRigid.mConfigs[0].mVelocity.z);
				z += NAVI_DEBUG_LINE_HEIGHT;
			} else {
				j2d.print(x, z, "Velocity");
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "x: %.2f", activeNavi->mVelocity.x);
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "y: %.2f", activeNavi->mVelocity.y);
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "z: %.2f", activeNavi->mVelocity.z);
				z += NAVI_DEBUG_LINE_HEIGHT;
			}
		}
		if (draw_target_velocity) {
			j2d.print(x, z, "Target Velocity");
			if (activeNavi->getCurrState() && activeNavi->getCurrState()->getCurrStateID() == NSID_Pellet && activeNavi->mPellet) {
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "x: -");
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "y: -");
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "z: -");
				z += NAVI_DEBUG_LINE_HEIGHT;
			} else {
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "x: %.2f", activeNavi->mTargetVelocity.x);
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "y: %.2f", activeNavi->mTargetVelocity.y);
				z += NAVI_DEBUG_LINE_HEIGHT;
				j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "z: %.2f", activeNavi->mTargetVelocity.z);
				z += NAVI_DEBUG_LINE_HEIGHT;
			}
		}
		if (draw_state) {
			j2d.print(x, z, "State");
			j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "%s", get_navi_state_name(activeNavi));
			z += NAVI_DEBUG_LINE_HEIGHT;
		}
	}
}
