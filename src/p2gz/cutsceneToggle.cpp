#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/CutsceneToggle.h>
#include <p2gz/p2gz.h>
#include <Game/gamePlayData.h>

using namespace gz;

static CutsceneMap CUTSCENE_NAME_MAP[] = {
	// Tutorials and "First X" cutscenes
	CutsceneMap(Game::DEMO_Unlock_Captain_Switch, "kill first bulborb", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Use_Louie, "first switch to louie", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Number_Pellet, "first pellet to onion", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Reunite_Captains, "reunite captains (crush bag)", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_You_Appear_Lost, "you appear lost", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Discover_Treasure, "discover first treasure", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Gate_Down, "first gate down", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Globe_Day_End, "first globe day end", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Nectar_Use, "first nectar flowering", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Sunset_Warning, "first sunset warning", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Whites_Digging, "white pikmin digging", TUTORIAL_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Spiderwort_Mold, "find spiderwort mold", TUTORIAL_MENU_TITLE),
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
	CutsceneMap(Game::DEMO_Pluck_First_Pikmin, "pluck first pikmin", PIKMIN_COLOR_MENU_TITLE),
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
	CutsceneMap(Game::DEMO_Pikmin_In_Danger_Fire, "pikmin on fire", HAZARD_MENU_TITLE),
	CutsceneMap(Game::DEMO_Pikmin_In_Danger_Water, "pikmin bubbled", HAZARD_MENU_TITLE),
	CutsceneMap(Game::DEMO_Pikmin_In_Danger_Poison, "pikmin poisoned", HAZARD_MENU_TITLE),
	CutsceneMap(Game::DEMO_Eat_White_Pikmin, "enemy eating white pikmin", HAZARD_MENU_TITLE),

	// Spicy and bitter spray cutscenes
	CutsceneMap(Game::DEMO_First_Spicy_Berry, "spicy berry collected", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Spicy_Spray_Made, "spicy spray made", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Spicy_Drop, "find spicy drop", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Spicy_Use, "spicy use", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Bitter_Berry, "bitter berry collected", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Bitter_Spray_Made, "bitter spray made", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_Find_Bitter_Drop, "find bitter drop", SPRAYS_MENU_TITLE),
	CutsceneMap(Game::DEMO_First_Bitter_Use, "bitter use", SPRAYS_MENU_TITLE),
};

static const size_t NUM_CUTSCENES = ARRAY_SIZE(CUTSCENE_NAME_MAP);

static EKCutsceneMap EK_CUTSCENE_NAME_MAP[] = {
	EKCutsceneMap(Game::OlimarData::ODII_BruteKnuckles, "brute knuckles", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_DreamMaterial, "dream material", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_AmplifiedAmplifier, "amplifier amplifier", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_ProfessionalNoisemaker, "professional noisemaker", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_StellarOrb, "stellar orb", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_JusticeAlloy, "justice alloy", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_ForgedCourage, "forged courage", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_RepugnantAppendage, "repugnant appendage", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_PrototypeDetector, "prototype detector", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_FiveManNapsack, "five man napsack", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_SphericalAtlas, "spherical atlas", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_GeographicProjection, "geographic projection", EK_MENU_TITLE),
	EKCutsceneMap(Game::OlimarData::ODII_TheKey, "the key", EK_MENU_TITLE),
};

static const size_t NUM_EK_CUTSCENES = ARRAY_SIZE(EK_CUTSCENE_NAME_MAP);

static CaveCutsceneMap CAVE_CUTSCENE_NAME_MAP[] = {
	CaveCutsceneMap(CAVE_EC, "emergence cave", CAVE_MENU_TITLE),       CaveCutsceneMap(CAVE_SCx, "subterranean complex", CAVE_MENU_TITLE),
	CaveCutsceneMap(CAVE_FC, "frontier cavern", CAVE_MENU_TITLE),      CaveCutsceneMap(CAVE_HoB, "hole of beasts", CAVE_MENU_TITLE),
	CaveCutsceneMap(CAVE_WFG, "white flower garden", CAVE_MENU_TITLE), CaveCutsceneMap(CAVE_BK, "bulblax kingdom", CAVE_MENU_TITLE),
	CaveCutsceneMap(CAVE_SH, "snagret hole", CAVE_MENU_TITLE),         CaveCutsceneMap(CAVE_CoS, "citadel of spiders", CAVE_MENU_TITLE),
	CaveCutsceneMap(CAVE_GK, "gluttons kitchen", CAVE_MENU_TITLE),     CaveCutsceneMap(CAVE_SR, "shower room", CAVE_MENU_TITLE),
	CaveCutsceneMap(CAVE_SmC, "submerged castle", CAVE_MENU_TITLE),    CaveCutsceneMap(CAVE_CoC, "cavern of chaos", CAVE_MENU_TITLE),
	CaveCutsceneMap(CAVE_HoH, "hole of heroes", CAVE_MENU_TITLE),      CaveCutsceneMap(CAVE_DD, "dream den", CAVE_MENU_TITLE),
};

