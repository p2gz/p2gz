#ifndef _GZ_BOUND_DELEGATE_H
#define _GZ_BOUND_DELEGATE_H

#include <IDelegate.h>

/// A Delegate1 that has a pre-bound argument for its function
template <typename T, typename A>
struct BoundDelegate1 : public IDelegate {
	inline BoundDelegate1(T* obj_, void (T::*func_)(A), A arg_)
	{
		obj  = obj_;
		func = func_;
		arg  = arg_;
	}

	virtual void invoke() { (obj->*func)(arg); }

	T* obj;
	A arg;
	void (T::*func)(A);
};

template <typename T, typename A, typename B>
struct BoundDelegate2 : public IDelegate {
	inline BoundDelegate2(T* obj_, void (T::*func_)(A, B), A arg1_, B arg2_)
	{
		obj  = obj_;
		func = func_;
		arg1 = arg1_;
		arg2 = arg2_;
	}

	virtual void invoke() { (obj->*func)(arg1, arg2); }

	T* obj;
	A arg1;
	B arg2;
	void (T::*func)(A, B);
};

/// A Delegate1 for a two-argument function where one argument is pre-applied
template <typename T, typename A, typename B>
struct CurriedDelegate1 : public IDelegate1<B> {
	inline CurriedDelegate1(T* obj_, void (T::*func_)(A, B), A arg1_)
	{
		obj  = obj_;
		func = func_;
		arg1 = arg1_;
	}

	virtual void invoke(B arg2) { (obj->*func)(arg1, arg2); }

	T* obj;
	A arg1;
	void (T::*func)(A, B);
};

/// A Delegate1 for a three-argument function where two arguments are pre-applied
template <typename T, typename A, typename B, typename C>
struct CurriedDelegate2 : public IDelegate1<C> {
	inline CurriedDelegate2(T* obj_, void (T::*func_)(A, B, C), A arg1_, B arg2_)
	{
		obj  = obj_;
		func = func_;
		arg1 = arg1_;
		arg2 = arg2_;
	}

	virtual void invoke(C arg3) { (obj->*func)(arg1, arg2, arg3); }

	T* obj;
	A arg1;
	B arg2;
	void (T::*func)(A, B, C);
};

struct FreeDelegate : public IDelegate {
	inline FreeDelegate(void (*func_)()) { func = func_; }

	virtual void invoke() { (func)(); }

	void (*func)();
};

template <typename R>
struct IDelegateR {
	virtual R invoke() = 0; // _08
};

template <typename A>
struct FreeBoundDelegate1 : public IDelegate {
	inline FreeBoundDelegate1(void (*func_)(A), A arg1_)
	{
		func = func_;
		arg1 = arg1_;
	}

	virtual void invoke() { (func)(arg1); }

	A arg1;
	void (*func)(A);
};

template <typename A, typename B>
struct FreeBoundDelegate2 : public IDelegate {
	inline FreeBoundDelegate2(void (*func_)(A, B), A arg1_, B arg2_)
	{
		func = func_;
		arg1 = arg1_;
		arg2 = arg2_;
	}

	virtual void invoke() { (func)(arg1, arg2); }

	A arg1;
	B arg2;
	void (*func)(A, B);
};

template <typename A, typename B, typename C>
struct FreeBoundDelegate3 : public IDelegate {
	inline FreeBoundDelegate3(void (*func_)(A, B, C), A arg1_, B arg2_, C arg3_)
	{
		func = func_;
		arg1 = arg1_;
		arg2 = arg2_;
		arg3 = arg3_;
	}

	virtual void invoke() { (func)(arg1, arg2, arg3); }

	A arg1;
	B arg2;
	C arg3;
	void (*func)(A, B, C);
};

/// A Delegate that can return an argument
template <typename R>
struct FreeDelegateR : public IDelegateR<R> {
	inline FreeDelegateR(R (*func_)()) { func = func_; }

	virtual R invoke() { return (func)(); }

	R (*func)();
};

#endif