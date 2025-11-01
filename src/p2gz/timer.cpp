#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/timer.h>
#include <p2gz/HelperInlines.h>

using namespace gz;

Timer::Timer()
    : enabled(false)
    , sub_timer_enabled(true)
    , main_timer_set(false)
    , sub_timer_set(false)
    , skip_timer_set(false)
    , pause_timer_set(false)
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

void Timer::reset_main_timer()
{
	main_timer = get_cur_time();
	reset_sub_timer();
}

void Timer::reset_sub_timer()
{
	sub_timer = get_cur_time();

	// If loading directly into a cave from file select, the initial
	// timer set that happens at init time is way too early, so do an
	// extra reset to bring it in line with the normal floor start time.
	if (!main_timer_set) {
		main_timer     = get_cur_time();
		main_timer_set = true;
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

void Timer::stop_skip_timer()
{
	if (skip_timer_set) {
		return;
	}
	skip_timer     = get_cur_time();
	skip_timer_set = true;
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

	main_timer = get_cur_time();
	sub_timer  = get_cur_time();
	enabled    = true;
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
