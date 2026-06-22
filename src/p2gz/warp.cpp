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
#include <Game/generalEnemyMgr.h>
#include <Game/MapMgr.h>
#include <Dolphin/rand.h>
#include <TParticle2dMgr.h>
#include <Screen/Game2DMgr.h>

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
static const char* ENTER_KINDS[3] = {
	"from cave",
	"from map screen",
	"first enter from map",
};

Warp::Warp()
{
	allow_zero_pikmin_in_caves           = true;
	warping_from_menu                    = false;
	needs_post_load_action               = false;
	applying_generators                  = false;
	needs_generator_cache_reconstruction = false;
	do_egate_parse_only_load             = false;
	warping                              = false;
	already_saved_generators             = false;
	preset_status                        = PS_Stale;
	lockout_frames                       = 0;
	active_captain                       = NAVIID_Olimar;
	use_set_seed                         = false;
	preset_during_warp                   = nullptr;
}

void Warp::init()
{
	area_opt            = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/area"));
	sublevel_opt        = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/sublevel"));
	cave_opt            = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/cave"));
	day_opt             = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/day"));
	enter_area_type_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/enter method"));
	captain_opt         = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/captain"));
	seed_opt            = static_cast<HexInputOption*>(p2gz->menu->get_option("warp/seed"));
	preset_opt          = static_cast<PresetMenuOption*>(p2gz->menu->get_option("warp/preset"));

	for (size_t i = 0; i < 4; i++) {
		area_opt->options.push(AREA_NAMES[i]);
	}
	for (size_t i = 0; i < ARRAY_SIZE(ENTER_KINDS); i++) {
		enter_area_type_opt->options.push(ENTER_KINDS[i]);
	}
	captain_opt->options.push("olimar");
	captain_opt->options.push("louie");

	enter_area_type_opt->visible = false;

	day_opt->set_selection(dest.day + 1);

	update_cave_opt();
	update_sublevel_opt();
	update_captain_opt();
}

void Warp::sync()
{
	active_captain = Game::playData->mCaveSaveData.mActiveNaviID;
	captain_opt->set_selection(active_captain);
}

void Warp::set_preset(PresetPreview* preset, int preset_status_)
{
	next_preset_p = preset;
	next_preset   = nullptr;
	preset_status = static_cast<PresetStatus>(preset_status_);

	update_day_opt();
	update_enter_type_opt();
}

int Warp::next_preset_category()
{
	if (next_preset) {
		return next_preset->category;
	} else {
		return next_preset_p->category;
	}
}

void Warp::set_preset(Preset* preset, int preset_status_)
{
	next_preset = preset;
	preset->ref();
	next_preset_p = nullptr;
	preset_status = static_cast<PresetStatus>(preset_status_);

	update_day_opt();
	update_enter_type_opt();
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
	update_day_opt();
	update_enter_type_opt();
	update_captain_opt();
}

void Warp::set_warp_cave(size_t cave)
{
	dest.cave     = cave;
	dest.sublevel = 0;

	update_sublevel_opt();
	update_preset_opt();
	update_day_opt();
	update_enter_type_opt();
	update_captain_opt();
}

void Warp::set_warp_sublevel(s32 sublevel)
{
	GZASSERTLINE(sublevel >= 1);
	dest.sublevel = sublevel - 1; // Menu is 1-indexed for nicer visuals

	update_preset_opt();
}

void Warp::set_seed(u32 seed_)
{
	use_set_seed = true;
	seed         = seed_;
	seed_opt->set_selected_val(seed);
}

void Warp::set_random_seed()
{
	use_set_seed = false;
	seed_opt->set_unselected();
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
		sublevel_opt->set_selection(dest.sublevel + 1);
		sublevel_opt->set_bounds(1, NUM_FLOORS[dest.area][dest.cave - 1]);
	}
}

void Warp::update_preset_opt()
{
	if (preset_status > PS_Suggested) {
		return;
	}

	PresetCategory category = PoD;
	if (has_next_preset()) {
		category = static_cast<PresetCategory>(next_preset_category());
	}

	PresetPreview* suggested_preset = p2gz->preset_mgr->suggested_preset(dest, category);
	if (suggested_preset) {
		set_preset(suggested_preset, PS_Suggested);
		preset_opt->current_preview = suggested_preset;
	}
}

