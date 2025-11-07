#ifndef _DISMISS_POSITIONS_H
#define _DISMISS_POSITIONS_H

#include <p2gz/gzCollections.h>
#include <Color4.h>
#include <Vector3.h>

namespace gz {
struct DismissPositions {
public:
	DismissPositions() { }
	~DismissPositions() { }

	void toggle(bool enabled_) { enabled = enabled_; }

	void draw();
	void update();

private:
	bool enabled;
	bool dismissed;
	Vector3f positions[8];
	f32 radii[8];
};
} // namespace gz

#endif
