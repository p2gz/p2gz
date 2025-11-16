#include <p2gz/TreasureDebugInfo.h>
#include <Game/Entities/PelletOtakara.h>
#include <Game/Entities/PelletItem.h>
#include <Game/Navi.h>
#include <P2JME/P2JME.h>

using namespace gz;

TreasureDebugInfo::TreasureDebugInfo()
{
	enabled = false;
}

void TreasureDebugInfo::draw()
{
	if (!enabled || !Game::PelletOtakara::mgr || !Game::PelletItem::mgr) {
		return;
	}

	Graphics* gfx = sys->mGfx;
	if (!gfx || !gfx->mCurrentViewport || !Game::naviMgr || !Game::naviMgr->getActiveNavi()) {
		return;
	}

	Iterator<Game::PelletOtakara::Object> treasureIterator(Game::PelletOtakara::mgr);
	CI_LOOP(treasureIterator)
	{
		Game::PelletOtakara::Object* treasure = *treasureIterator;
		draw_treasure_dbg(treasure, gfx);
	}

	Iterator<Game::PelletItem::Object> itemIterator(Game::PelletItem::mgr);
	CI_LOOP(itemIterator)
	{
		Game::PelletItem::Object* item = *itemIterator;
		draw_treasure_dbg(item, gfx);
	}
}

void TreasureDebugInfo::draw_treasure_dbg(Game::Pellet* pellet, Graphics* gfx)
{
	if (!pellet->isAlive()) {
		return;
	}

	// Setup text draw first
	gfx->initPerspPrintf(gfx->mCurrentViewport);

	PerspPrintfInfo info;
	info.mFont          = gP2JMEMgr->mFont;
	info.mScale         = 0.5f;
	info.mColorA        = Color4(255, 255, 255, 200);
	info.mColorB        = Color4(255, 255, 255, 200);
	Vector3f pellet_pos = pellet->getPosition();
	Vector3f pos        = pellet_pos + Vector3f(0, 45.0f, 0);

	Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(pellet->mGenerator->mObject);
	int treasure_id             = gen_pellet->mGenParm->mIndex;
	int kind                    = gen_pellet->mPelType;
	const char* treasure_name   = Game::PelletList::Mgr::mInstance->getConfig(kind)->getPelletConfig(treasure_id)->mParams.mName.mData;

	gfx->perspPrintf(info, pos, "%d: %s", treasure_id, treasure_name);
	info.mPerspectiveOffsetY += 22;
	gfx->perspPrintf(info, pos, "(%.2f, %.2f, %.2f)", pellet->getPosition().x, pellet->getPosition().y, pellet->getPosition().z);
	info.mPerspectiveOffsetY += 22;
	gfx->perspPrintf(info, pos, "carry power: %.1f", pellet->mCarryPower);
}
