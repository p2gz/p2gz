#ifndef _GZ_TIMER_H
#define _GZ_TIMER_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/os.h>

// forward declarations
namespace Game {
struct MovieConfig;
} // namespace Game

namespace gz {

// forward declarations
struct ListMenu;

/// timer offsets, in seconds
/// NB: these might be the same, but they're split out in case we want to edit them later
#define CAVE_ENTER_SAVE_OFFSET_TIME    (4.0f)
#define NEXT_SUBLEVEL_SAVE_OFFSET_TIME (4.0f)

/// NB: this doesn't include fade-in or fade-out, since we can't skip til fade in, and have to fade out anyway
#define MAX_TREASURE_CUTSCENE_TIME (8.5f)

/// TODO: make individual ones of these for each upgrade depending on text length
#define MAX_UPGRADE_CUTSCENE_TIME (13.5f)

/// for intro/crash landing cutscene
#define MAX_CRASH_LANDING_CUTSCENE_TIME (37.5f)

/// for all other "first time enter" cutscenes
#define MAX_FIRST_ENTER_CUTSCENE_TIME (13.0f)

/// for defining the maximum number of split times and segments to keep track of in the segment timer
#define MAX_NUMBER_SEGMENTS (20)

struct Timer {
public:
	Timer();
	~Timer() { }

	void init();
	void draw();

	void enable();
	void disable();
	void set_enabled(bool on);
	void set_sub_timer_enabled(bool on);
	void on_reset();
	void sync();

	void reset_main_timer();
	void reset_sub_timer();
	void reset_main_timer(f32 offset_seconds);
	void reset_sub_timer(f32 offset_seconds);

	void offset_main_timer(f32 offset_seconds);
	void offset_sub_timer(f32 offset_seconds);

	void reset_skip_timer();
	void stop_skip_timer(Game::MovieConfig* config);
	void cancel_skip_timer();

	void pause();
	void unpause();
	u32 get_elapsed_time() { return ((get_cur_time() - sub_timer) / 1000); } // this returns time since last loaded section in seconds

	// @P2GZ race mode: elapsed RTA in ms (respects pause, so reads frozen while paused)
	u32 get_main_elapsed_ms();

	void reset_navi_swap_timer();
	f32 stop_navi_swap_timer();
	void cancel_navi_swap_timer();

	void set_FS_map_flag(bool flag) { FS_map_flag = flag; }
	bool get_FS_map_flag() { return FS_map_flag; }

	void set_freecam_mode(bool set) { in_freecam_mode = set; }
	bool is_freecam_mode() { return in_freecam_mode; }

	// Segment timer functions and variables
	void add_split_times();
	void reset_split_times();
	void set_segment_timer_enabled(bool enabled_) { segment_timer_enabled = enabled_; }
	void set_split_on_captain_swap_enabled(bool enabled_) { split_on_captain_swap = enabled_; }
	void set_split_on_gate_seg_enabled(bool enabled_) { split_on_gate_seg = enabled_; }
	void set_split_on_gate_down_enabled(bool enabled_) { split_on_gate_down = enabled_; }
	void set_split_on_bag_crush_enabled(bool enabled_) { split_on_bag_crush = enabled_; }
	void set_split_on_poison_demo_enabled(bool enabled_) { split_on_poison_demo = enabled_; }
	void set_split_on_carry_enabled(bool enabled_) { split_on_carry = enabled_; }
	void set_split_on_enemy_death_enabled(bool enabled_) { split_on_enemy_death = enabled_; }
	void set_draw_best_times_enabled(bool enabled_) { draw_best_times_enabled = enabled_; }
	void set_draw_comparisons_enabled(bool enabled_) { draw_comparisons_enabled = enabled_; }
	void reset_best_segments();
	void enable_mark_run_to_discard() { mark_run_for_discard = true; }

	bool split_on_captain_swap;
	bool split_on_gate_seg;
	bool split_on_gate_down;
	bool split_on_bag_crush;
	bool split_on_poison_demo;
	bool split_on_carry;
	bool split_on_enemy_death;

private:
	struct TimeComponents {
		u32 minutes;
		u32 seconds;
		u32 tenths;
	};

	static u32 get_cur_time() { return (u32)OSTicksToMilliseconds(OSGetTime()); }
	static TimeComponents calc_time(u32 start_time);
	static TimeComponents calc_time(u32 start_time, u32 end_time);

	bool enabled;
	bool sub_timer_enabled;
	bool skip_timer_set;
	bool pause_timer_set;
	bool navi_swap_timer_set;

	bool FS_map_flag;     // are we loading into the world map/select area from file select?
	bool in_freecam_mode; // handle pausing timer differently when we close the menu for freecam

	bool segment_timer_enabled; // are we using the segment timer?
	bool draw_best_times_enabled;
	bool mark_run_for_discard;
	bool draw_comparisons_enabled;

	u32 main_timer;  // overall/default timer starting point
	u32 sub_timer;   // sublevel timer starting point
	u32 skip_timer;  // for offsets because of P2GZ toggles
	u32 pause_timer; // for when gz menu is open

	u32 navi_swap_timer; // for measuring captain swap times

	char curr_index;                        // tracks current active index of split_times
	u32 split_times[MAX_NUMBER_SEGMENTS];   // tracks timestamps of trigger events
	u32 segment_times[MAX_NUMBER_SEGMENTS]; // tracks the segment times to display on screen
	u32 best_segments[MAX_NUMBER_SEGMENTS];

	// menu hook
	ListMenu* timer_menu;

	// display settings
	JUtility::TColor color;
	f32 glyph_width;
	f32 glyph_height;
	f32 x;
	f32 z;
};

}; // namespace gz

#endif
