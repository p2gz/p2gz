#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/CutsceneToggle.h>
#include <p2gz/p2gz.h>
#include <Game/gamePlayData.h>

using namespace gz;

static CutsceneMap CUTSCENE_NAME_MAP[] = {
	// Tutorials and "First X" cutscenes
	CutsceneMap(Game::DEMO_Unlock_Captain_Switch, "unlock captain switching (kill bulborb)", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Use_Louie, "first switch to louie", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Pluck_First_Pikmin, "pluck first pikmin", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Number_Pellet, "first pellet to onion", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Reunite_Captains, "reunite captains (crush bag)", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_You_Appear_Lost, "you appear lost", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Discover_Treasure, "discover first treasure", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Gate_Down, "first gate down", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Globe_Day_End, "first globe day end", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Nectar_Use, "first nectar flowering", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Sunset_Warning, "first sunset warning", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Whites_Digging, "white pikmin digging", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Spiderwort_Mold, "find mold", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_President_Start, "start as president", TUTORIAL_MENU_TITLE),

	// Area enters + crash landing
	CutsceneMap(Game::DEMO_Day_One_Start, "crash landing", AREA_ENTER_MENU_TITLE),
	CutsceneMap(Game::DEMO_Enter_Awakening_Wood, "enter awakening wood", AREA_ENTER_MENU_TITLE),
	CutsceneMap(Game::DEMO_Enter_Perplexing_Pool, "enter perplexing pool", AREA_ENTER_MENU_TITLE),
	CutsceneMap(Game::DEMO_Enter_Wistful_Wild, "enter wistful wild", AREA_ENTER_MENU_TITLE),

	// Cave-related cutscenes
	CutsceneMap(Game::DEMO_First_Cave_Enter, "first cave enter", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Corpse_In_Cave, "first bug body in cave", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Cave_Deeper_Hole, "find cave deeper hole", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Cave_Geyser, "find cave geyser", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Cave_Return, "first cave return", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_Waterwraith_Appears, "waterwraith appears", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Titan_Dweevil, "find titan dweevil", CAVE_RELATED_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Loozy_Treasure, "find louie (treasure)", CAVE_RELATED_MENU_TITLE),

	// Pikmin, Candypop and Onion color unlocks
	CutsceneMap(Game::DEMO_Louie_Finds_Red_Onion, "louie find red onion (louie skip)", PIKMIN_COLOR_MENU_TITLE),
	CutsceneMap(Game::DEMO_Meet_Red_Pikmin, "meet red pikmin", PIKMIN_COLOR_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Yellow_Onion, "find yellow onion", PIKMIN_COLOR_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Blue_Onion, "find blue onion", PIKMIN_COLOR_MENU_TITLE),
	CutsceneMap(Game::DEMO_Purple_Candypop, "discover purple candypop", PIKMIN_COLOR_MENU_TITLE),
	CutsceneMap(Game::DEMO_White_Candypop, "discover white candypop", PIKMIN_COLOR_MENU_TITLE),
	CutsceneMap(Game::DEMO_Discover_Bulbmin, "discover bulbmin", PIKMIN_COLOR_MENU_TITLE),

	// Pikmin count- and grouping-related cutscenes
	CutsceneMap(Game::DEMO_Reds_In_Onion, "reds in onion", PIKMIN_GROUP_MENU_TITLE),
	CutsceneMap(Game::DEMO_Purples_In_Ship, "purples in ship", PIKMIN_GROUP_MENU_TITLE),
	CutsceneMap(Game::DEMO_Reds_Purples_Tutorial, "reds and purples together", PIKMIN_GROUP_MENU_TITLE),
	CutsceneMap(Game::DEMO_Whites_In_Ship, "whites in ship", PIKMIN_GROUP_MENU_TITLE),
	CutsceneMap(Game::DEMO_Max_Pikmin_On_Field, "max pikmin on field", PIKMIN_GROUP_MENU_TITLE),

	// Hazard-related cutscenes
	CutsceneMap(Game::DEMO_Pikmin_In_Danger_Fire, "hazard: on fire", HAZARD_MENU_TITLE),
	CutsceneMap(Game::DEMO_Pikmin_In_Danger_Water, "hazard: bubbled-drowning", HAZARD_MENU_TITLE),
	CutsceneMap(Game::DEMO_Pikmin_In_Danger_Poison, "hazard: poisoned", HAZARD_MENU_TITLE),
	CutsceneMap(Game::DEMO_Eat_White_Pikmin, "first white eaten", HAZARD_MENU_TITLE),

	// Spicy and bitter spray cutscenes
	CutsceneMap(Game::DEMO_First_Spicy_Berry, "first spicy berry collected", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Spicy_Spray_Made, "first spicy spray made", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Spicy_Drop, "find spicy drop", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Spicy_Use, "first spicy use", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Bitter_Berry, "first bitter berry collected", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Bitter_Spray_Made, "first bitter spray made", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Bitter_Drop, "find bitter drop", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Bitter_Use, "first bitter use", SPRAYS_MENU_TITLE),
};

