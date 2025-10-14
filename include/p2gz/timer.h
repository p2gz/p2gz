#ifndef _GZ_TIMER_H
#define _GZ_TIMER_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/os.h>

namespace gz {

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

private:
	struct TimeComponents {
		u32 minutes;
		u32 seconds;
		u32 tenths;
	};

	static u32 get_cur_time() { return (u32)OSTicksToMilliseconds(OSGetTime()); }
	static TimeComponents calc_time(u32 start_time);

	bool enabled;
	bool sub_timer_enabled;
	bool has_been_set;

	u32 main_timer;
	u32 sub_timer;

	// display settings
	JUtility::TColor color;
	f32 glyph_width;
	f32 glyph_height;
	f32 x;
	f32 z;
};

}; // namespace gz

#endif
