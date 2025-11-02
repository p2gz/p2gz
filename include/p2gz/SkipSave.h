#ifndef _GZ_SKIPSAVE_H
#define _GZ_SKIPSAVE_H

#include <types.h>

namespace gz {

struct SkipSave {
public:
	SkipSave()
	    : enabled(true)
	{
	}

	void toggle_save_skip(bool enabled_) { enabled = enabled_; }

	bool get_save_skip_status() { return enabled; }

private:
	// if enabled, skip save prompts
	bool enabled;
};

}; // namespace gz

#endif
