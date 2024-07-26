#ifndef _GZCOLLECTIONS_H
#define _GZCOLLECTIONS_H

#include "types.h"
#include "GZMacros.h"

namespace gzCollections {

template<u32 N, typename T>
struct RingBuffer {
    RingBuffer() {
        GZASSERTLINE(N > 0);
        bufHead = 0;
        length = 0;
    }

    void push(T val) {
        buf[bufHead] = val;
        bufHead = (bufHead + 1) % N;
        if (length < N) length++;
    }

	/// @brief Returns a copy of the entry without removing it
	/// @return The latest entry in the history
	T* peek() {
        if (length == 0) return nullptr;
        return &buf[(bufHead - 1) % N];
    }

private:
    u32 length;
	u32 bufHead;
	T buf[N];
};

template<typename T>
struct Vec {
    Vec(size_t capacity = 8) {
        mCapacity = capacity;
        mLen = 0;
        mBuf = new T[capacity];
    }

    size_t len() {
        return mLen;
    }

    size_t capacity() {
        return mCapacity;
    }

    void push(T val) {
        if (mLen >= mCapacity) {
            _grow(mCapacity * 2);
        }
        mBuf[mLen] = val;
        mLen++;
    }

    int find(T val) {
        for (size_t i = 0; i < mLen; i++) {
            if (mBuf[i] == val) {
                return i;
            }
        }
        return -1;
    }

    T removeAt(size_t idx) {
        GZASSERTLINE(idx < mLen);
        if (idx == mLen - 1) { return pop(); }
        T val = mBuf[idx];
        memcpy(mBuf[mBuf[idx], mBuf[idx+1], mLen - idx - 1]);
        mLen--;
        return val;
    }

    T pop() {
        GZASSERTLINE(mLen > 0);
        mLen--;
        return mBuf[mLen];
    }

    T& operator[](size_t idx) {
        GZASSERTLINE(idx < mLen);
        return mBuf[idx];
    }

    void expandCapacityTo(size_t capacity) {
        if (capacity > mCapacity) {
            _grow(capacity);
        }
    }

    void extend(Vec<T>& other) {
        expandCapacityTo(len() + other.len());
        for (size_t i = 0; i < other.len(); i++) {
            push(other[i]);
        }
    }

    void clear() {
        mLen = 0;
    }

private:
    void _grow(size_t newCapacity) {
        size_t oldCapacity = mCapacity;
        mCapacity *= newCapacity;
        T* newBuf = new T[mCapacity];
        memcpy(newBuf, mBuf, oldCapacity);
        delete mBuf;
        mBuf = newBuf;
    }

    size_t mCapacity;
    size_t mLen;
    T* mBuf;
};

} // namespace gzCollections

#endif
