#ifndef _TREASURE_EDITOR_H
#define _TREASURE_EDITOR_H

#include <p2gz/gzmenu.h>

namespace gz {
struct TreasureEditor {
public:
	TreasureEditor() { }
	~TreasureEditor() { }

	void disable();
	void enable();
	void init();
	void update();

private:
	ListMenu* treasures;
};
} // namespace gz

#endif
