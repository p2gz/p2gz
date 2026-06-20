#ifndef _POKO_EDITOR_H
#define _POKO_EDITOR_H

#include <types.h>
#include <p2gz/gzmenu.h>

namespace gz {
struct PokoEditor {
public:
	PokoEditor() { }
	~PokoEditor() { }

	void init();
	void sync();
	u32 get_pokos();
	void set_pokos(u32 pokos);
	void apply_cave_pokos();

	// variable which tracks whether a % CS will play next warp. 
	// set on warp by play_repay_demo, which is read from the presets
	bool repay_demo_enabled = false;

private:
	gz::DecimalInputOption* poko_menu;
};
} // namespace gz
#endif
