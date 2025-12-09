#ifndef _GZ_DROP_EDITOR_H
#define _GZ_DROP_EDITOR_H

#include <types.h>

namespace gz {

// structure for editing enemy drops - currently just eggs, but could force spray drops or specific pellets later
struct DropEditor {
public:
	DropEditor()
	    : enabled_egg_always_mitites(false)
	{
	}

	void set_egg_always_mitites(bool enabled) { enabled_egg_always_mitites = enabled; }
	bool is_egg_always_mitites() { return enabled_egg_always_mitites; }

private:
	bool enabled_egg_always_mitites;
};

} // namespace gz

#endif
