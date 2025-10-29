#ifndef _SPRAY_EDITOR_H
#define _SPRAY_EDITOR_H

#include <p2gz/gzmenu.h>

namespace gz {
struct SprayEditor {
public:
	SprayEditor() { }
	~SprayEditor() { }

	void init();
	void update();

	bool get_bitters_unlocked();
	bool get_spicies_unlocked();
	s32 get_bitters();
	s32 get_spicies();

	void set_bitters(s32);
	void set_spicies(s32);
	void toggle_bitters(bool);
	void toggle_spicies(bool);

private:
	ListMenu* spray_menu;
};
} // namespace gz

#endif
