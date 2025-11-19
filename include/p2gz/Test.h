#ifndef _GZ_TEST_RUNNER_H
#define _GZ_TEST_RUNNER_H

#include <Controller.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/pad.h>
#include <IDelegate.h>
#include <p2gz/BoundDelegate.h>

namespace gz {
namespace test {

// Creates a Test object with the right array sizes
#define TEST(name, ...) ((new Test(name))__VA_ARGS__ WAIT(30))
#define __TEST_OP(op)   ->push(op)

#define __COMPOUND(...)        (new CompoundOp()) __VA_ARGS__
#define PRESS(btn)             __TEST_OP(new ButtonInput(btn)) __TEST_OP(new Wait(1))
#define DBL_DPAD_L             PRESS(PAD_BUTTON_LEFT) PRESS(PAD_BUTTON_LEFT)
#define MASH_TEXT              PRESS(PAD_BUTTON_A) PRESS(PAD_BUTTON_B)
#define SKIP_CUTSCENE          __TEST_OP(new ActionOp(new FreeDelegate(&skip_movie)))
#define SKIP_LOAD_CUTSCENES    DO_UNTIL(SKIP_CUTSCENE, in_gameplay)

#define DO_ACTION(delegate)    __TEST_OP(new ActionOp(delegate))
#define DO_N(num, ...)         __TEST_OP(new DoN(__COMPOUND(__VA_ARGS__), num))
#define DO_UNTIL(op, state_fn) __TEST_OP(new DoUntil(__COMPOUND(op), new FreeDelegateR<bool>(state_fn)))

#define WAIT(frames)       __TEST_OP(new Wait(frames))
#define WAIT_FOR(state_fn) __TEST_OP(new WaitForState(new FreeDelegateR<bool>(&state_fn)))

struct TestOp {
public:
	TestOp() { }
	TestOp(TestOp& other) { }

	/// Returns whether to continue on to the next op
	virtual bool execute() { return true; }
	virtual void restart() { }
};

struct CompoundOp : public TestOp {
public:
	CompoundOp() { cur_op = 0; }

	virtual bool execute();
	virtual void restart();

	CompoundOp* push(TestOp* op);

	Vec<TestOp*> ops;

private:
	size_t cur_op;
};

struct NoOp : public TestOp {
public:
	NoOp() { }

	virtual bool execute() { return true; }
};

struct DoN : public TestOp {
public:
	DoN(TestOp* other_, size_t n_)
	    : start_n(n_)
	{
		GZASSERTLINE(other_);
		other = other_;
		n     = n_;
	}

	virtual bool execute();
	virtual void restart()
	{
		n = start_n;
		other->restart();
	}

	TestOp* other;
	size_t n;
	const size_t start_n;
};

struct DoUntil : public TestOp {
public:
	DoUntil(TestOp* other_, IDelegateR<bool>* is_in_state_)
	{
		GZASSERTLINE(other_);
		GZASSERTLINE(is_in_state_);
		other       = other_;
		is_in_state = is_in_state_;
	}

	virtual bool execute();
	virtual void restart() { other->restart(); }

	TestOp* other;
	IDelegateR<bool>* is_in_state;
};

struct ButtonInput : public TestOp {
public:
	ButtonInput(int button_, int hold_frames_ = 1)
	    : max_frames(hold_frames_)
	{
		button      = button_;
		hold_frames = max_frames;
	}

	virtual bool execute();
	virtual void restart() { hold_frames = max_frames; }

	int button;
	int hold_frames;
	const int max_frames;
};

struct ActionOp : public TestOp {
public:
	ActionOp(IDelegate* action_) { action = action_; }

	virtual bool execute()
	{
		action->invoke();
		return true;
	}

	IDelegate* action;
};

struct Wait : public TestOp {
public:
	Wait(size_t frames_) { frames = frames_; }

	virtual bool execute();

	size_t frames;
};

struct WaitForState : public TestOp {
public:
	WaitForState(IDelegateR<bool>* is_in_state_) { is_in_state = is_in_state_; }

	virtual bool execute();

	IDelegateR<bool>* is_in_state;
};

struct Test {
public:
	Test(const char* name_);

	/// Returns whether the test is finished
	bool update();
	Test* push(TestOp* op)
	{
		ops.push(op);
		return this;
	}

	inline const char* get_name() { return name; }

private:
	const char* name;
	size_t cur_op;
	Vec<TestOp*> ops;
	bool started;
};

struct TestRunner {
public:
	TestRunner();

	void init();
	void update();
	void draw_2d();

	JUTGamePadRecordFixed* gamepad;

private:
	void create_all_tests();

	Vec<Test*> tests;
	size_t cur_test;
	bool inited;
};

}; // namespace test
}; // namespace gz

#endif
