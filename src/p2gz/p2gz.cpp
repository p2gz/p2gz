#include <p2gz/p2gz.h>
#include <p2gz/FreeCam.h>
#include <p2gz/NaviTools.h>
#include <Game/Navi.h>
#include <IDelegate.h>

using namespace gz;

P2GZ* p2gz;

P2GZ::P2GZ()
{
	menu       = new GZMenu();
	freecam    = new FreeCam();
	navi_tools = new NaviTools();
}

void P2GZ::update()
{
	freecam->update();

	// Menu must update last so button presses for menu interactions don't
	// inadvertantly do things in other systems on the same frame they're pressed.
	menu->update();
}

void P2GZ::draw()
{
	Graphics* gfx = sys->getGfx();
	if (!gfx || !gfx->mCurrentViewport)
		return;

	menu->draw(gfx);
}
