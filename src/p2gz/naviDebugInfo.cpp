#include <p2gz/NaviDebugInfo.h>
#include <p2gz/p2gz.h>
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

#define NAVI_SWAP_DISPLAY_TIME (2.0f) // how long will swap text display (in seconds)

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
    , draw_face_dir(true)
    , draw_state(true)
    , draw_swap_time(true)
{
	swap_time               = 0.0f;
	swap_time_state         = SWAP_Inactive;
	swap_display_fade_timer = 0.0f;
	swap_navi_source_ID     = NAVIID_Olimar;
}

/// Call when captain swap starts, so we can start a timer
void NaviDebugInfo::swap_start(int startNaviID)
{
	// Segment timer code
	if (p2gz->timer->get_segment_timer_enabled() && p2gz->timer->split_on_captain_swap) {
		p2gz->timer->add_split_times(); // add the split time before the if condition so it isn't dependent on captain debug info enabled
	}

	if (!enabled || !draw_swap_time) {
		return;
	}
	swap_time_state         = SWAP_Swapping;
	swap_time               = 0.0f;
	swap_display_fade_timer = 0.0f;
	swap_navi_source_ID     = startNaviID; // record who we're swapping from (for text color)
	p2gz->timer->cancel_navi_swap_timer(); // make sure we don't have a timer running already somehow
	p2gz->timer->reset_navi_swap_timer();  // start timer
}

/// Call when captain swap ends, so we can calc and print the time taken
void NaviDebugInfo::swap_complete()
{
	swap_time_state         = SWAP_Complete;
	swap_time               = p2gz->timer->stop_navi_swap_timer();
	swap_display_fade_timer = NAVI_SWAP_DISPLAY_TIME;
}

/// Necessary
void NaviDebugInfo::update()
{
	if (!enabled) {
		return;
	}
	if (draw_swap_time) {
		switch (swap_time_state) {
		case SWAP_Inactive:
			// nothing to update
			break;
		case SWAP_Swapping:
			// timer is running, nothing to update here though
			break;
		case SWAP_Complete:
			// timer is complete, we have a value, draw it for a set time
			swap_display_fade_timer -= sys->getDeltaTime();
			if (swap_display_fade_timer <= 0.0f) {
				swap_display_fade_timer = 0.0f;
				swap_time_state         = SWAP_Inactive;
			}
			break;
		}
	}
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
		if (draw_face_dir) {
			j2d.print(x, z, "Face angle (deg)");
			j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "%.2f", RAD2DEG * activeNavi->getFaceDir());
			z += NAVI_DEBUG_LINE_HEIGHT;
		}
		if (draw_state) {
			j2d.print(x, z, "State");
			j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "%s", get_navi_state_name(activeNavi));
			z += NAVI_DEBUG_LINE_HEIGHT;
		}
	}
	if (draw_swap_time && swap_time_state == SWAP_Complete) {
		// make text color the one from previous captain so it stands out a bit
		JUtility::TColor color = (swap_navi_source_ID == NAVIID_Olimar) ? olimarTextColor : louieTextColor;
		COLOR(color);
		j2d.print(x, z, "Swap time");
		j2d.print(x + NAVI_DEBUG_COLUMN_WIDTH, z, "%.2f sec", swap_time);
		z += NAVI_DEBUG_LINE_HEIGHT;
	}
}
