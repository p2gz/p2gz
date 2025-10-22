#include <p2gz/p2gz.h>
#include <p2gz/LanguageSwap.h>
#include <System.h>
#include <P2JME/P2JME.h>

using namespace gz;

void LanguageSwap::init()
{
	RadioMenuOption* language_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("localization/Menu Text"));
	// Add each available language here
	language_opt->options.push("English");
	language_opt->options.push("French");
	language_opt->options.push("German");
	language_opt->options.push("Dutch (Unused)");
	language_opt->options.push("Italian");
	language_opt->options.push("Japanese");
	language_opt->options.push("Spanish");

	language_opt->set_selection(sys->mRegion);

	// update_cave_opt();
	// update_sublevel_opt();
}

void LanguageSwap::set_language(size_t new_language_id)
{
	language_id  = static_cast<System::LanguageID>(new_language_id);
	sys->mRegion = language_id;
	gP2JMEMgr->reloadMessageResource();
}