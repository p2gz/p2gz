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
	void update();

private:
	gz::DecimalInputOption* pokos;
};
} // namespace gz
#endif