static const size_t NUM_CAVE_CUTSCENES = ARRAY_SIZE(CAVE_CUTSCENE_NAME_MAP);

CutsceneMgr::CutsceneMgr()
{
	// need to initialise these before gzmenu gets initialised
	// regular demo cutscenes
	for (size_t i = 0; i < NUM_CUTSCENES; i++) {
		Game::DemoFlags id = CUTSCENE_NAME_MAP[i].idx;
		cutscene_list.push(new CutsceneToggle(id));
	}

	// exploration kit treasure discovery cutscenes
	for (size_t i = 0; i < NUM_EK_CUTSCENES; i++) {
		Game::OlimarData::ItemIndex id = EK_CUTSCENE_NAME_MAP[i].idx;
		ek_cutscene_list.push(new EKCutsceneToggle(id));
	}

	// cave discovery cutscenes
	for (size_t i = 0; i < NUM_CAVE_CUTSCENES; i++) {
		CaveIndex id = CAVE_CUTSCENE_NAME_MAP[i].idx;
		cave_cutscene_list.push(new CaveCutsceneToggle(id));
	}
}

void CutsceneMgr::init()
{
	// clang-format off
	static_cast<ListMenu*>(p2gz->menu->get_option("cutscenes")->get_sub_menu())
		->push(new OpenSubMenuOption(TUTORIAL_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_Unlock_Captain_Switch))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Use_Louie))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Number_Pellet))
			->push(CutsceneMgr::create_option(Game::DEMO_Reunite_Captains))
			->push(CutsceneMgr::create_option(Game::DEMO_You_Appear_Lost))
			->push(CutsceneMgr::create_option(Game::DEMO_Discover_Treasure))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Gate_Down))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Globe_Day_End))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Nectar_Use))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Sunset_Warning))
			->push(CutsceneMgr::create_option(Game::DEMO_Whites_Digging))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Spiderwort_Mold))
			->push(CutsceneMgr::create_option(Game::DEMO_President_Start))
		))
		->push(new OpenSubMenuOption(AREA_ENTER_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_Day_One_Start))
			->push(CutsceneMgr::create_option(Game::DEMO_Enter_Awakening_Wood))
			->push(CutsceneMgr::create_option(Game::DEMO_Enter_Perplexing_Pool))
			->push(CutsceneMgr::create_option(Game::DEMO_Enter_Wistful_Wild))
		))
		->push(new OpenSubMenuOption(CAVE_RELATED_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_First_Cave_Enter))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Corpse_In_Cave))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Cave_Deeper_Hole))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Cave_Geyser))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Cave_Return))
			->push(CutsceneMgr::create_option(Game::DEMO_Waterwraith_Appears))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Titan_Dweevil))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Loozy_Treasure))
		))
		->push(new OpenSubMenuOption(PIKMIN_COLOR_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_Louie_Finds_Red_Onion))
			->push(CutsceneMgr::create_option(Game::DEMO_Pluck_First_Pikmin))
			->push(CutsceneMgr::create_option(Game::DEMO_Meet_Red_Pikmin))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Yellow_Onion))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Blue_Onion))
			->push(CutsceneMgr::create_option(Game::DEMO_Purple_Candypop))
			->push(CutsceneMgr::create_option(Game::DEMO_White_Candypop))
			->push(CutsceneMgr::create_option(Game::DEMO_Discover_Bulbmin))
		))
		->push(new OpenSubMenuOption(PIKMIN_GROUP_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_Reds_In_Onion))
			->push(CutsceneMgr::create_option(Game::DEMO_Purples_In_Ship))
			->push(CutsceneMgr::create_option(Game::DEMO_Reds_Purples_Tutorial))
			->push(CutsceneMgr::create_option(Game::DEMO_Whites_In_Ship))
			->push(CutsceneMgr::create_option(Game::DEMO_Max_Pikmin_On_Field))
		))
		->push(new OpenSubMenuOption(HAZARD_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_Pikmin_In_Danger_Fire))
			->push(CutsceneMgr::create_option(Game::DEMO_Pikmin_In_Danger_Water))
			->push(CutsceneMgr::create_option(Game::DEMO_Pikmin_In_Danger_Poison))
			->push(CutsceneMgr::create_option(Game::DEMO_Eat_White_Pikmin))
		))
		->push(new OpenSubMenuOption(SPRAYS_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_option(Game::DEMO_First_Spicy_Berry))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Spicy_Spray_Made))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Spicy_Drop))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Spicy_Use))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Bitter_Berry))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Bitter_Spray_Made))
			->push(CutsceneMgr::create_option(Game::DEMO_Find_Bitter_Drop))
			->push(CutsceneMgr::create_option(Game::DEMO_First_Bitter_Use))
		))
		->push(new OpenSubMenuOption(EK_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_SphericalAtlas))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_GeographicProjection))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_PrototypeDetector))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_FiveManNapsack))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_JusticeAlloy))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_ForgedCourage))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_StellarOrb))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_BruteKnuckles))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_RepugnantAppendage))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_TheKey))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_DreamMaterial))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_AmplifiedAmplifier))
			->push(CutsceneMgr::create_ek_option(Game::OlimarData::ODII_ProfessionalNoisemaker))
		))
		->push(new OpenSubMenuOption(CAVE_MENU_TITLE, (new ListMenu())
			->push(CutsceneMgr::create_cave_option(CAVE_EC))
			->push(CutsceneMgr::create_cave_option(CAVE_SCx))
			->push(CutsceneMgr::create_cave_option(CAVE_FC))
			->push(CutsceneMgr::create_cave_option(CAVE_HoB))
			->push(CutsceneMgr::create_cave_option(CAVE_WFG))
			->push(CutsceneMgr::create_cave_option(CAVE_SH))
			->push(CutsceneMgr::create_cave_option(CAVE_BK))
			->push(CutsceneMgr::create_cave_option(CAVE_CoS))
			->push(CutsceneMgr::create_cave_option(CAVE_GK))
			->push(CutsceneMgr::create_cave_option(CAVE_SR))
			->push(CutsceneMgr::create_cave_option(CAVE_SmC))
			->push(CutsceneMgr::create_cave_option(CAVE_CoC))
			->push(CutsceneMgr::create_cave_option(CAVE_HoH))
			->push(CutsceneMgr::create_cave_option(CAVE_DD))
		));
	// clang-format on

	// regular cutscenes
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		CutsceneToggle* toggle = cutscene_list[i];
		Game::DemoFlags id     = toggle->get_cutscene_id();
		char option_path[PATH_MAX];
		sprintf(option_path, "cutscenes/%s/%s", CutsceneMap::get_menu_from_idx(id), CutsceneMap::get_name_from_idx(id));
		toggle->init(static_cast<CutsceneMenuOption*>(p2gz->menu->get_option(option_path)));
	}

	// exploration kit treasure discovery cutscenes
	for (size_t i = 0; i < ek_cutscene_list.len(); i++) {
		EKCutsceneToggle* toggle       = ek_cutscene_list[i];
		Game::OlimarData::ItemIndex id = toggle->get_cutscene_id();
		char option_path[PATH_MAX];
		sprintf(option_path, "cutscenes/%s/%s", EKCutsceneMap::get_menu_from_idx(id), EKCutsceneMap::get_name_from_idx(id));
		toggle->init(static_cast<CutsceneMenuOption*>(p2gz->menu->get_option(option_path)));
	}

	// cave discovery cutscenes
	for (size_t i = 0; i < cave_cutscene_list.len(); i++) {
		CaveCutsceneToggle* toggle = cave_cutscene_list[i];
		CaveIndex id               = toggle->get_cutscene_id();
		char option_path[PATH_MAX];
		sprintf(option_path, "cutscenes/%s/%s", CaveCutsceneMap::get_menu_from_idx(id), CaveCutsceneMap::get_name_from_idx(id));
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

	// only update if we don't have a (direct) cutscene submenu open
	if (p2gz->menu->is_active_menu_parent("cutscenes")) {
		return;
	}

	// sync all regular cutscene flags from playData
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		Game::DemoFlags id = cutscene_list[i]->get_cutscene_id();
		cutscene_list[i]->get_option()->set_selection(Game::playData->isDemoFlag(id));
	}

	// sync all EK treasure cutscene flags from playData
	for (size_t i = 0; i < ek_cutscene_list.len(); i++) {
		Game::OlimarData::ItemIndex id = ek_cutscene_list[i]->get_cutscene_id();
		ek_cutscene_list[i]->get_option()->set_selection(Game::playData->isFindItemDemoFlag(id));
	}

	// sync all cave discovery cutscene flags from playData
	for (size_t i = 0; i < cave_cutscene_list.len(); i++) {
		CaveIndex id = cave_cutscene_list[i]->get_cutscene_id();
		ID32 caveID  = get_id_from_cave(id);
		cave_cutscene_list[i]->get_option()->set_selection(!Game::playData->isCaveFirstTime(get_area_from_cave(id), caveID));
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

EKCutsceneToggle* CutsceneMgr::get_ek_toggle(Game::OlimarData::ItemIndex id)
{
	for (size_t i = 0; i < ek_cutscene_list.len(); i++) {
		if (ek_cutscene_list[i]->get_cutscene_id() == id) {
			return ek_cutscene_list[i];
		}
	}
	return nullptr;
}

CaveCutsceneToggle* CutsceneMgr::get_cave_toggle(CaveIndex id)
{
	for (size_t i = 0; i < cave_cutscene_list.len(); i++) {
		if (cave_cutscene_list[i]->get_cutscene_id() == id) {
			return cave_cutscene_list[i];
		}
	}
	return nullptr;
}

void CutsceneMgr::reset_all()
{
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		cutscene_list[i]->set_cutscene_flag(false);
	}
	for (size_t i = 0; i < ek_cutscene_list.len(); i++) {
		ek_cutscene_list[i]->set_cutscene_flag(false);
	}
	for (size_t i = 0; i < cave_cutscene_list.len(); i++) {
		cave_cutscene_list[i]->set_cutscene_flag(false);
	}
}

void CutsceneMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		x += p2gz->images->draw(image_name, x, z - p2gz->images->height());
		x += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		x += j2d.print(x, z, "%s: %s", title, on ? "already played" : "not yet played");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "toggle played");
	}
}

void CutsceneToggle::set_cutscene_flag(bool played)
{
	// force selection to playData
	if (played) {
		// game now thinks cutscene has been played
		Game::playData->setDemoFlag(cutscene_id);

		// cutscene-specific interactions
		if (get_cutscene_id() == Game::DEMO_Pluck_First_Pikmin) {
			// unlock reds
			Game::playData->setMeetPikmin(Game::Red);
			Game::playData->setContainer(Game::Red);
		}
	} else {
		// game now thinks cutscene has NOT been played
		Game::playData->mDemoFlags.resetFlag(cutscene_id);

		// cutscene-specific interactions
		if (get_cutscene_id() == Game::DEMO_Pluck_First_Pikmin) {
			// plucking first pikmin unlocks reds - need to re-prime this trigger
			Game::playData->mMeetPikminFlags &= ~(1 << Game::Red);
			Game::playData->mHasContainerFlags &= ~(1 << Game::Red);
		}
	}
}

void EKCutsceneToggle::set_cutscene_flag(bool played)
{
	// force selection to playData
	if (played) {
		// game now thinks cutscene has been played
		Game::playData->setFindItemDemoFlag(cutscene_id);

	} else {
		// game now thinks cutscene has NOT been played
		Game::playData->mFindItemFlags.resetFlag(cutscene_id);
	}
}

