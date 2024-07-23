#ifndef _GZCOLLECTIONS_H
#define _GZCOLLECTIONS_H

#include "types.h"
#include "P2Macros.h"

template<u32 N, typename T>
struct RingBuffer {
    RingBuffer() {
        P2ASSERTLINE(10, N > 0);
        bufHead = 0;
    }

    void push(T val) {
        buf[bufHead] = val;
        bufHead = (bufHead + 1) % N;
    }

	/// @brief Removes value from the history and returns it
	/// @return The latest entry in the history
	T pop() {
        bufHead = (bufHead - 1) % N;
        return buf[bufHead];
    }

	/// @brief Returns a copy of the entry without removing it
	/// @return The latest entry in the history
	T* peek() {
        return &buf[(bufHead - 1) % N];
    }

private:
	int bufHead;
	T buf[N];
};

#endif