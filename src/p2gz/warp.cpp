#include <p2gz/p2gz.h>
#include <p2gz/warp.h>
#include <p2gz/gzMacros.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
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
#include <Dolphin/rand.h>

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
	{ 5, 6, 7, 5 },
	{ 10, 15, 14, 0 },
};
static const char* ENTER_KINDS[2] = {
	"from cave",
	"from map screen",
};

Warp::Warp()
{
	allow_zero_pikmin_in_caves = true;
	preset_status              = PS_Stale;
	cave                       = nullptr;
}

void Warp::init()
{
	area_opt            = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/area"));
	sublevel_opt        = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/sublevel"));
	cave_opt            = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/cave"));
	day_opt             = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/day"));
	enter_area_type_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/enter method"));
	seed_opt            = static_cast<HexInputOption*>(p2gz->menu->get_option("warp/seed"));
	preset_opt          = static_cast<PresetMenuOption*>(p2gz->menu->get_option("warp/preset"));

	for (size_t i = 0; i < 4; i++) {
		area_opt->options.push(AREA_NAMES[i]);
	}
	for (size_t i = 0; i < 2; i++) {
		enter_area_type_opt->options.push(ENTER_KINDS[i]);
	}

	day_opt->set_selection(dest.day + 1);

	update_cave_opt();
	update_sublevel_opt();
}

void Warp::set_preset(Preset* preset_, int preset_status_)
{
	preset        = preset_;
	preset_status = static_cast<PresetStatus>(preset_status_);
	if (preset_opt) {
		preset_opt->current_preset = preset;
	}
}

WarpDestination Warp::current_dest()
{
	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	ID32 cave_id(game->getCaveID());
	WarpDestination dest;

	dest.area         = game->mCurrentCourseInfo->mCourseIndex;
	dest.cave         = game->mCurrentCourseInfo->getCaveIndex_FromID(cave_id) + 1;
	dest.sublevel     = game->mCurrentFloor;
	dest.day          = Game::gameSystem->mTimeMgr->mDayCount;
	dest.use_set_seed = false;

	return dest;
}

void Warp::set_dest(WarpDestination new_dest)
{
	dest = new_dest;
}

void Warp::set_warp_area(size_t area)
{
	dest.area     = area;
	dest.cave     = 0;
	dest.sublevel = 0;

	update_cave_opt();
	update_sublevel_opt();
	update_preset_opt();
}

void Warp::set_warp_cave(size_t cave)
{
	dest.cave     = cave;
	dest.sublevel = 0;

	update_sublevel_opt();
	update_preset_opt();

	enter_area_type_opt->visible = dest.cave == 0;
}

void Warp::set_warp_sublevel(s32 sublevel)
{
	GZASSERTLINE(sublevel >= 1);
	dest.sublevel = sublevel - 1; // Menu is 1-indexed for nicer visuals

	update_preset_opt();
}

void Warp::set_seed(u32 seed)
{
	dest.use_set_seed = true;
	dest.seed         = seed;
	seed_opt->set_selected_val(seed);
}

void Warp::update_cave_opt()
{
	GZASSERTLINE(dest.area < 4);
	cave_opt->set_selection(dest.cave);

	cave_opt->options.clear();
	cave_opt->options.push("Above Ground");
	for (size_t i = 0; i < 4; i++) {
		const char* cave_name = CAVE_NAMES[dest.area][i];
		if (cave_name && strlen(cave_name) > 0) {
			cave_opt->options.push(cave_name);
		}
	}
}

void Warp::update_sublevel_opt()
{
	GZASSERTLINE(dest.area < 4);
	GZASSERTLINE(dest.cave < 5); // 0th is AG

	// If destination is above ground, hide cave-related options
	bool selection_is_cave = dest.cave > 0;
	sublevel_opt->visible  = selection_is_cave;
	seed_opt->visible      = selection_is_cave;

	if (selection_is_cave) {
		sublevel_opt->max = NUM_FLOORS[dest.area][dest.cave - 1];
		sublevel_opt->set_selection(dest.sublevel + 1);
	}
}

void Warp::update_preset_opt()
{
	if (preset_status > PS_Suggested) {
		return;
	}

	PresetCategory category = PoD;
	if (preset) {
		category = preset->category;
	}

	Preset* suggested_preset = p2gz->preset_mgr->suggested_preset(dest, category);
	if (suggested_preset) {
		set_preset(suggested_preset, PS_Suggested);
	}
}

void Warp::do_warp()
{
	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	p2gz->menu->close();

	if (preset) {
		preset->apply();
		preset_status = PS_Stale;
	}

	reset_cave_treasure_collections(game);
	if (dest.cave == 0) {
		warp_to_area(game);
	} else {
		warp_to_cave(game);
	}
}

