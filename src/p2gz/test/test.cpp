#include <p2gz/p2gz.h>
#include <p2gz/Test.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include <Dolphin/pad.h>

using namespace gz;
using namespace gz::test;

TestRunner::TestRunner()
{
	inited   = false;
	cur_test = 0;
	gamepad  = new JUTGamePadRecordFixed();
}

void TestRunner::init()
{
	OSReport("GZTest: initializing\n");

	tests.push(new Test("open menu and warp", 11,
	                    (TestOp* [11]) {
	                        new Wait(300),
	                        new ButtonInput(PAD_BUTTON_LEFT),
	                        new Wait(1),
	                        new ButtonInput(PAD_BUTTON_LEFT),
	                        new Wait(1),
	                        new ButtonInput(PAD_BUTTON_A),
	                        new Wait(1),
	                        new ButtonInput(PAD_BUTTON_UP),
	                        new Wait(1),
	                        new ButtonInput(PAD_BUTTON_A),
	                        new Wait(1),
	                        // new Wait(300),
	                    }));

	inited = true;
}

void TestRunner::update()
{
	if (!inited) {
		return;
	}

	if (cur_test >= tests.len()) {
		return;
	}

	Test* test = tests[cur_test];
	GZASSERTLINE(test);

	const bool test_done = test->update();
	if (test_done) {
		cur_test += 1;
	}

	if (cur_test >= tests.len()) {
		OSReport("GZTest: Done with all tests\n");
	}
}

Test::Test(const char* name_, const size_t num_ops_, TestOp** ops_)
    : num_ops(num_ops_)
    , ops(num_ops_)
{
	GZASSERTLINE(name_);
	name    = name_;
	cur_op  = 0;
	started = false;

	for (size_t i = 0; i < num_ops; i++) {
		ops.push(ops_[i]);
	}
}

bool Test::update()
{
	if (cur_op == 0 && !started) {
		OSReport("GZTest: Starting test\t%s\n", name);
		p2gz->controller->mPadReplay = p2gz->test_runner->gamepad;
		started                      = true;
	} else if (cur_op >= num_ops) {
		return true;
	}

	p2gz->test_runner->gamepad->clear();
	const bool op_done = ops[cur_op]->execute();
	if (op_done) {
		cur_op += 1;
	}

	if (cur_op >= num_ops) {
		OSReport("GZTest: Finished test\t%s\n", name);
		p2gz->controller->mPadReplay = nullptr;
		return true;
	}

	return false;
}

bool Wait::execute()
{
	if (frames == 0) {
		return true;
	}
	frames -= 1;
	return false;
}

bool ButtonInput::execute()
{
	hold_frames -= 1;
	p2gz->test_runner->gamepad->status.button |= button;
	if (hold_frames <= 0) {
		return true;
	}
	return false;
}
