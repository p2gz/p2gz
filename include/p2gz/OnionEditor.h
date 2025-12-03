#ifndef _ONION_EDITOR_H
#define _ONION_EDITOR_H

#include <p2gz/gzmenu.h>
#include <Game/Entities/ItemOnyon.h>
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
	void sync();

	void set_onion_unlocked(Game::EPikiKind color, bool unlocked);
	inline void set_piki_count(Game::EPikiKind color, Game::EPikiHappa stage, s32 selection);
	void move_onion(Game::Onyon*, Vector3f, f32);
	void kill_onion(int);

private:
	RangeMenuOption* get_pik_option(Game::EPikiKind color, Game::EPikiHappa stage);
	ToggleMenuOption* get_onion_option(Game::EPikiKind color);
	bool is_in_unlock_course(Game::Onyon*);

	GridMenu* onion_menu;
};

}; // namespace gz

#endif
