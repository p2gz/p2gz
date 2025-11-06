#ifndef _GZ_TEST_RUNNER_H
#define _GZ_TEST_RUNNER_H

#include <Controller.h>
#include <p2gz/gzCollections.h>

namespace gz {
namespace test {

struct TestOp {
public:
	TestOp() { }
	TestOp(TestOp& other) { }

	/// Returns whether to continue on to the next op
	virtual bool execute() { return true; }
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
	Vec<Test*> tests;
	size_t cur_test;
	bool inited;
};

}; // namespace test
}; // namespace gz

#endif
