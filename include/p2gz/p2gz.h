#ifndef _P2GZ_H
#define _P2GZ_H

#include <p2gz/gzmenu.h>
#include <p2gz/FreeCam.h>
#include <p2gz/NaviTools.h>
#include <p2gz/timer.h>
#include <Game/BaseGameSection.h>

struct P2GZ {
public:
	P2GZ();
	~P2GZ() { }

	void update();
	void draw();

	gz::GZMenu* menu;
	gz::FreeCam* freecam;
	gz::NaviTools* navi_tools;
	gz::Timer* timer;
};

// global instance
extern P2GZ* p2gz;

#endif
