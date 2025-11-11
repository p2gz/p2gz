#include <p2gz/p2gz.h>
#include <p2gz/LanguageSwap.h>
#include <System.h>
#include <P2JME/P2JME.h>

using namespace gz;

void LanguageSwap::init_menu()
{
	JUT_ASSERTLINE(10, p2gz->menu, "no p2gz menu!\n");

	// Setup treasure region option and 3 regions below (order of how we add them to the menu matters!!!)
	RadioMenuOption* region_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("localization/treasure region"));
	region_opt->options.push("English");
	region_opt->options.push("Japanese");
	region_opt->options.push("PAL");
}

void LanguageSwap::set_treasure_region(size_t new_treasure_region_id)
{
	// ID in the menu is not 1-to-1 with internal ID's, fix that here
	switch (new_treasure_region_id) {
	case 0:
	default:
		treasure_region_id = System::LANG_English;
		break;

	case 1:
		treasure_region_id = System::LANG_Japanese;
		break;

	// Internally, we'll have have all PAL treasures classified as french
	case 2:
		treasure_region_id = System::LANG_French;
		break;
	}
}
