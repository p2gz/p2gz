#include <p2gz/p2gz.h>
#include <p2gz/FreeCam.h>
#include <Game/Navi.h>
#include <IDelegate.h>

using namespace gz;

P2GZ* p2gz;

P2GZ::P2GZ()
{
	menu    = new GZMenu();
	freecam = new FreeCam();
}

void P2GZ::update()
{
	menu->update();
}

void P2GZ::draw()
{
	Graphics* gfx = sys->getGfx();
	if (!gfx || !gfx->mCurrentViewport)
		return;

	menu->draw(gfx);
}

void P2GZ::die_painfully(int navi_id)
{
	Game::NaviMgr* mgr = Game::naviMgr;
	Game::Navi* navi   = mgr->getAt(navi_id);
	if (navi) {
		navi->addDamage(9999.0, true /* this is the painful part */);
	}
}

void P2GZ::boing(int navi_id)
{
	Game::NaviMgr* mgr = Game::naviMgr;
	Game::Navi* navi   = mgr->getAt(navi_id);
	if (navi) {
		navi->mVelocity.add(Vector3f(0.0, 1000.0, 0.0));
	}
}
