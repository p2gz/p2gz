#include <p2gz/p2gz.h>
#include <p2gz/FreeCam.h>
#include <p2gz/NaviTools.h>
#include <p2gz/timer.h>
#include <p2gz/WaypointViewer.h>
#include <p2gz/warp.h>
#include <Game/Navi.h>
#include <IDelegate.h>

using namespace gz;

P2GZ* p2gz;

P2GZ::P2GZ()
{
	collision_viewer             = new CollisionViewer();
	controller                   = new Controller(JUTGamePad::PORT_0);
	freecam                      = new FreeCam();
	menu                         = new GZMenu();
	navi_tools                   = new NaviTools();
	timer                        = new Timer();
	waypoint_viewer              = new WaypointViewer();
	warp                         = new Warp();
	skippable_treasure_cutscenes = new SkippableTreasureCS();
	imageMgr                     = new ImageMgr();
}

void P2GZ::init()
{
	// Menu must come first since other inits might change menu options
	menu->init_menu();

	warp->init();
	imageMgr->init();
}

void P2GZ::update()
{
	collision_viewer->update();
	freecam->update();
	waypoint_viewer->update();

	// Menu must update last so button presses for menu interactions don't
	// inadvertantly do things in other systems on the same frame they're pressed.
	menu->update();
}

void P2GZ::draw()
{
	timer->draw();
	menu->draw();
}
