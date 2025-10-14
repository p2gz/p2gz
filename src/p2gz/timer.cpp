#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/timer.h>

using namespace gz;

Timer::Timer()
    : enabled(false)
    , sub_timer_enabled(true)
    , has_been_set(false)
    , main_timer(0)
    , sub_timer(0)
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

	Timer::TimeComponents main_c = calc_time(main_timer);
	if (sub_timer_enabled) {
		Timer::TimeComponents sub_c = calc_time(sub_timer);
		j2d.print(x, z, "%ld:%.2ld:%.1ld (%ld:%.2ld:%.1ld)", main_c.minutes, main_c.seconds, main_c.tenths, sub_c.minutes, sub_c.seconds,
		          sub_c.tenths);
	} else {
		j2d.print(x, z, "%ld:%.2ld:%.1ld", main_c.minutes, main_c.seconds, main_c.tenths);
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
	if (!has_been_set) {
		main_timer   = get_cur_time();
		has_been_set = true;
	}
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
