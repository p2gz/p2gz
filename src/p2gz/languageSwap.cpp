#include <p2gz/p2gz.h>
#include <p2gz/LanguageSwap.h>
#include <System.h>
#include <P2JME/P2JME.h>

using namespace gz;

LanguageSwap* global_language_swap;

void LanguageSwap::LanguageSwapMenuWrapper::init_menu()
{
	JUT_ASSERTLINE(10, p2gz->menu, "no p2gz menu!\n");

	// // First setup the menu sub-otptions
	// ListMenu* localization_menu = static_cast<ListMenu*>(p2gz->menu->get_option("localization")->get_sub_menu());
	// localization_menu
	//     ->push(new RadioMenuOption("menu text", new Delegate1<LanguageSwap, size_t>(global_language_swap, &LanguageSwap::set_language)))
	//     ->push(new RadioMenuOption("treasure region",
	//                                new Delegate1<LanguageSwap, size_t>(global_language_swap, &LanguageSwap::set_treasure_region)));

	RadioMenuOption* language_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("localization/menu text"));
	// Add each available language here (order of how we add them to the menu matters!!!)
	language_opt->options.push("English");
	language_opt->options.push("Japanese");
	language_opt->options.push("French");
	language_opt->options.push("Spanish");
	language_opt->options.push("Italian");
	language_opt->options.push("German");
	// language_opt->options.push("Dutch (Unused)");

	// Internal region ID is not 1-to-1 with our order above, so convert that here
	switch (sys->mRegion) {
	case System::LANG_English:
	default:
		language_opt->set_selection(0);
		break;

	case System::LANG_Japanese:
		language_opt->set_selection(1);
		break;

	case System::LANG_French:
		language_opt->set_selection(2);
		break;

	case System::LANG_Spanish:
		language_opt->set_selection(3);
		break;

	case System::LANG_Italian:
		language_opt->set_selection(4);
		break;

	case System::LANG_German:
		language_opt->set_selection(5);
		break;
	}

	// Setup treasure region option
	RadioMenuOption* region_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("localization/treasure region"));
	// Add 3 regions below (order of how we add them to the menu matters!!!)
	region_opt->options.push("English");
	region_opt->options.push("Japanese");
	region_opt->options.push("PAL");
}

void LanguageSwap::LanguageSwapMenuWrapper::set_language(size_t new_language_id)
{
	global_language_swap->set_language(new_language_id);
}

void LanguageSwap::LanguageSwapMenuWrapper::set_treasure_region(size_t new_treasure_region_id)
{
	global_language_swap->set_treasure_region(new_treasure_region_id);
}

void LanguageSwap::set_language(size_t new_language_id)
{
	// Set swap flag; next time we enter the main menu we'll swap languages
	need_to_change_flag = true;
	// ID in the menu is not 1-to-1 with internal ID's, fix that here
	switch (new_language_id) {
	case 0:
	default:
		language_id = System::LANG_English;
		break;

	case 1:
		language_id = System::LANG_Japanese;
		break;

	case 2:
		language_id = System::LANG_French;
		break;

	case 3:
		language_id = System::LANG_Spanish;
		break;

	case 4:
		language_id = System::LANG_Italian;
		break;

	case 5:
		language_id = System::LANG_German;
		break;
	}
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

void LanguageSwap::swap_language()
{
	// Actually swap the language here
	sys->mRegion = language_id;
	gP2JMEMgr->reloadMessageResource();
	// Clear swap flag, so we don't try to change language again in main menu until a new language is set
	need_to_change_flag = false;
}
