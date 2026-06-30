#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/timer.h>
#include <p2gz/Utility.h>
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

#define COLOR(color)           \
	j2d.mCharColor.set(color); \
	j2d.mGradientColor.set(color)

Timer::Timer()
    : enabled(false)
    , sub_timer_enabled(true)
    , skip_timer_set(false)
    , pause_timer_set(false)
    , navi_swap_timer_set(false)
    , FS_map_flag(false)
    , in_freecam_mode(false)
    , segment_timer_enabled(false) // controls whether or not the segment timer is turned on
    , draw_best_times_enabled(false)
    , split_on_captain_swap(false)
    , split_on_gate_seg(false)
    , split_on_gate_down(false)
    , split_on_bag_crush(false)
    , split_on_poison_demo(false)
    , split_on_carry(false)
    , split_on_enemy_death(false)
    , split_on_cave_enter(true)
    , mark_run_for_discard(false)
    , main_timer(0)
    , sub_timer(0)
    , skip_timer(0)
    , pause_timer(0)
    , navi_swap_timer(0)
    , curr_index(0) // gets updated every time a function writes to split_times
{
	color        = JUtility::TColor(255, 255, 255, 130);
	glyph_width  = 16.0;
	glyph_height = 16.0;
	x            = 12.0;
	z            = 12.0;
	for (int i = 0; i < MAX_NUMBER_SEGMENTS; i++) {
		split_times[i]   = 0;
		segment_times[i] = 0;
		best_segments[i] = 0;
	}
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

	// race mode draws its own RTA/IGT instead, don't draw it twice
	if (p2gz->race_mode && p2gz->race_mode->is_active()) {
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

	if (segment_timer_enabled && !p2gz->menu->is_open()) {
		f32 starting_seg_offset = z + glyph_height + 10.0f;
		// Draw segment times
		// smaller font sizes for the segment times
		j2d.mGlyphWidth  = 14.0f;
		j2d.mGlyphHeight = 14.0f;
		const f32 decimal_x   = 12.0f + j2d.getWidth("1:59.99");
		
		for (int i = 0; i < curr_index; i++) {
			if (split_times[i] > 0) {
				Timer::TimeComponents seg_c;
				// calc_time(x,y) returns y-x as a TimeComponents object. If x is unspecified, it takes the value main_timer.
				// main_timer changes everytime the gz menu is paused, so we avoid dealing with it entirely
				if (i == 0) {
					seg_c = calc_time(0, segment_times[0]);
				} else {
					seg_c = calc_time(0, segment_times[i]);
				}

				if (seg_c.minutes > 0) {
					j2d.print(decimal_x - j2d.getWidth("%ld:%2ld.%ld", seg_c.minutes, seg_c.seconds, seg_c.tenths), starting_seg_offset + (i * 16),
					          "%ld:%2ld.%ld", seg_c.minutes, seg_c.seconds, seg_c.tenths);
				} else {
					j2d.print(decimal_x - j2d.getWidth("%ld.%ld", seg_c.seconds, seg_c.tenths), starting_seg_offset + (i * 16), "%ld.%ld",
					          seg_c.seconds, seg_c.tenths);
				}

				if (draw_best_times_enabled && (best_segments[i] > 0)) {
					// fixed offset so it produces aligned columns
					f32 sub_offset = 60.0f;
					seg_c          = calc_time(0, best_segments[i]);
					if (seg_c.minutes > 0) {
						j2d.print(decimal_x + sub_offset - j2d.getWidth("%ld:%2ld.%ld", seg_c.minutes, seg_c.seconds, seg_c.tenths),
						          starting_seg_offset + (i * 16), "%ld:%2ld.%ld", seg_c.minutes, seg_c.seconds, seg_c.tenths);
					} else {
						j2d.print(decimal_x + sub_offset - j2d.getWidth("%ld.%ld", seg_c.seconds, seg_c.tenths), starting_seg_offset + (i * 16),
						          "%ld.%ld", seg_c.seconds, seg_c.tenths);
					}
				}

				if (draw_best_times_enabled && draw_comparisons_enabled && (best_segments[i] > 0)) {
					// fixed offset so it produces aligned columns
					f32 sub_offset = 120.0f;
					bool isGreen   = segment_times[i] < best_segments[i];
					if (!isGreen) {
						// alternative: calc_time(best_segments[i], segment_times[i])
						// I don't like this since these are not the start and endpoint of a period of elapsed time
						seg_c = calc_time(0, segment_times[i] - best_segments[i]);
						JUtility::TColor red_color(205, 30, 30, 204);
						COLOR(red_color);
					} else {
						seg_c = calc_time(0, best_segments[i] - segment_times[i]);
						JUtility::TColor green_color(30, 205, 30, 204);
						COLOR(green_color);
					}
					if (seg_c.minutes > 0) {
						j2d.print(
						    decimal_x + sub_offset
						        - j2d.getWidth(isGreen ? "-%ld:%2ld.%ld" : "+%ld:%ld.%ld", seg_c.minutes, seg_c.seconds, seg_c.tenths),
						    starting_seg_offset + (i * 16), isGreen ? "-%ld:%2ld.%ld" : "+%ld:%ld.%ld", seg_c.minutes, seg_c.seconds,
						    seg_c.tenths);
					} else {
						j2d.print(decimal_x + sub_offset - j2d.getWidth(isGreen ? "-%ld.%ld" : "+%ld.%ld", seg_c.seconds, seg_c.tenths),
						          starting_seg_offset + (i * 16), isGreen ? "-%ld.%ld" : "+%ld.%ld", seg_c.seconds, seg_c.tenths);
					}
					COLOR(color);
				}
			}
		}
	}
	// reset the font size
	j2d.mGlyphWidth  = glyph_width;
	j2d.mGlyphHeight = glyph_height;
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

	// Segment timer
	reset_split_times();
}

void Timer::reset_main_timer()
{
	main_timer = get_cur_time();
	reset_sub_timer();

	// Segment timer
	reset_split_times();
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

u32 Timer::stop_skip_timer(Game::MovieConfig* config)
{
	if (!skip_timer_set || !config) {
		return 0;
	}

	f32 max_cutscene_time = 0.0f;

	if (config->is("x01_gamestart")) {
		// intro crash landing cutscene
		max_cutscene_time = MAX_CRASH_LANDING_CUTSCENE_TIME;
	} else if (config->is("x01_coursein_forest") || config->is("x01_coursein_yakushima") || config->is("x01_coursein_last")) {
		// first area enter cutscene (all three others are the same length)
		max_cutscene_time = MAX_FIRST_ENTER_CUTSCENE_TIME;
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
		return 0;
	}

	int remaining = skip_timer + (max_cutscene_time * 1000.0f) - get_cur_time();
	if (remaining < 0) {
		remaining = 0;
	}

	main_timer -= remaining;
	sub_timer -= remaining;
	skip_timer_set = false;

	// hand back the unwatched vanilla cutscene time we just compensated for, so timers that run on
	// their own clock (e.g. race mode) can apply the same offset
	return (u32)remaining;
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
	pause_timer     = get_cur_time(); // this is not a delta
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

void Timer::reset_navi_swap_timer()
{
	if (navi_swap_timer_set) {
		return;
	}
	navi_swap_timer     = get_cur_time();
	navi_swap_timer_set = true;
}

f32 Timer::stop_navi_swap_timer()
{
	if (!navi_swap_timer_set) {
		return;
	}

	// duration in milliseconds
	int diff = get_cur_time() - navi_swap_timer;
	if (diff < 0) {
		diff = 0;
	}

	return (f32)(diff) / 1000.0f;
}

void Timer::cancel_navi_swap_timer()
{
	if (!navi_swap_timer_set) {
		return;
	}
	navi_swap_timer     = 0;
	navi_swap_timer_set = false;
}

// main function which populates the split_times and segment_times arrays
void Timer::add_split_times()
{
	if (!segment_timer_enabled) {
		return;
	}

	u32 split_time = get_cur_time() - main_timer;

	if (curr_index < MAX_NUMBER_SEGMENTS) { // 20 by default
		split_times[curr_index] = split_time;
		if (curr_index == 0) {
			segment_times[0] = split_times[0];
		} else {
			segment_times[curr_index] = split_times[curr_index] - split_times[curr_index - 1];
		}
		curr_index++;
	}
}

void Timer::reset_best_segments()
{
	for (int i = 0; i < MAX_NUMBER_SEGMENTS; i++) {
		best_segments[i] = 0;
	}
}

// resets all the split/segment time related stuff
void Timer::reset_split_times()
{
	for (int i = 0; i < MAX_NUMBER_SEGMENTS; i++) {
		if ((!mark_run_for_discard) && ((segment_times[i] != 0) && ((best_segments[i] == 0) || (segment_times[i] < best_segments[i])))) {
			best_segments[i] = segment_times[i];
		}
		split_times[i]   = 0;
		segment_times[i] = 0;
	}
	curr_index           = 0;
	mark_run_for_discard = false;
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
