#ifndef _GZ_LanguageSwap_H
#define _GZ_LanguageSwap_H

#include <System.h>

namespace gz {

struct LanguageSwap {
public:
	LanguageSwap()
	    : language_id(System::LANG_English)
	{
	}

	void init();
	void set_language(size_t new_language_id);
	System::LanguageID get_language() { return language_id; }

private:
	System::LanguageID language_id;
};

}; // namespace gz

#endif
