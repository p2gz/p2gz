#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/timer.h>
#include <p2gz/HelperInlines.h>

using namespace gz;

Timer::Timer()
    : enabled(false)
    , sub_timer_enabled(true)
    , skip_timer_set(false)
    , pause_timer_set(false)
    , FS_map_flag(false)
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

	enable();
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

void Timer::stop_skip_timer_treasure()
{
	if (!skip_timer_set) {
		return;
	}
	OSReport("Skip timer value: %d\n", skip_timer);
	OSReport("Current time: %d\n", get_cur_time());
	int remaining = skip_timer + (MAX_TREASURE_CUTSCENE_TIME * 1000.0f) - get_cur_time();
	OSReport("remaining: %d\n", remaining);
	if (remaining < 0) {
		remaining = 0;
	}

	OSReport("Old main timer: %d\n", main_timer);
	main_timer -= remaining;
	OSReport("New main timer: %d\n", main_timer);
	sub_timer -= remaining;
	skip_timer_set = false;
}

void Timer::stop_skip_timer_upgrade()
{
	if (!skip_timer_set) {
		return;
	}
	int remaining = skip_timer + (MAX_UPGRADE_CUTSCENE_TIME * 1000.0f) - get_cur_time();
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

void Timer::reset_pause_timer()
{
	if (pause_timer_set) {
		return;
	}
	pause_timer     = get_cur_time();
	pause_timer_set = true;
}

void Timer::stop_pause_timer()
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

	main_timer  = get_cur_time();
	sub_timer   = get_cur_time();
	pause_timer = get_cur_time();
	enabled     = true;
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
