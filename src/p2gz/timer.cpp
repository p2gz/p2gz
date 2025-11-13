#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/timer.h>
#include <p2gz/HelperInlines.h>
#include <p2gz/p2gz.h>

#include "Morimura/HurryUp.h"
#include "Morimura/mrUtil.h"
#include "PSSystem/PSSystemIF.h"
#include "Game/GameSystem.h"
#include "Game/MoviePlayer.h"
#include "Game/gamePlayData.h"
#include "Game/Navi.h"
#include "nans.h"

using namespace gz;

Timer::Timer()
    : enabled(false)
    , sub_timer_enabled(true)
    , skip_timer_set(false)
    , pause_timer_set(false)
    , FS_map_flag(false)
    , in_freecam_mode(false)
    , main_timer(0)
    , sub_timer(0)
    , skip_timer(0)
    , pause_timer(0)
{
	color        = JUtility::TColor(255, 255, 255, 130);
	glyph_width  = 16.0;
	glyph_height = 16.0;
	x            = 12.0;
	z            = 12.0;
}

void Timer::init()
{
	enable();
	reset_main_timer();
	timer_menu = static_cast<ListMenu*>(p2gz->menu->get_option("timer")->get_sub_menu());
}

void Timer::draw()
{
	if (!enabled) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = glyph_width;
	j2d.mGlyphHeight = glyph_height;
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	// main timer calculations
	Timer::TimeComponents main_c = calc_time(main_timer);

	if (pause_timer_set) {
		// if we're paused, do static calc with no update
		main_c = calc_time(main_timer, pause_timer);
	}
	j2d.print(x, z, "%ld:%.2ld.%.1ld", main_c.minutes, main_c.seconds, main_c.tenths);

	// sub timer calculations
	if (sub_timer_enabled) {
		f32 sub_offset = (main_c.minutes < 10) ? 65.0f : (main_c.minutes < 100) ? 80.0f : 95.0f;
		// if you have the main timer going for longer than 999 minutes, go touch grass.

		Timer::TimeComponents sub_c = calc_time(sub_timer);
		if (pause_timer_set) {
			sub_c = calc_time(sub_timer, pause_timer);
		}
		j2d.print(x + sub_offset, z, "(%ld:%.2ld.%.1ld)", sub_c.minutes, sub_c.seconds, sub_c.tenths);
	}
}

void Timer::sync()
{
	static_cast<ToggleMenuOption*>(timer_menu->get_option("enabled"))->set_selection(enabled);
	static_cast<ToggleMenuOption*>(timer_menu->get_option("show sub-timer"))->set_selection(sub_timer_enabled);
}

Timer::TimeComponents Timer::calc_time(u32 start_time)
{
	u32 current_time = get_cur_time();
	u32 elapsed      = current_time - start_time;

	Timer::TimeComponents components;
	components.minutes = elapsed / (60 * 1000);
	components.seconds = (elapsed / 1000) % 60;
	components.tenths  = (elapsed / 100) % 10;

	return components;
}

Timer::TimeComponents Timer::calc_time(u32 start_time, u32 end_time)
{
	u32 elapsed = end_time - start_time;

	Timer::TimeComponents components;
	components.minutes = elapsed / (60 * 1000);
	components.seconds = (elapsed / 1000) % 60;
	components.tenths  = (elapsed / 100) % 10;

	return components;
}

void Timer::on_reset()
{
	main_timer = get_cur_time();
	sub_timer  = get_cur_time();
	if (pause_timer_set) {
		pause_timer = get_cur_time();
	}
}

void Timer::reset_main_timer()
{
	main_timer = get_cur_time();
	reset_sub_timer();
}

