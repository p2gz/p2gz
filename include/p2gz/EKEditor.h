#ifndef _GZ_EK_EDITOR_H
#define _GZ_EK_EDITOR_H

#include <BitFlag.h>
#include <p2gz/gzmenu.h>
#include <Game/gamePlayData.h>

namespace gz {

struct EKEditor {
public:
	EKEditor() { upgrades_menu = nullptr; }
	~EKEditor() { }

	void init();
	void set_upgrade(Game::OlimarData::ItemIndex, bool enabled);
	void reset_all();
	void check_upgrades();
	BitFlag<u16> get_upgrades_bitfield();

private:
	void set_menu_opt(Game::OlimarData::ItemIndex, const char* opt_name);

	ListMenu* upgrades_menu;
};

}; // namespace gz

#endif
