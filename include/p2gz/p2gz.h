#ifndef _P2GZ_H
#define _P2GZ_H

#include <p2gz/gzmenu.h>
#include <p2gz/CollisionViewer.h>
#include <p2gz/FreeCam.h>
#include <p2gz/NaviTools.h>
#include <p2gz/timer.h>
#include <p2gz/WaypointViewer.h>
#include <Game/BaseGameSection.h>
#include <p2gz/warp.h>
#include <p2gz/SkippableCS.h>
// #include <p2gz/EnemyDebugInfo.h>

struct P2GZ {
public:
	P2GZ();
	~P2GZ() { }

	void init();
	void update();
	void draw();

	// our own persistent controller so we don't crash the game on new file starting (don't ask)
	Controller* controller;

	// gz specifics
	gz::GZMenu* menu;
	gz::CollisionViewer* collision_viewer;
	gz::FreeCam* freecam;
	gz::NaviTools* navi_tools;
	gz::Timer* timer;
	gz::WaypointViewer* waypoint_viewer;
	gz::Warp* warp;
	gz::SkippableTreasureCS* skippable_treasure_cutscenes;
	// gz::EnemyDebugInfo* enemy_debug_info;
};

// global instance
extern P2GZ* p2gz;

#endif
