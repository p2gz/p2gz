#ifndef _GZ_EK_EDITOR_H
#define _GZ_EK_EDITOR_H

#include <Game/gamePlayData.h>

namespace gz {

struct EKEditor {
public:
	EKEditor() { }
	~EKEditor() { }

	void init();
	void set_upgrade(Game::OlimarData::ItemIndex, bool enabled);
	void check_upgrades();
};

}; // namespace gz

#endif
