#ifndef _TREASURE_EDITOR_H
#define _TREASURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <Game/Entities/PelletItem.h>

namespace gz {

struct TreasureEditor {
public:
	TreasureEditor() { }
	~TreasureEditor() { }

	void init();
	void sync();

	void start_move(const char* treasure_name);
	void end_move()
	{
		enabled         = false;
		active_treasure = nullptr;
	}
	bool is_enabled() { return enabled; }

	Game::Pellet* get_active_treasure() { return active_treasure; }
	void reset_active_treasure() { active_treasure->setPosition(initial_position, false); }

	void add(Game::Pellet*);
	void add(const char* config_name);
	void clear_treasures();

	void set_collected(const char* treasure_name, bool);
	void snap_to_nearest_waypoint();

private:
	Game::Pellet* spawn_treasure(const char* config_name);
	void sync_treasure_option(const char* treasure_name, ToggleMenuOption* treasure_collected_opt);
	void focus_treasure(const char* treasure_name);

	ListMenu* treasures;
	Game::Pellet* active_treasure;
	Vector3f initial_position; // used when moving a treasure
	bool enabled;
};

} // namespace gz

#endif
