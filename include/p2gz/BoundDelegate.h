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
