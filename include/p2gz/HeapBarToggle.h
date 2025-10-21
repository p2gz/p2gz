#ifndef _GZ_HEAPBARTOGGLE_H
#define _GZ_HEAPBARTOGGLE_H

// predeclarations
namespace Game {
struct Creature;
struct MovieConfig;
} // namespace Game

namespace gz {

struct HeapBarToggle {
public:
	HeapBarToggle()
	    : enabled(false)
	{
	}

	void toggle_heapbar(bool enabled_) { enabled = enabled_; }

	bool get_heapbar_status() { return enabled; }

private:
	bool enabled;
};

}; // namespace gz

#endif
