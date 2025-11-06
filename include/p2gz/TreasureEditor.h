#ifndef _TREASURE_EDITOR_H
#define _TREASURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <Game/Entities/PelletItem.h>

namespace gz {
struct TreasureEditor {
public:
	TreasureEditor() { }
	~TreasureEditor() { }

	bool is_enabled() { return enabled; }
	Game::Pellet* get_active_treasure() { return active_treasure; }

	void disable() { enabled = false; }
	void enable();
	void init();
	void add(Game::Pellet*);
	void set_collected(Game::Pellet*, bool);
	void find_treasure();
	void handle_breadbug(Game::EnemyBase*);
	void handle_dweevil(Game::EnemyBase*);
	void clear_treasures();
	void toggle_collected(bool) { }
	void reset_active_treasure() { active_treasure->setPosition(initial_position, false); }
	void snap_to_nearest_waypoint();

private:
	ListMenu* treasures;
	Game::Pellet* active_treasure;
	Vector3f initial_position;
	bool enabled;
};

struct TreasureConfig {
	const char* internal_name;
	const char* external_name;
	Vector3f spawnpoint;
};
} // namespace gz

#endif
