#ifndef _GZCOLLECTIONS_H
#define _GZCOLLECTIONS_H

#include <types.h>
#include <System.h>
#include <stl/mem.h>
#include <p2gz/gzMacros.h>
#include <JSystem/JKernel/JKRHeap.h>

namespace gz {

template <u32 N, typename T>
struct RingBuffer {
	RingBuffer()
	{
		GZASSERTLINE(N > 0);
		JKRHeap* prevHeap = sys->mSysHeap->becomeCurrentHeap();
		mBuf              = new T[N];
		prevHeap->becomeCurrentHeap();
		mBufHead = N;
		mLen     = 0;
	}

	~RingBuffer() { delete[] mBuf; }

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

	/// Gets the oldest entry (the slot the next push() overwrites). Only valid when atCapacity().
	T getLast()
	{
		GZASSERTLINE(mLen == N);
		// push() overwrites (mBufHead + 1) % N, which holds the oldest entry == peekN(mLen - 1).
		return peekN(mLen - 1);
	}

private:
	// Non-copyable since we own the mBuf allocation - a shallow copy would double-free on destruction
	RingBuffer(const RingBuffer&);
	RingBuffer& operator=(const RingBuffer&);

	size_t mLen;
	size_t mBufHead;
	T* mBuf;
};

template <typename T>
struct Vec {
	// doesn't allocate space until we need it
	Vec(size_t capacity = 0)
	{
		mCapacity = capacity;
		mLen      = 0;
		if (mCapacity > 0) {
			JKRHeap* prevHeap = sys->mSysHeap->becomeCurrentHeap();
			mBuf              = new T[capacity];
			prevHeap->becomeCurrentHeap();
		} else {
			mBuf = nullptr;
		}
	}

	~Vec()
	{
		if (mBuf)
			delete[] mBuf;
	}

	inline size_t len() const { return mLen; }

	inline size_t capacity() const { return mCapacity; }

	void push(T val)
	{
		if (mLen >= mCapacity) {
			_grow(mCapacity == 0 ? 8 : mCapacity * 2);
		}
		GZASSERTLINE(mBuf);
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
		// mBuf may be null for a never-pushed Vec
		return -1;
	}

	T removeAt(size_t idx)
	{
		GZASSERTLINE(mBuf);
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
		GZASSERTLINE(mBuf);
		GZASSERTLINE(mLen > 0);
		mLen--;
		return mBuf[mLen];
	}

	T& operator[](size_t idx)
	{
		GZASSERTLINE(mBuf);
		GZASSERTLINE(idx < mLen);
		return mBuf[idx];
	}

	const T& operator[](size_t idx) const
	{
		GZASSERTLINE(mBuf);
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
		JKRHeap* prevHeap = sys->mSysHeap->becomeCurrentHeap();
		T* newBuf         = new T[newCapacity];
		if (mBuf) {
			memmove(newBuf, mBuf, sizeof(T) * mLen);
			delete[] mBuf;
		}
		mBuf      = newBuf;
		mCapacity = newCapacity;
		prevHeap->becomeCurrentHeap();
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
		if (vec[i]) {                        \
			delete vec[i];                   \
			vec[i] = nullptr;                \
		}                                    \
	}                                        \
	vec.clear();

} // namespace gz

#endif
