#ifndef _GZCOLLECTIONS_H
#define _GZCOLLECTIONS_H

#include <types.h>
#include <stl/mem.h>
#include <p2gz/gzMacros.h>

namespace gz {

template <u32 N, typename T>
struct RingBuffer {
	RingBuffer()
	{
		GZASSERTLINE(N > 0);
		mBuf     = new T[N];
		mBufHead = N;
		mLen     = 0;
	}

	size_t len() { return mLen; }

	void push(T val)
	{
		mBufHead       = (mBufHead + 1) % N;
		mBuf[mBufHead] = val;
		if (mLen < N) {
			mLen++;
		}
	}

	/// @brief Returns a copy of the entry without removing it
	/// @return The latest entry in the history
	T peek()
	{
		GZASSERTLINE(mLen > 0);
		return mBuf[mBufHead];
	}

	T peekN(const size_t n)
	{
		GZASSERTLINE(mLen > 0);
		GZASSERTLINE(n < mLen);
		return mBuf[(mBufHead + N - n) % N];
	}

	bool atCapacity() { return mLen == N; }

	/// Gets the oldest thing in the ring buffer. Only save to call if `atCapacity()` returns true.
	T getLast()
	{
		GZASSERTLINE(mLen == N);
		// the oldest thing is the thing at the current head that will be overwritten
		// on the next call to push.
		return peekN(0);
	}

private:
	size_t mLen;
	size_t mBufHead;
	T* mBuf;
};

template <typename T>
struct Vec {
	Vec(size_t capacity = 8)
	{
		mCapacity = capacity;
		mLen      = 0;
		mBuf      = new T[capacity];
	}

	~Vec() { delete[] mBuf; }

	inline size_t len() { return mLen; }

	inline size_t capacity() { return mCapacity; }

	void push(T val)
	{
		if (mLen >= mCapacity) {
			_grow(mCapacity * 2);
		}
		mBuf[mLen] = val;
		mLen++;
	}

	int find(T val)
	{
		for (size_t i = 0; i < mLen; i++) {
			if (mBuf[i] == val) {
				return i;
			}
		}
		return -1;
	}

	T removeAt(size_t idx)
	{
		GZASSERTLINE(idx < mLen);
		T val = mBuf[idx];
		if (idx < mLen - 1) {
			memmove(&mBuf[idx], &mBuf[idx + 1], sizeof(T) * (mLen - idx - 1));
		}
		mLen--;
		return val;
	}

	T pop()
	{
		GZASSERTLINE(mLen > 0);
		mLen--;
		return mBuf[mLen];
	}

	T& operator[](size_t idx)
	{
		GZASSERTLINE(idx < mLen);
		return mBuf[idx];
	}

	void expandCapacityTo(size_t capacity)
	{
		if (capacity > mCapacity) {
			_grow(capacity);
		}
	}

	void extend(Vec<T>& other)
	{
		expandCapacityTo(len() + other.len());
		for (size_t i = 0; i < other.len(); i++) {
			push(other[i]);
		}
	}

	void clear() { mLen = 0; }

private:
	void _grow(size_t newCapacity)
	{
		T* newBuf = new T[newCapacity];
		memmove(newBuf, mBuf, sizeof(T) * mLen);
		delete[] mBuf;
		mBuf      = newBuf;
		mCapacity = newCapacity;
	}

	size_t mCapacity;
	size_t mLen;
	T* mBuf;
};

template <typename T>
static void copy_vec(Vec<T>& dst, Vec<T>& src)
{
	dst.clear();
	dst.expandCapacityTo(src.len());

	for (size_t i = 0; i < src.len(); i++) {
		dst.push(src[i]);
	}
}

#define FOREACH_VEC(vec) for (u32 i = 0; i < vec.len(); i++)

#define DELETE_ALL(vec)                      \
	for (size_t i = 0; i < vec.len(); i++) { \
		delete vec[i];                       \
	}                                        \
	vec.clear()

} // namespace gz

#endif
