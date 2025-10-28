#ifndef _POKO_EDITOR_H
#define _POKO_EDITOR_H

#include <types.h>
#include <p2gz/gzmenu.h>

namespace gz {
struct PokoEditor {
public:
	PokoEditor() { percent_cutscene = false; }
	~PokoEditor() { }

	void init();
	void update();

	bool check() { return percent_cutscene; }

	bool clear()
	{
		if (percent_cutscene) {
			percent_cutscene = false;
			return true;
		}
		return false;
	}

private:
	gz::DecimalInputOption* pokos;
	bool percent_cutscene;
};
} // namespace gz
#endif
