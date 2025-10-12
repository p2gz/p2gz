#ifndef _P2GZ_H
#define _P2GZ_H

#include <p2gz/gzmenu.h>
#include <Game/BaseGameSection.h>
#include <Graphics.h>

struct P2GZ {
public:
	P2GZ();
	~P2GZ() { }

	void update();
	void draw();

	// some demo functions for the menu
	void die_painfully(int navi_id);
	void boing(int navi_id);

	void teleport();

	gz::GZMenu* menu;
};

// global instance
extern P2GZ* p2gz;

#endif