// only show day option when warping with no preset
void Warp::update_day_opt()
{
	if (day_opt) {
		if (!has_next_preset()) {
			day_opt->visible = true;
		} else {
			day_opt->visible = false;
		}
	}
}

// only show enter type option when warping AG with no preset
void Warp::update_enter_type_opt()
{
	if (enter_area_type_opt) {
		if (dest.cave == 0 && !has_next_preset()) {
			enter_area_type_opt->visible = true;
		} else {
			enter_area_type_opt->visible = false;
		}
	}
}

void Warp::update_captain_opt()
{
	captain_opt->visible = dest.cave > 0;
}

void Warp::do_warp()
{
	warping = true;

	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	p2gz->menu->close();

	if (has_next_preset()) {
		if (next_preset) {
			preset_during_warp = next_preset;
		} else {
			preset_during_warp = p2gz->preset_mgr->load_preset(next_preset_p);
		}

		GZASSERTLINE(preset_during_warp);
		preset_during_warp->apply();
		needs_post_load_action               = true;
		needs_generator_cache_reconstruction = true; // rebuild caches when we warp
	} else {
		dest.day = day_opt->get_selection() - 1;
	}

	if (particle2dMgr) {
		particle2dMgr->killAll();
	}

	reset_cave_treasure_collections(game);
	p2gz->poko_editor->apply_cave_pokos();
	game->disableTimer(Game::DEMOTIMER_None);

	if (dest.cave == 0) {
		warp_to_area(game);
	} else {
		warp_to_cave(game);
	}

	// Warping revives captains, but because it skips the normal load sequence, any dead captains
	// are still considered dead even after being revived. This informs naviMgr that they're both alive again.
	if (Game::naviMgr) {
		Game::naviMgr->clearDeadCount();
	}

	if (warping_from_menu) {
		p2gz->in_save_file = false;
	}

	already_saved_generators = false;

	// TODO: This is obviously a dumb hack. Once we have more trainers, this should be refactored and handled by a trainer manager.
	if (dest.area != 1 || dest.cave != 1 || dest.sublevel != 4) {
		p2gz->empress_trainer->stop();
	}
	
	if (!p2gz->early_blues_trainer->is_pending_setup()) {
		p2gz->early_blues_trainer->stop();
	}

	if (p2gz->collision_viewer->is_enabled()) {
		p2gz->collision_viewer->handle_warp();
	}
}

