#include <p2gz/p2gz.h>
#include <p2gz/warp.h>
#include <p2gz/gzMacros.h>
#include <p2gz/gzmenu.h>
#include <Game/BaseGameSection.h>
#include <Game/SingleGameSection.h>
#include <Game/Entities/PelletCarcass.h>
#include <Game/Entities/PelletFruit.h>
#include <Game/Entities/PelletItem.h>
#include <Game/Entities/PelletOtakara.h>
#include <Game/MoviePlayer.h>
#include <Game/NaviState.h>
#include <PSM/Global.h>
#include <Game/PikiMgr.h>
#include <Game/generalEnemyMgr.h>
#include <Game/SingleGame.h>
#include <PikiAi.h>
#include <Game/PikiState.h>
#include <Game/Entities/ItemCave.h>
#include <Game/generalEnemyMgr.h>
#include <Game/MapMgr.h>

using namespace gz;

static const char* AREA_NAMES[4] = {
	"Valley of Repose",
	"Awakening Wood",
	"Perplexing Pool",
	"Wistful Wild",
};
static const char* CAVE_NAMES[4][4] = {
	{ "Emergence Cave", "Subterranean Complex", "Frontier Cavern", "" },
	{ "Hole of Beasts", "White Flower Garden", "Bulblax Kingdom", "Snagret Hole" },
	{
	    "Citadel of Spiders",
	    "Glutton's Kitchen",
	    "Shower Room",
	    "Submerged Castle",
	},
	{ "Cavern of Chaos", "Hole of Heroes", "Dream Den", "" },
};
static const size_t NUM_FLOORS[4][4] = {
	{ 2, 9, 8, 0 },
	{ 5, 5, 7, 7 },
	{ 5, 6, 5, 7 },
	{ 10, 15, 14, 0 },
};

Warp::Warp()
    : warp_area(0)
    , warp_cave(0)
    , warp_sublevel(1)
    , warp_day(2)
{
}

void Warp::init()
{
	RadioMenuOption* area_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/area"));
	RangeMenuOption* sublevel_opt = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/sublevel"));
	RangeMenuOption* day_opt      = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/day"));
	for (size_t i = 0; i < 4; i++) {
		area_opt->options.push(AREA_NAMES[i]);
	}

	day_opt->set_selection(warp_day + 1);

	update_cave_opt();
	update_sublevel_opt();
}

void Warp::set_warp_area(size_t area)
{
	warp_area = area;
	warp_cave     = 0;
	warp_sublevel = 0;

	update_cave_opt();
	update_sublevel_opt();
}

void Warp::set_warp_cave(size_t cave)
{
	warp_cave = cave;
	warp_sublevel = 0;

	update_sublevel_opt();
}

void Warp::set_warp_sublevel(s32 sublevel)
{
	GZASSERTLINE(sublevel >= 1);
	warp_sublevel = sublevel - 1; // Menu is 1-indexed for nicer visuals
}

void Warp::update_cave_opt()
{
	GZASSERTLINE(warp_area < 4);
	RadioMenuOption* cave_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/cave"));
	cave_opt->set_selection(warp_cave);

	cave_opt->options.clear();
	cave_opt->options.push("Above Ground");
	for (size_t i = 0; i < 4; i++) {
		const char* cave_name = CAVE_NAMES[warp_area][i];
		if (cave_name && strlen(cave_name) > 0) {
			cave_opt->options.push(cave_name);
		}
	}
}

void Warp::update_sublevel_opt()
{
	GZASSERTLINE(warp_area < 4);
	GZASSERTLINE(warp_cave < 5); // 5th is AG
	RangeMenuOption* sublevel_opt = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/sublevel"));

	// If destination is above ground, hide sublevel option
	sublevel_opt->visible = warp_cave > 0;
	if (!sublevel_opt->visible) {
		return;
	}

	sublevel_opt->max = NUM_FLOORS[warp_area][warp_cave - 1];
	sublevel_opt->set_selection(warp_sublevel + 1);
}

void Warp::do_warp()
{
	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	p2gz->menu->close();
	if (warp_cave == 0) {
		warp_to_area(game);
	} else {
		warp_to_cave(game);
	}
}

