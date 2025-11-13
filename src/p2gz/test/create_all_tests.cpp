#include <p2gz/Test.h>
#include <Dolphin/pad.h>
#include <p2gz/HelperInlines.h>

using namespace gz;
using namespace gz::test;

void TestRunner::create_all_tests()
{
	// clang-format off
	tests.push(TEST("warp from menu",
        WAIT(300)
        DBL_DPAD_L
        PRESS(PAD_BUTTON_A)
        PRESS(PAD_BUTTON_UP)
        PRESS(PAD_BUTTON_A)
        WAIT_FOR(in_above_ground_play)
        DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
    ));
	tests.push(TEST("warp to ww day 31",
        DBL_DPAD_L
        PRESS(PAD_BUTTON_A)
        PRESS(PAD_BUTTON_LEFT)
        DO_N(3, PRESS(PAD_BUTTON_DOWN))
	    DO_N(28, PRESS(PAD_BUTTON_RIGHT))
        DO_N(2, PRESS(PAD_BUTTON_DOWN))
        PRESS(PAD_BUTTON_A)
	    WAIT_FOR(in_above_ground_play)
        DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
    ));
	// clang-format on
}
