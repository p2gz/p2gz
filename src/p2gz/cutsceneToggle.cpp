#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/CutsceneToggle.h>
#include <p2gz/gzConstants.h>
#include <p2gz/p2gz.h>
#include <Game/gamePlayData.h>

using namespace gz;

static const size_t NUM_CUTSCENES = 57; // number of cutscenes in the game

static CutsceneToggle::CutsceneMap CUTSCENE_NAME_MAP[] = {
	// Tutorials and "First X" cutscenes
	CutsceneToggle::CutsceneMap(Game::DEMO_Unlock_Captain_Switch, "unlock captain switching (kill bulborb)", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Use_Louie, "first switch to louie", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_Pluck_First_Pikmin, "pluck first pikmin", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Number_Pellet, "first pellet to onion", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_Reunite_Captains, "reunite captains (crush bag)", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_You_Appear_Lost, "you appear lost", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_Discover_Treasure, "discover first treasure", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Gate_Down, "first gate down", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Globe_Day_End, "first globe day end", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Nectar_Use, "first nectar flowering", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Sunset_Warning, "first sunset warning", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_Whites_Digging, "white pikmin digging", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Spiderwort_Mold, "find mold", GROUP_Tutorial_Firsts),
	CutsceneToggle::CutsceneMap(Game::DEMO_President_Start, "start as president", GROUP_Tutorial_Firsts),

	// Area enters + crash landing
	CutsceneToggle::CutsceneMap(Game::DEMO_Day_One_Start, "crash landing", GROUP_Area_Enters),
	CutsceneToggle::CutsceneMap(Game::DEMO_Enter_Awakening_Wood, "enter awakening wood", GROUP_Area_Enters),
	CutsceneToggle::CutsceneMap(Game::DEMO_Enter_Perplexing_Pool, "enter perplexing pool", GROUP_Area_Enters),
	CutsceneToggle::CutsceneMap(Game::DEMO_Enter_Wistful_Wild, "enter wistful wild", GROUP_Area_Enters),

	// Cave-related cutscenes
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Cave_Enter, "first cave enter", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Corpse_In_Cave, "first bug body in cave", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Cave_Deeper_Hole, "find cave deeper hole", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Cave_Geyser, "find cave geyser", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Cave_Return, "first cave return", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_Waterwraith_Appears, "waterwraith appears", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Titan_Dweevil, "find titan dweevil", GROUP_Cave_Related),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Loozy_Treasure, "find louie (treasure)", GROUP_Cave_Related),

	// Pikmin, Candypop and Onion color unlocks
	CutsceneToggle::CutsceneMap(Game::DEMO_Louie_Finds_Red_Onion, "louie find red onion (louie skip)", GROUP_Pikmin_Colors),
	CutsceneToggle::CutsceneMap(Game::DEMO_Meet_Red_Pikmin, "meet red pikmin", GROUP_Pikmin_Colors),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Yellow_Onion, "find yellow onion", GROUP_Pikmin_Colors),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Blue_Onion, "find blue onion", GROUP_Pikmin_Colors),
	CutsceneToggle::CutsceneMap(Game::DEMO_Purple_Candypop, "discover purple candypop", GROUP_Pikmin_Colors),
	CutsceneToggle::CutsceneMap(Game::DEMO_White_Candypop, "discover white candypop", GROUP_Pikmin_Colors),
	CutsceneToggle::CutsceneMap(Game::DEMO_Discover_Bulbmin, "discover bulbmin", GROUP_Pikmin_Colors),

	// Pikmin count- and grouping-related cutscenes
	CutsceneToggle::CutsceneMap(Game::DEMO_Reds_In_Onion, "reds in onion", GROUP_Pikmin_Counts_Groups),
	CutsceneToggle::CutsceneMap(Game::DEMO_Purples_In_Ship, "purples in ship", GROUP_Pikmin_Counts_Groups),
	CutsceneToggle::CutsceneMap(Game::DEMO_Reds_Purples_Tutorial, "reds and purples together", GROUP_Pikmin_Counts_Groups),
	CutsceneToggle::CutsceneMap(Game::DEMO_Whites_In_Ship, "whites in ship", GROUP_Pikmin_Counts_Groups),
	CutsceneToggle::CutsceneMap(Game::DEMO_Max_Pikmin_On_Field, "max pikmin on field", GROUP_Pikmin_Counts_Groups),

	// Hazard-related cutscenes
	CutsceneToggle::CutsceneMap(Game::DEMO_Pikmin_In_Danger_Fire, "hazard: on fire", GROUP_Hazards),
	CutsceneToggle::CutsceneMap(Game::DEMO_Pikmin_In_Danger_Water, "hazard: bubbled/drowning", GROUP_Hazards),
	CutsceneToggle::CutsceneMap(Game::DEMO_Pikmin_In_Danger_Poison, "hazard: poisoned", GROUP_Hazards),
	CutsceneToggle::CutsceneMap(Game::DEMO_Eat_White_Pikmin, "first white eaten", GROUP_Hazards),

	// Spicy and bitter spray cutscenes
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Spicy_Berry, "first spicy berry collected", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Spicy_Spray_Made, "first spicy spray made", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Spicy_Drop, "find spicy drop", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Spicy_Use, "first spicy use", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Bitter_Berry, "first bitter berry collected", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Bitter_Spray_Made, "first bitter spray made", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_Find_Bitter_Drop, "find bitter drop", GROUP_Sprays),
	CutsceneToggle::CutsceneMap(Game::DEMO_First_Bitter_Use, "first bitter use", GROUP_Sprays),
};

