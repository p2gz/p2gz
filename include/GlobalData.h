#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"

struct P2GZ {
	P2GZ();

	~P2GZ() { }

    void init();

	bool mIsScrollingCamera; // controlling camera for warping
};

extern P2GZ* p2gz;

#endif
