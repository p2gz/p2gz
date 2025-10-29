#ifndef _GZ_LanguageSwap_H
#define _GZ_LanguageSwap_H

#include <System.h>

namespace gz {

struct LanguageSwap {
public:
	LanguageSwap()
	    : language_id(System::LANG_English)
	    , need_to_change_flag(false)
	{
	}

	void init();
	System::LanguageID get_language() { return language_id; }
	bool need_to_change_lang() { return need_to_change_flag; }

	// Sets the language we need to swap to, and sets flag to let the game know to swap languages next time we are in main menu
	void set_language(size_t new_language_id);

	// Function that actually swaps languages, and updates BMG to current languages
	void swap_language();

private:
	System::LanguageID language_id;
	// We can only change language in title menu, so use this flag to let us know when todo that next time we enter the menu
	bool need_to_change_flag;
};

}; // namespace gz

#endif
