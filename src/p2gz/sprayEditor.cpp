#include <p2gz/SprayEditor.h>
#include <p2gz/p2gz.h>
#include <Game/gamePlayData.h>

using namespace gz;

void SprayEditor::init()
{
	spray_menu = static_cast<ListMenu*>(p2gz->menu->get_option("items/sprays")->get_sub_menu());
}

void SprayEditor::set_bitters(s32 count)
{
	if (!Game::playData->isDemoFlag(Game::DEMO_BITTER_ENABLED)) {
		static_cast<ToggleMenuOption*>(spray_menu->get_option("bitters unlocked"))->set_selection(true);
		toggle_bitters(true);
	}
	Game::playData->mSprayCount[1] = count;
}

void SprayEditor::set_spicies(s32 count)
{
	if (!Game::playData->isDemoFlag(Game::DEMO_SPICY_ENABLED)) {
		static_cast<ToggleMenuOption*>(spray_menu->get_option("spicies unlocked"))->set_selection(true);
		toggle_spicies(true);
	}
	Game::playData->mSprayCount[0] = count;
}

void SprayEditor::toggle_bitters(bool unlocked)
{
	if (unlocked) {
		Game::playData->setDemoFlag(Game::DEMO_First_Bitter_Berry);
		Game::playData->setDemoFlag(Game::DEMO_First_Bitter_Spray_Made);
		Game::playData->setDemoFlag(Game::DEMO_BITTER_ENABLED);
	} else {
		Game::playData->mSprayCount[1] = 0;
		static_cast<RangeMenuOption*>(spray_menu->get_option("bitters"))->set_selection(0);
		Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Bitter_Berry);
		Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Bitter_Spray_Made);
		Game::playData->mDemoFlags.resetFlag(Game::DEMO_BITTER_ENABLED);
	}
}

void SprayEditor::toggle_spicies(bool unlocked)
{
	if (unlocked) {
		Game::playData->setDemoFlag(Game::DEMO_First_Spicy_Berry);
		Game::playData->setDemoFlag(Game::DEMO_First_Spicy_Spray_Made);
		Game::playData->setDemoFlag(Game::DEMO_SPICY_ENABLED);
	} else {
		Game::playData->mSprayCount[0] = 0;
		static_cast<RangeMenuOption*>(spray_menu->get_option("spicies"))->set_selection(0);
		Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Spicy_Berry);
		Game::playData->mDemoFlags.resetFlag(Game::DEMO_First_Spicy_Spray_Made);
		Game::playData->mDemoFlags.resetFlag(Game::DEMO_SPICY_ENABLED);
	}
}

void SprayEditor::update()
{
	if (p2gz->menu->is_open() && p2gz->menu->get_active_layer() && p2gz->menu->get_active_layer()->title
	    && strcmp(p2gz->menu->get_active_layer()->title, "sprays") == 0) {
		return;
	}

	static_cast<ToggleMenuOption*>(spray_menu->get_option("bitters unlocked"))
	    ->set_selection(Game::playData->isDemoFlag(Game::DEMO_BITTER_ENABLED));
	static_cast<ToggleMenuOption*>(spray_menu->get_option("spicies unlocked"))
	    ->set_selection(Game::playData->isDemoFlag(Game::DEMO_SPICY_ENABLED));
}