void Warp::reset_cave_treasure_collections(Game::SingleGameSection* game)
{
	Game::PelletCropMemory* mem        = Game::playData->getCaveCropMemory();
	Game::KindCounter& counter_otakara = mem->mOtakara;
	Game::BasePelletMgr* pelmgr        = Game::PelletOtakara::mgr;

	for (int i = 0; i < counter_otakara.getNumKinds(); i++) {
		Game::playData->losePellet(pelmgr, i);
		Game::playData->mCaveCropMemory->mOtakara(i) = 0;
		counter_otakara(i)                           = 0;
	}

	pelmgr                          = Game::PelletItem::mgr;
	Game::KindCounter& counter_item = mem->mItem;
	for (int i = 0; i < counter_item.getNumKinds(); i++) {
		Game::playData->losePellet(pelmgr, i);
		Game::playData->mCaveCropMemory->mItem(i) = 0;
		counter_item(i)                           = 0;
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
	if (!warping_from_menu) {
		save_pikmin();
	}

	// Look up destination cave ID from index
	Game::CourseInfo* dst_course_info = Game::stageList->getCourseInfo(dest.area);
	ID32 caveID(dst_course_info->getCaveID_FromIndex(dest.cave - 1));
	Game::playData->setSaveFlag(Game::STORYSAVE_Cave, nullptr);
	Game::playData->setCurrentCourse(dst_course_info->mCourseIndex);
	Game::playData->setCurrentCave(caveID, 0);
	Game::gameSystem->mTimeMgr->mDayCount        = dest.day; // set day
	Game::playData->mCaveSaveData.mTime          = Game::gameSystem->mTimeMgr->mCurrentTimeOfDay;
	Game::playData->mCaveSaveData.mCourseIdx     = dst_course_info->mCourseIndex;
	Game::playData->mCaveSaveData.mCurrentCaveID = caveID;

	// save changes to world state if we're above-ground AND are not warping to a preset
	// (preset will set the above-ground state itself)
	if (in_above_ground_play() && !already_saved_generators && !has_next_preset()) {
		game->saveToGeneratorCache(game->mCurrentCourseInfo);
	}

	game->mCurrentCourseInfo = dst_course_info;
	game->mCurrentCave       = nullptr;
	game->mCaveID.setID(caveID.getID());
	game->mCaveIndex    = caveID.getID();
	game->mCurrentFloor = dest.sublevel;
	game->mInCave       = true;
	strcpy(game->mCaveFilename, dst_course_info->getCaveinfoFilename_FromID(caveID));
	Game::playData->mCaveSaveData.mCurrentFloor = dest.sublevel;
	Game::playData->mCaveSaveData.mActiveNaviID = active_captain;

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

	Game::SingleGame::LoadArg arg(Game::SingleGame::MapEnter_CaveEnter, true, warping_from_menu, false);
	game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}

void Warp::warp_to_area(Game::SingleGameSection* game)
{
	if (!warping_from_menu) {
		save_pikmin();
	}

	// TODO: Probably not all of this is necessary - copy-paste from DayEndState::exec()
	Game::gameSystem->resetFlag(Game::GAMESYS_IsGameWorldActive);
	Game::gameSystem->setFlag(Game::GAMESYS_DisableDeathCounter);
	Game::moviePlayer->reset();
	Game::moviePlayer->clearSuspendedDemo();

	if (game->mTheExpHeap) {
		PSMCancelToPauseOffMainBgm();
	}

	if (Game::ItemOnyon::mgr) {
		Iterator<Game::Onyon> iOnyon(Game::ItemOnyon::mgr);
		CI_LOOP(iOnyon)
		{
			(*iOnyon)->setSpotEffectActive(false);
			(*iOnyon)->mSuckTimer = 4.0f;
			(*iOnyon)->forceClose();
		}
	}

	// save changes to world state if we're above-ground AND are not warping to a preset
	// (preset will set the above-ground state itself)
	if (in_above_ground_play() && !already_saved_generators && !has_next_preset()) {
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
	if (Game::naviMgr && Game::naviMgr->mArray) {
		for (int i = 0; i < 2; i++) {
			Game::Navi* navi = Game::naviMgr->getAt(i);
			if (navi && navi->isAlive()) {
				navi->mFsm->transit(navi, Game::NSID_Walk, nullptr);
				efx::TNaviEffect* effectsObj = navi->mEffectsObj;
				if (effectsObj) {
					effectsObj->mFlags.unset(efx::NAVIFX_InWater);
					effectsObj->killHamonA_();
					effectsObj->killHamonB_();
				}
			}
		}
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
	Game::SingleGame::LoadArg arg(map_enter_status, false, warping_from_menu, false);
	game->mFsm->transit(game, Game::SingleGame::SGS_Load, &arg);
}

void Warp::do_post_warp()
{
	warping = false;

	// Make sure all navi have max health
	if (Game::naviMgr && Game::naviMgr->mArray) {
		for (int i = 0; i < 2; i++) {
			Game::Navi* navi = Game::naviMgr->getAt(i);
			if (navi && navi->isAlive()) {
				navi->mHealth = 50.0f;
			}
		}
	}

	if (preset_during_warp) {
		if (needs_post_load_action) {
			preset_during_warp->apply_post_load();
			preset_status = PS_Stale;
		}
		preset_during_warp->del();
	}

	// null-out next_preset to avoid dangling
	// NB: we DON'T want to null-out next_preset_p though, otherwise warp menu will fall out of sync
	next_preset = nullptr;

	if (use_set_seed) {
		set_random_seed();
	}

	preset_during_warp     = nullptr;
	needs_post_load_action = false;
}
