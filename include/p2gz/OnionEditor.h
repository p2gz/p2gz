#ifndef _ONION_EDITOR_H
#define _ONION_EDITOR_H

#include <p2gz/gzmenu.h>
#include <Vector3.h>

namespace gz {
struct OnionConfig {
	OnionConfig(Vector3f up, f32 ur, Vector3f lp = Vector3f::zero, f32 lr = 0.0f)
	    : unlocked_position(up)
	    , unlocked_rotation(ur)
	    , locked_position(lp)
	    , locked_rotation(lr)
	{
	}

	Vector3f unlocked_position;
	f32 unlocked_rotation;
	Vector3f locked_position;
	f32 locked_rotation;
};

struct OnionEditor {
public:
	OnionEditor() { }
	~OnionEditor() { }

	void init();
	void set_unlocked(bool);
	void set_count(s32);
	void move_onion(int, int, bool);
	void update();

private:
	GridMenu* onion_menu;
};
} // namespace gz

#endif