static const size_t NUM_CUTSCENES = ARRAY_SIZE(CUTSCENE_NAME_MAP);

CutsceneMgr::CutsceneMgr()
{
	// need to initialise these before gzmenu gets initialised
	for (size_t i = 0; i < NUM_CUTSCENES; i++) {
		Game::DemoFlags id = CUTSCENE_NAME_MAP[i].idx;
		cutscene_list.push(new CutsceneToggle(id));
	}
}

void CutsceneMgr::init()
{
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		CutsceneToggle* toggle = cutscene_list[i];
		Game::DemoFlags id     = toggle->get_cutscene_id();
		char option_path[PATH_MAX];
		sprintf(option_path, "cutscenes/%s/%s", CutsceneMap::get_menu_from_idx(id), CutsceneMap::get_name_from_idx(id));
		toggle->init(static_cast<CutsceneMenuOption*>(p2gz->menu->get_option(option_path)));
	}
}

void CutsceneMgr::update()
{
	// don't update if gameplay data isn't loaded yet
	if (!Game::playData) {
		return;
	}

	// only update if p2gz menu is open
	if (!p2gz->menu->is_open()) {
		return;
	}

	// only update if we don't have a cutscene submenu open
	if (p2gz->menu->is_within_menu("cutscenes")) {
		return;
	}

	// sync all cutscene flags from playData
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		Game::DemoFlags id = cutscene_list[i]->get_cutscene_id();
		cutscene_list[i]->get_option()->set_selection(Game::playData->isDemoFlag(id));
	}
}

CutsceneToggle* CutsceneMgr::get_toggle(Game::DemoFlags id)
{
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		if (cutscene_list[i]->get_cutscene_id() == id) {
			return cutscene_list[i];
		}
	}
	return nullptr;
}

void CutsceneToggle::set_cutscene_flag(bool played)
{
	// force selection to playData
	if (played) {
		// game now thinks cutscene has been played
		Game::playData->setDemoFlag(cutscene_id);
	} else {
		// game now thinks cutscene has NOT been played
		Game::playData->mDemoFlags.resetFlag(cutscene_id);
	}
}

const char* CutsceneMap::get_name_from_idx(Game::DemoFlags idx_)
{
	for (size_t i = 0; i < NUM_CUTSCENES; i++) {
		CutsceneMap& map = CUTSCENE_NAME_MAP[i];
		if (map.idx == idx_) {
			return map.name;
		}
	}
	return nullptr;
}

const char* CutsceneMap::get_menu_from_idx(Game::DemoFlags idx_)
{
	for (size_t i = 0; i < NUM_CUTSCENES; i++) {
		CutsceneMap& map = CUTSCENE_NAME_MAP[i];
		if (map.idx == idx_) {
			return map.submenu_name;
		}
	}
	return nullptr;
}

CutsceneMenuOption* CutsceneMgr::create_option(Game::DemoFlags id)
{
	GZASSERTLINE(id < Game::DEMO_FLAG_COUNT);
	CutsceneToggle* toggle = p2gz->cutscene_mgr->get_toggle(id);
	return new CutsceneMenuOption(CutsceneMap::get_name_from_idx(id), false,
	                              new Delegate1<CutsceneToggle, bool>(toggle, &CutsceneToggle::set_cutscene_flag));
}
