#include <p2gz/p2gz.h>
#include <p2gz/Test.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include <Dolphin/pad.h>
#include <P2JME/P2JME.h>

using namespace gz;
using namespace gz::test;

static const bool dbg = false;
#define LOG(...)                 \
	if (dbg) {                   \
		OSReport("[DBG] ==== "); \
		OSReport(__VA_ARGS__);   \
	}

TestRunner::TestRunner()
{
	inited   = false;
	cur_test = 0;
	gamepad  = new JUTGamePadRecordFixed();
}

void TestRunner::init()
{
	OSReport("GZTest: initializing\n");
	create_all_tests();
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

void TestRunner::draw_2d()
{
	if (!inited || cur_test >= tests.len()) {
		return;
	}

	Test* test = tests[cur_test];

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = 28.0f;
	j2d.mGlyphHeight = 28.0f;
	const JUtility::TColor color(200, 255, 200, 255);
	j2d.mCharColor.set(color);
	j2d.mGradientColor.set(color);

	j2d.print(96.0f, 128.0f, "Running test:\n-> %s", test->get_name());
}

Test::Test(const char* name_)
{
	GZASSERTLINE(name_);
	name    = name_;
	cur_op  = 0;
	started = false;
}

bool Test::update()
{
	if (cur_op == 0 && !started) {
		OSReport("GZTest: Starting test\t%s\n", name);
		p2gz->controller->mPadReplay = p2gz->test_runner->gamepad;
		started                      = true;
	} else if (cur_op >= ops.len()) {
		return true;
	}

	p2gz->test_runner->gamepad->clear();
	const bool op_done = ops[cur_op]->execute();
	if (op_done) {
		cur_op += 1;
	}

	if (cur_op >= ops.len()) {
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
	LOG("Waiting 1 frame\n");
	frames -= 1;
	return false;
}

void CompoundOp::restart()
{
	cur_op = 0;
	for (size_t i = 0; i < ops.len(); i++) {
		ops[i]->restart();
	}
}

CompoundOp* CompoundOp::push(TestOp* op)
{
	ops.push(op);
	return this;
}

bool CompoundOp::execute()
{
	if (cur_op >= ops.len()) {
		return true;
	}

	if (ops[cur_op]->execute()) {
		cur_op += 1;
	}

	return false;
}

bool DoN::execute()
{
	if (n > 0) {
		LOG("DoN with n = %d/%d\n", n, start_n);
		if (other->execute()) {
			n -= 1;
			other->restart();
		}
		return false;
	}
	LOG("DoN %d done\n", start_n);
	return true;
}

bool DoUntil::execute()
{
	if (!is_in_state->invoke()) {
		LOG("DoUntil not in goal state, executing op\n");
		if (other->execute()) {
			other->restart();
		}
		return false;
	}
	LOG("DoUntil reached goal state\n");
	return true;
}

bool ButtonInput::execute()
{
	LOG("Pressing button %X\n", button);
	hold_frames -= 1;
	p2gz->test_runner->gamepad->status.button |= button;
	if (hold_frames <= 0) {
		return true;
	}
	return false;
}

bool WaitForState::execute()
{
	LOG("Waiting for state...\n");
	return is_in_state->invoke();
}
