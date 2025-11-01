#ifndef _GZ_TIMER_H
#define _GZ_TIMER_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/os.h>

namespace gz {

/// timer offsets, in seconds
/// NB: these might be the same, but they're split out in case we want to edit them later
#define CAVE_ENTER_SAVE_OFFSET_TIME    (4.0f)
#define NEXT_SUBLEVEL_SAVE_OFFSET_TIME (4.0f)

/// NB: this doesn't include fade-in or fade-out, since we can't skip til fade in, and have to fade out anyway
#define MAX_TREASURE_CUTSCENE_TIME (8.5f)

/// TODO: make individual ones of these for each upgrade depending on text length
#define MAX_UPGRADE_CUTSCENE_TIME (13.5f)

struct Timer {
public:
	Timer();
	~Timer() { }

	void draw();

	void enable();
	void disable();
	void set_enabled(bool on);
	void set_sub_timer_enabled(bool on);

	void reset_main_timer();
	void reset_sub_timer();
	void reset_main_timer(f32 offset_seconds);
	void reset_sub_timer(f32 offset_seconds);

	void offset_main_timer(f32 offset_seconds);
	void offset_sub_timer(f32 offset_seconds);

	void reset_skip_timer();
	void stop_skip_timer_treasure();
	void stop_skip_timer_upgrade();
	void cancel_skip_timer();

	void reset_pause_timer();
	void stop_pause_timer();

	void set_FS_map_flag(bool flag) { FS_map_flag = flag; }
	bool get_FS_map_flag() { return FS_map_flag; }

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

	bool FS_map_flag; // are we loading into the world map/select area from file select?

	u32 main_timer;  // overall/default timer
	u32 sub_timer;   // sublevel timer
	u32 skip_timer;  // for offsets because of P2GZ toggles
	u32 pause_timer; // for when gz menu is open

	// display settings
	JUtility::TColor color;
	f32 glyph_width;
	f32 glyph_height;
	f32 x;
	f32 z;
};

}; // namespace gz

#endif
