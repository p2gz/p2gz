#include <p2gz/p2gz.h>
#include <p2gz/LanguageSwap.h>
#include <System.h>
#include <P2JME/P2JME.h>

using namespace gz;

void LanguageSwap::init()
{
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

	// update_cave_opt();
	// update_sublevel_opt();
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

void LanguageSwap::swap_language()
{
	// Actually swap the language here
	sys->mRegion = language_id;
	gP2JMEMgr->reloadMessageResource();
	// Clear swap flag, so we don't try to change language again in main menu until a new language is set
	need_to_change_flag = false;
}