static const size_t NUM_CUTSCENES = ARRAY_SIZE(CUTSCENE_NAME_MAP);

void CutsceneToggle::init()
{

	// Add options for each cutscene below: organized by the type of cutscene they are

	// Meet pikmin cutscenes //
	ListMenu* cutscene_toggle_base_list = static_cast<ListMenu*>(p2gz->menu->get_option("cutscenes/Meet Pikmin")->get_sub_menu());
	CutsceneWrapper* redPiki            = new CutsceneWrapper(Game::DEMO_Meet_Red_Pikmin);
	cutscene_wrapper_list.push(redPiki);

	cutscene_toggle_base_list->push(
	    new ToggleMenuOption("\"Meet Red Pikmin\" already played", true,
	                         new Delegate1<CutsceneWrapper, bool>(redPiki, &CutsceneToggle::CutsceneWrapper::set_cutscene_flag)));
}

void CutsceneToggle::update()
{
	// Update each cutscene menu option
	// Debug
	if (p2gz->menu->is_within_menu("cutscenes")) {
		// OSReport("Cur menu: %s\n", p2gz->menu->get_active_layer_title());
	}
}

void CutsceneToggle::CutsceneWrapper::set_cutscene_flag(bool setFlag)
{
	// Depending on which cutscene this option belongs to, trick the game into thinking it has either already played or not
	if (setFlag) {
		// Set the flag, trick the game that the cutscene played so it won't activate
		Game::playData->setDemoFlag(cutscene_id);
	} else {
		// Clear the flag, trick the game that the cutscene never played so it may be activated again
		// Playdata doesn't have a "resetDemoFlag", so we have to manually adjust the demoflag variable ourselves
		Game::playData->mDemoFlags.resetFlag(cutscene_id);
		// This is normally handled in 'setDemoFlag', but as mentioned we have todo this ourselves
		Game::playData->mDemoFlags.dump();
	}
}

Game::DemoFlags CutsceneToggle::CutsceneMap::get_idx_from_name(const char* name_)
{
	for (size_t i = 0; i < NUM_CUTSCENES; i++) {
		CutsceneMap& map = CUTSCENE_NAME_MAP[i];
		if (map.is(name_)) {
			return map.idx;
		}
	}
	return (Game::DemoFlags)-1;
}

const char* CutsceneToggle::CutsceneMap::get_name_from_idx(Game::DemoFlags idx_)
{
	for (size_t i = 0; i < NUM_CUTSCENES; i++) {
		CutsceneMap& map = CUTSCENE_NAME_MAP[i];
		if (map.idx == idx_) {
			return map.name;
		}
	}
	return nullptr;
}

void CutsceneToggle::playdata_update_setting(Game::DemoFlags id)
{
	for (int i = 0; i < cutscene_wrapper_list.len() - 1; i++) {
		if (cutscene_wrapper_list[i]->get_cutscene_id() == id) {
			cutscene_wrapper_list[i]->set_cutscene_flag(true);
		}
	}
}
