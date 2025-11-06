#ifndef _TREASURE_EDITOR_H
#define _TREASURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <Game/Entities/PelletItem.h>

namespace gz {
struct TreasureEditor {
public:
	TreasureEditor() { }
	~TreasureEditor() { }

	void disable() { enabled = false; }
	Game::Pellet* get_active_treasure() { return active_treasure; }
	bool is_enabled() { return enabled; }
	void reset_active_treasure() { active_treasure->setPosition(initial_position, false); }

	void add(Game::Pellet*);
	void clear_treasures();
	void enable();
	void init();
	void set_collected(Game::Pellet*, bool);
	void snap_to_nearest_waypoint();

private:
	void toggle_collected(bool) { }

	void find_treasure();
	void handle_breadbug(Game::EnemyBase*);
	void handle_dweevil(Game::EnemyBase*);

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
