#ifndef _GZCOLLECTIONS_H
#define _GZCOLLECTIONS_H

#include "types.h"
#include "P2Macros.h"

template<u32 N, typename T>
struct RingBuffer {
    RingBuffer() {
        P2ASSERTLINE(10, N > 0);
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

#endif