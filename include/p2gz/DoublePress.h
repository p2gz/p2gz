#ifndef _DOUBLEPRESS_H
#define _DOUBLEPRESS_H

#include <JSystem/JUtility/JUTGamePad.h>

namespace gz {

class DoublePress {
public:
    DoublePress(JUTGamePad::EButton button_, size_t window_)
      : button(button_),
        window(window_),
        frames_left(0) {}

    bool check(JUTGamePad* controller) {
        if (frames_left > 0) {
            frames_left -= 1;
        }

        if (controller->getButtonDown() & button) {
            if (frames_left > 0) {
                return true;
            }
            frames_left = window;
        }

        return false;
    }

    void reset() {
        frames_left = 0;
    }

private:
    const JUTGamePad::EButton button;
    size_t frames_left;
    size_t window;
};

} // namespace gz

#endif