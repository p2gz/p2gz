#ifndef _GZ_INPUT_HELPERS_H
#define _GZ_INPUT_HELPERS_H

#include <Controller.h>
#include <System.h>
#include <p2gz/Utility.h>

namespace gz {

class DoublePress {
public:
	DoublePress(JUTGamePad::EButton button_, size_t window_)
	    : button(button_)
	    , window(window_)
	    , frames_left(0)
	{
	}

	bool check(Controller* controller)
	{
		if (frames_left > 0) {
			frames_left -= 1;
		}

		if (controller->getButtonDown() & button) {
			if (frames_left > 0) {
				return true;
			}
			frames_left = is_30_fps() ? window : window * 2;
		}

		return false;
	}

	void reset() { frames_left = 0; }

private:
	const JUTGamePad::EButton button;
	size_t frames_left;
	size_t window;
};

class PressAndHold {
public:
	static const size_t INIT_REPEAT_DELAY = 9;
	static const size_t REPEAT_DELAY      = 2;

	PressAndHold(JUTGamePad::EButton button_, size_t initial_repeat_delay_ = INIT_REPEAT_DELAY, size_t repeat_delay_ = REPEAT_DELAY)
	    : button(button_)
	    , initial_repeat_delay(initial_repeat_delay_)
	    , repeat_delay(repeat_delay_)
	{
		repeating   = false;
		frames_left = -1;
	}

	bool check(Controller* controller)
	{
		if (controller->getButton() & button) {
			if (frames_left == 0) {
				if (repeating) {
					frames_left = is_30_fps() ? repeat_delay : repeat_delay * 2;
				} else {
					frames_left = is_30_fps() ? initial_repeat_delay : initial_repeat_delay * 2;
					repeating   = true;
				}
				return true;
			} else {
				frames_left -= 1;
			}
		} else {
			frames_left = 0;
			repeating   = false;
		}
		return false;
	}

private:
	const JUTGamePad::EButton button;
	const size_t initial_repeat_delay;
	const size_t repeat_delay;
	bool repeating;
	size_t frames_left;
};

} // namespace gz

#endif
