#ifndef _GZ_LanguageSwap_H
#define _GZ_LanguageSwap_H

#include <System.h>

struct LanguageSwap {

public:
	LanguageSwap()
	    : need_to_change_flag(false)
	    , treasure_region_id(System::LANG_English)
	{
	}

	void init_menu();
	System::LanguageID get_treasure_region() { return treasure_region_id; }

	// Sets the region for which treasures to load
	void set_treasure_region(size_t new_treasure_region_id);

private:
	// Used to determine which treasure to spawn if it's different in a region (US vs. PAL vs. JP)
	System::LanguageID treasure_region_id;
	// We can only change language in title menu, so use this flag to let us know when todo that next time we enter the menu
	bool need_to_change_flag;
};

#endif
