#ifndef _GZ_LanguageSwap_H
#define _GZ_LanguageSwap_H

#include <System.h>

struct LanguageSwap {

	// Create a wrapper struct to be used by P2GZ obj for localization menu stuff; this can be safely recreated everytime we make a new p2gz
	struct LanguageSwapMenuWrapper {

		void init_menu();
		// Wrapper version of the function to be passed into the delegate; will call the LanguageSwap function version for actual
		// functionality
		void set_language(size_t new_language_id);
		// Wrapper version of the function to be passed into the delegate; will call the LanguageSwap function version for actual
		// functionality
		void set_treasure_region(size_t new_treasure_region_id);
	};

public:
	LanguageSwap()
	    : language_id(System::LANG_English)
	    , need_to_change_flag(false)
	    , treasure_region_id(System::LANG_English)
	{
	}

	System::LanguageID get_language() { return language_id; }
	System::LanguageID get_treasure_region() { return treasure_region_id; }

	bool need_to_change_lang() { return need_to_change_flag; }

	// Sets the language we need to swap to, and sets flag to let the game know to swap languages next time we are in main menu
	void set_language(size_t new_language_id);
	// Sets the region for which treasures to load
	void set_treasure_region(size_t new_treasure_region_id);

	// Function that actually swaps languages, and updates BMG to current languages
	void swap_language();

private:
	// Controls the language for the menu text/BLO
	System::LanguageID language_id;
	// Used to determine which treasure to spawn if it's different in a region (US vs. PAL vs. JP)
	System::LanguageID treasure_region_id;
	// We can only change language in title menu, so use this flag to let us know when todo that next time we enter the menu
	bool need_to_change_flag;
};

// @P2GZ localization-swap: localization object cannot be recreated as it messed with how the game determines region stuff
// Since currently we make a new p2gz object everytime we re-enter the main menu, we want to avoid making a new language swap object
// TODO: once we move the p2gz init to run once per game instead of everytime you enter file select/ch mode, we can remove this
extern LanguageSwap* global_language_swap;

#endif