void Warp::reset_cave_treasure_collections(Game::SingleGameSection* game)
{
	Game::PelletCropMemory* mem        = Game::playData->getCaveCropMemory();
	Game::KindCounter& counter_otakara = mem->mOtakara;
	Game::BasePelletMgr* pelmgr        = Game::PelletOtakara::mgr;

	for (int i = 0; i < counter_otakara.getNumKinds(); i++) {
		Game::playData->losePellet(pelmgr, i);
		counter_otakara(i) = 0;
	}

	pelmgr                          = Game::PelletItem::mgr;
	Game::KindCounter& counter_item = mem->mItem;
	for (int i = 0; i < counter_item.getNumKinds(); i++) {
		Game::playData->losePellet(pelmgr, i);
		counter_item(i) = 0;
	}
}

void Warp::save_pikmin()
{
	// clear cave piki container so we don't double up
	Game::playData->mCaveSaveData.mCavePikis.clear();

	// Save pikmin currently in squad so they come with us into the warp destination
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		// kill all bulbmin to keep pikmin counts correct
		if (piki->getKind() == Game::Bulbmin) {
			Game::PikiKillArg killArg(Game::CKILL_DontCountAsDeath);
			piki->kill(&killArg);
		}
		//                      vvvvvvvvvvvvvvvv <- make sure we don't bring wild pikmin
		if (piki->isAlive() && !piki->isZikatu() && piki->isPikmin()) {
			// Don't bring non-blues into SmC
			if (!(dest.area == 2 && dest.cave == 4) || piki->getKind() == Game::Blue) {
				Game::playData->mCaveSaveData.mCavePikis(piki)++;
				Game::PikiKillArg arg(Game::CKILL_DontCountAsDeath);
				piki->kill(&arg);
			}
		}
	}
}

void Warp::warp_to_cave(Game::SingleGameSection* game)
{
	save_pikmin();

	// Look up destination cave ID from index
	Game::CourseInfo* dst_course_info = Game::stageList->getCourseInfo(dest.area);
	ID32 caveID(dst_course_info->getCaveID_FromIndex(dest.cave - 1));
	if (!cave) {
		cave = new Game::ItemCave::Item;
	}
	cave->mCaveID       = caveID;
	cave->mCaveFilename = dst_course_info->getCaveinfoFilename_FromID(caveID);

	Game::gameSystem->mTimeMgr->mDayCount        = dest.day; // set day
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
	game->mCurrentFloor      = dest.sublevel;
	strcpy(game->mCaveFilename, cave->mCaveFilename);

	// adjust timer to account for saving + enable sub timer
	// usually we'd only reset the sub timer between sublevels, but on warp we reset both
	// NB: this means that retrying the sublevel will reset the main timer - that's probably okay for now?
	// TODO: record timer at sublevel start and, if retrying level, reset main timer to that value instead
	p2gz->timer->set_sub_timer_enabled(true);
	p2gz->timer->reset_main_timer();
	if (game->mCurrentFloor == 0) {
		p2gz->timer->offset_main_timer(CAVE_ENTER_SAVE_OFFSET_TIME);
	} else {
		p2gz->timer->offset_main_timer(NEXT_SUBLEVEL_SAVE_OFFSET_TIME);
	}

	Game::SingleGame::LoadArg arg(Game::SingleGame::MapEnter_CaveEnter, true, false, false);
	game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}

void Warp::warp_to_area(Game::SingleGameSection* game)
{
	save_pikmin();

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

	if (dest.day % 30 == 0) {
		for (int i = 0; i < 4; i++) {
			Game::playData->mLimitGen[i].mLoops.all_zero();
		}
	}

	// Look up and apply destination settings
	Game::gameSystem->mTimeMgr->mDayCount = dest.day;
	Game::gameSystem->detachObjectMgr(Game::generalEnemyMgr);
	Game::gameSystem->detachObjectMgr(Game::mapMgr);

	game->mIsGameStarted     = false;
	game->mCurrentCourseInfo = Game::stageList->getCourseInfo(dest.area);

	int map_enter_status;
	switch (dest.enter_area_type) {
	case 0:
		map_enter_status = Game::SingleGame::MapEnter_CaveGeyser;
		break;
	case 1:
	default:
		map_enter_status = Game::SingleGame::MapEnter_NewDay;
		// set flag so timer resets on load-in
		p2gz->timer->set_FS_map_flag(true);
		break;
	}
	Game::SingleGame::LoadArg arg(map_enter_status, false, false, false);
	game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}
