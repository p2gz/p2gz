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