void Timer::reset_main_timer(f32 offset_seconds)
{
	if (offset_seconds >= 0.0f) {
		u32 secs      = floor(offset_seconds);
		u32 tenths    = floor((offset_seconds - floor(offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		main_timer    = get_cur_time() - millisecs;
	} else {
		u32 secs      = floor(-offset_seconds);
		u32 tenths    = floor((-offset_seconds - floor(-offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		main_timer    = get_cur_time() + millisecs;
	}
	reset_sub_timer(offset_seconds);
}

void Timer::offset_main_timer(f32 offset_seconds)
{
	if (offset_seconds >= 0.0f) {
		// "add" to timer
		u32 secs      = floor(offset_seconds);
		u32 tenths    = floor((offset_seconds - floor(offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		main_timer -= millisecs;
	} else {
		// "subtract" from timer
		u32 secs      = floor(-offset_seconds);
		u32 tenths    = floor((-offset_seconds - floor(-offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		main_timer += millisecs;
	}
	offset_sub_timer(offset_seconds);
}

void Timer::reset_sub_timer()
{
	sub_timer = get_cur_time();
}

void Timer::reset_sub_timer(f32 offset_seconds)
{
	if (offset_seconds >= 0.0f) {
		u32 secs      = floor(offset_seconds);
		u32 tenths    = floor((offset_seconds - floor(offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		sub_timer     = get_cur_time() - millisecs;
	} else {
		u32 secs      = floor(-offset_seconds);
		u32 tenths    = floor((-offset_seconds - floor(-offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		sub_timer     = get_cur_time() + millisecs;
	}
}
void Timer::offset_sub_timer(f32 offset_seconds)
{
	if (offset_seconds >= 0.0f) {
		// "add" to timer
		u32 secs      = floor(offset_seconds);
		u32 tenths    = floor((offset_seconds - floor(offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		sub_timer -= millisecs;
	} else {
		// "subtract" from timer
		u32 secs      = floor(-offset_seconds);
		u32 tenths    = floor((-offset_seconds - floor(-offset_seconds)) * 10.0f);
		u32 millisecs = secs * 1000 + tenths * 100;
		sub_timer += millisecs;
	}
}

void Timer::reset_skip_timer()
{
	if (skip_timer_set) {
		return;
	}
	skip_timer     = get_cur_time();
	skip_timer_set = true;
}

void Timer::stop_skip_timer(Game::MovieConfig* config)
{
	if (!skip_timer_set || !config) {
		return;
	}

	f32 max_cutscene_time = 0.0f;

	if (config->is("x01_gamestart")) {
		// intro crash landing cutscene
		max_cutscene_time = MAX_CRASH_LANDING_CUTSCENE_TIME;
	} else if (config->is("s22_cv_suck_treasure") || config->is("s10_suck_treasure")) {
		// treasure cutscene
		max_cutscene_time = MAX_TREASURE_CUTSCENE_TIME;
	} else if (config->is("s22_cv_suck_equipment") || config->is("s17_suck_equipment")) {
		// upgrade cutscene
		max_cutscene_time = MAX_UPGRADE_CUTSCENE_TIME;
	} else {
		// we have a skip timer going during the wrong cutscene, something is Wrong
		OSReport("[P2GZ WARN] stop_skip_timer: unhandled config name\n");
		OSReport("[P2GZ WARN] >> config: %s\n", config->mMovieNameBuffer2);
		return;
	}

	int remaining = skip_timer + (max_cutscene_time * 1000.0f) - get_cur_time();
	if (remaining < 0) {
		remaining = 0;
	}

	main_timer -= remaining;
	sub_timer -= remaining;
	skip_timer_set = false;
}

void Timer::cancel_skip_timer()
{
	if (!skip_timer_set) {
		return;
	}
	skip_timer     = 0;
	skip_timer_set = false;
}

void Timer::pause()
{
	if (pause_timer_set) {
		return;
	}
	pause_timer     = get_cur_time();
	pause_timer_set = true;
}

void Timer::unpause()
{
	if (!pause_timer_set) {
		return;
	}
	main_timer += (get_cur_time() - pause_timer);
	sub_timer += (get_cur_time() - pause_timer);
	if (skip_timer_set) {
		skip_timer += (get_cur_time() - pause_timer);
	}
	pause_timer_set = false;
}

void Timer::enable()
{
	if (enabled) {
		return;
	}

	enabled = true;
}

void Timer::disable()
{
	enabled = false;
}

void Timer::set_enabled(bool on)
{
	if (on) {
		enable();
	} else {
		disable();
	}
}

void Timer::set_sub_timer_enabled(bool on)
{
	sub_timer_enabled = on;
}

// @Extracted: hurryUp2D.s scaleUp2__Q28Morimura10THurryUp2DFv

namespace Morimura {

/**
 * @note Address: 0x8034792C
 * @note Size: 0x240
 */
void THurryUp2D::scaleUp2()
{
	f32 goal = mParams[mState].mGoalScale;
	if (mPaneSunL->mScale.x < goal) {
		f32 factor = mTimer * mScaleSp2 * 60.0f;
		f32 scale  = factor * sys->mDeltaTime + mParams[mState].mScale;
		if (scale > goal) {
			scale = goal;
		}
		u8 alpha = mFadeFraction * u8(mAlphaMod1 * scale + mAlphaMod2);
		mPaneHurry2->setAlpha(alpha);
		mPaneSundown2->setAlpha(alpha);
		mPaneSunL->setAlpha(alpha);
		mPaneSunW->setAlpha(alpha);
		mWhitePane->setAlpha(0);
		mPaneSunL->updateScale(scale);
	} else {
		if (!mIsSection && (Game::gameSystem->isFlag(Game::GAMESYS_IsGameWorldActive)) && !mIsSection && Game::moviePlayer
		    && !Game::playData->isDemoFlag(Game::DEMO_First_Sunset_Warning)) {
			Game::MoviePlayArg arg("g09_first_sunset", nullptr, nullptr, 0);
			Game::Navi* navi = Game::naviMgr->getActiveNavi();
			if (navi && navi->mCamera) {
				// @P2GZ sunset-pause-fix: close p2gz menu when first sunset warning cutscene plays to prevent unpause desync issues
				if (p2gz && p2gz->menu->is_open()) {
					p2gz->menu->close();
				}
				Game::playData->setDemoFlag(Game::DEMO_First_Sunset_Warning);
				Game::moviePlayer->mTargetNavi   = navi;
				Game::moviePlayer->mActingCamera = navi->mCamera;
				Game::moviePlayer->play(arg);
			}
		}
	}
}

} // namespace Morimura