void Warp::warp_to_cave(Game::SingleGameSection* game)
{
	// Save pikmin currently in squad so they come with us into the warp destination
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->isAlive() && piki->getCurrActionID() == PikiAI::ACT_Formation) {
			int state = piki->getStateID();
			if (state != Game::PIKISTATE_Flying && state != Game::PIKISTATE_HipDrop
			    && piki->mNavi
			    //    Check if we're in SmC
			    && (!(warp_area == 2 && warp_cave != 4) || piki->getKind() == Game::Blue)) {
				Game::playData->mCaveSaveData.mCavePikis(piki)++;
			}
		}
	}

	// Look up destination cave ID from index
	Game::CourseInfo* dst_course_info = Game::stageList->getCourseInfo(warp_area);
	ID32 caveID(dst_course_info->getCaveID_FromIndex(warp_cave - 1));
	Game::ItemCave::Item* cave = new Game::ItemCave::Item;
	cave->mCaveID              = caveID;
	cave->mCaveFilename        = dst_course_info->getCaveinfoFilename_FromID(caveID);

	Game::gameSystem->mTimeMgr->mDayCount        = warp_day; // set day
	Game::playData->mCaveSaveData.mTime          = Game::gameSystem->mTimeMgr->mCurrentTimeOfDay;
	Game::playData->mCaveSaveData.mCourseIdx     = dst_course_info->mCourseIndex;
	Game::playData->mCaveSaveData.mCurrentCaveID = caveID;

	// Save changes to world state if we're above-ground currently
	// TODO: do we want to do this? Should it be a setting?
	if (!Game::gameSystem->mIsInCave) {
		game->saveToGeneratorCache(game->mCurrentCourseInfo);
	}

	game->mCurrentCourseInfo = dst_course_info;
	game->mCurrentCave       = cave;
	game->mCaveID            = caveID;
	game->mCaveIndex         = caveID.getID();
	game->mCurrentFloor      = warp_sublevel;
	strcpy(game->mCaveFilename, cave->mCaveFilename);

	Game::SingleGame::LoadArg arg(100, true, false, false);
	game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}

void Warp::warp_to_area(Game::SingleGameSection* game)
{
	// TODO: Probably not all of this is necessary - copy-paste from DayEndState::exec()
	Game::gameSystem->resetFlag(Game::GAMESYS_IsGameWorldActive);
	Game::gameSystem->setFlag(Game::GAMESYS_DisableDeathCounter);
	Game::moviePlayer->reset();
	Game::moviePlayer->clearSuspendedDemo();

	if (game->mTheExpHeap != nullptr) {
		PSMCancelToPauseOffMainBgm();
	}

	Iterator<Game::Onyon> iOnyon(Game::ItemOnyon::mgr);
	CI_LOOP(iOnyon)
	{
		(*iOnyon)->setSpotEffectActive(false);
		(*iOnyon)->mSuckTimer = 4.0f;
		(*iOnyon)->forceClose();
	}

	// Save changes to world state if we're above-ground currently
	// TODO: do we want to do this? Should it be a setting?
	if (!Game::gameSystem->mIsInCave) {
		game->saveToGeneratorCache(game->mCurrentCourseInfo);
	}

	// Kill all pellets and free resources
	Game::PelletIterator iPellet;
	CI_LOOP(iPellet)
	{
		Game::Pellet* pellet = *iPellet;
		if (pellet->isAlive() && pellet->mCaptureMatrix == nullptr) {
			pellet->kill(nullptr);
		}
	}

	Game::PelletCarcass::mgr->resetMgr();
	Game::PelletFruit::mgr->resetMgr();
	Game::PelletItem::mgr->resetMgrAndResources();
	Game::PelletOtakara::mgr->resetMgrAndResources();

	// Clean up Navi resources
	Game::Navi* navi = Game::naviMgr->getAt(NAVIID_Olimar);
	if (navi->isAlive()) {
		navi->mFsm->transit(navi, Game::NSID_Walk, nullptr);
		efx::TNaviEffect* effectsObj = navi->mEffectsObj;
		effectsObj->mFlags.unset(efx::NAVIFX_InWater);
		effectsObj->killHamonA_();
		effectsObj->killHamonB_();
	}

	navi = Game::naviMgr->getAt(NAVIID_Louie);
	if (navi->isAlive()) {
		navi->mFsm->transit(navi, Game::NSID_Walk, nullptr);
		efx::TNaviEffect* effectsObj = navi->mEffectsObj;
		effectsObj->mFlags.unset(efx::NAVIFX_InWater);
		effectsObj->killHamonA_();
		effectsObj->killHamonB_();
	}

	Game::pikiMgr->forceEnterPikmins(false);
	Game::gameSystem->mTimeMgr->setStartTime(); // Restore time of day setting

	if (warp_day % 30 == 0) {
		for (int i = 0; i < 4; i++) {
			Game::playData->mLimitGen[i].mLoops.all_zero();
		}
	}

	// Look up and apply destination settings
	Game::gameSystem->mTimeMgr->mDayCount = warp_day;
	Game::gameSystem->detachObjectMgr(Game::generalEnemyMgr);
	Game::gameSystem->detachObjectMgr(Game::mapMgr);

	game->mIsGameStarted     = false;
	game->mCurrentCourseInfo = Game::stageList->getCourseInfo(warp_area);

	Game::SingleGame::LoadArg arg(0, false, false, false);
	game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}