void CaveCutsceneToggle::set_cutscene_flag(bool played)
{
	// force selection to playData
	// NB: adapted from checks in PlayData::setCaveVisit
	if (played) {
		// game now thinks cutscene has been played
		CourseIndex area       = get_area_from_cave(cutscene_id);
		Game::CourseInfo* info = Game::stageList->getCourseInfo(area);
		if (!info) {
			return;
		}
		Game::PlayData::CaveOtakara* ota    = &Game::playData->mCaveOtakara[area];
		Game::PlayData::CaveOtakara* otaOld = &Game::playData->mCaveOtakaraOld[area];
		ID32 caveID                         = get_id_from_cave(cutscene_id);
		int id                              = info->getCaveIndex_FromID(caveID);
		if (id != -1) {
			// set both so it doesn't do any animations on the worldmap screen
			ota->mVisitStatus[id]    = 1;
			otaOld->mVisitStatus[id] = 1;
		}

	} else {
		// game now thinks cutscene has NOT been played
		CourseIndex area       = get_area_from_cave(cutscene_id);
		Game::CourseInfo* info = Game::stageList->getCourseInfo(area);
		if (!info) {
			return;
		}
		Game::PlayData::CaveOtakara* ota    = &Game::playData->mCaveOtakara[area];
		Game::PlayData::CaveOtakara* otaOld = &Game::playData->mCaveOtakaraOld[area];
		ID32 caveID                         = get_id_from_cave(cutscene_id);
		int id                              = info->getCaveIndex_FromID(caveID);
		if (id != -1) {
			ota->mVisitStatus[id]    = 0;
			otaOld->mVisitStatus[id] = 0;
		}
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

const char* EKCutsceneMap::get_name_from_idx(Game::OlimarData::ItemIndex idx_)
{
	for (size_t i = 0; i < NUM_EK_CUTSCENES; i++) {
		EKCutsceneMap& map = EK_CUTSCENE_NAME_MAP[i];
		if (map.idx == idx_) {
			return map.name;
		}
	}
	return nullptr;
}

const char* EKCutsceneMap::get_menu_from_idx(Game::OlimarData::ItemIndex idx_)
{
	for (size_t i = 0; i < NUM_EK_CUTSCENES; i++) {
		EKCutsceneMap& map = EK_CUTSCENE_NAME_MAP[i];
		if (map.idx == idx_) {
			return map.submenu_name;
		}
	}
	return nullptr;
}

const char* CaveCutsceneMap::get_name_from_idx(CaveIndex idx_)
{
	for (size_t i = 0; i < NUM_CAVE_CUTSCENES; i++) {
		CaveCutsceneMap& map = CAVE_CUTSCENE_NAME_MAP[i];
		if (map.idx == idx_) {
			return map.name;
		}
	}
	return nullptr;
}

const char* CaveCutsceneMap::get_menu_from_idx(CaveIndex idx_)
{
	for (size_t i = 0; i < NUM_CAVE_CUTSCENES; i++) {
		CaveCutsceneMap& map = CAVE_CUTSCENE_NAME_MAP[i];
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

CutsceneMenuOption* CutsceneMgr::create_ek_option(Game::OlimarData::ItemIndex id)
{
	GZASSERTLINE(id >= Game::OlimarData::ODII_FIRST_EXPLORATION_KIT_ITEM && id < Game::OlimarData::ODII_COUNT);
	EKCutsceneToggle* toggle = p2gz->cutscene_mgr->get_ek_toggle(id);
	return new CutsceneMenuOption(EKCutsceneMap::get_name_from_idx(id), false,
	                              new Delegate1<EKCutsceneToggle, bool>(toggle, &EKCutsceneToggle::set_cutscene_flag));
}

CutsceneMenuOption* CutsceneMgr::create_cave_option(CaveIndex id)
{
	GZASSERTLINE(id > CAVE_AboveGround && id < CAVE_COUNT);
	CaveCutsceneToggle* toggle = p2gz->cutscene_mgr->get_cave_toggle(id);
	return new CutsceneMenuOption(CaveCutsceneMap::get_name_from_idx(id), false,
	                              new Delegate1<CaveCutsceneToggle, bool>(toggle, &CaveCutsceneToggle::set_cutscene_flag));
}

CutscenesBitfield CutsceneMgr::get_cur_cutscenes()
{
	CutscenesBitfield bitfield;
	for (size_t i = 0; i < cutscene_list.len(); i++) {
		CutsceneToggle* toggle = cutscene_list[i];
		if (Game::playData->isDemoFlag(toggle->get_cutscene_id())) {
			bitfield.set_cutscene_played(toggle->get_cutscene_id());
		}
	}
	return bitfield;
}

BitFlag<u16> CutsceneMgr::get_cur_ek_cutscenes()
{
	BitFlag<u16> bitfield;
	for (size_t i = 0; i < ek_cutscene_list.len(); i++) {
		EKCutsceneToggle* toggle = ek_cutscene_list[i];
		if (Game::playData->isFindItemDemoFlag(toggle->get_cutscene_id())) {
			u16 mask = 1 << toggle->get_cutscene_id();
			bitfield.set(mask);
		}
	}
	return bitfield;
}

BitFlag<u16> CutsceneMgr::get_cur_cave_cutscenes()
{
	BitFlag<u16> bitfield;
	for (size_t i = 0; i < cave_cutscene_list.len(); i++) {
		CaveCutsceneToggle* toggle = cave_cutscene_list[i];
		ID32 caveID                = get_id_from_cave(toggle->get_cutscene_id());
		if (!Game::playData->isCaveFirstTime(get_area_from_cave(toggle->get_cutscene_id()), caveID)) {
			u16 mask = 1 << toggle->get_cutscene_id();
			bitfield.set(mask);
		}
	}
	return bitfield;
}
