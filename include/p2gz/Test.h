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
#define TEST(name, ...) new Test(name, ARRAY_SIZE(((TestOp*[]) { __VA_ARGS__ })), ((TestOp*[]) { __VA_ARGS__ }))

#define PRESS(btn)     new ButtonInput(btn), new Wait(1)
#define DBL_DPAD_L     PRESS(PAD_BUTTON_LEFT), PRESS(PAD_BUTTON_LEFT)
#define DO_N(num, ...) new DoN(new CompoundOp(ARRAY_SIZE(((TestOp*[]) { __VA_ARGS__ })), ((TestOp*[]) { __VA_ARGS__ })), num)

#define WAIT_FOR(state_fn) new WaitForState(new FreeDelegateR<bool>(&state_fn))

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
	CompoundOp(size_t num_, TestOp** ops_);

	virtual bool execute();
	virtual void restart()
	{
		cur_op = 0;
		for (size_t i = 0; i < num; i++) {
			ops[i]->restart();
		}
	}

	Vec<TestOp*> ops;
	size_t num;

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
	virtual void restart() { n = start_n; }

	TestOp* other;
	size_t n;
	const size_t start_n;
};

struct ButtonInput : public TestOp {
public:
	ButtonInput(int button_, int hold_frames_ = 1)
	{
		button      = button_;
		hold_frames = hold_frames_;
	}

	virtual bool execute();

	int button;
	int hold_frames;
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
	Test(const char* name_, const size_t num_ops_, TestOp** ops_);

	/// Returns whether the test is finished
	bool update();

private:
	const char* name;
	const size_t num_ops;
	size_t cur_op;
	Vec<TestOp*> ops;
	bool started;
};

struct TestRunner {
public:
	TestRunner();

	void init();
	void update();

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
