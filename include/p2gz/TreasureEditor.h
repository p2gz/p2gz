#ifndef _TREASURE_EDITOR_H
#define _TREASURE_EDITOR_H

#include <p2gz/gzmenu.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/PelletItem.h>

namespace gz {

struct TreasureEditor {
public:
	TreasureEditor()
	    : treasures(nullptr)
	    , active_treasure(nullptr)
	    , enabled(false)
	{
	}
	~TreasureEditor() { }

	void init();
	void update();
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

	void set_collected(const char* treasure_name, ToggleMenuOption* collected_opt, bool collected);
	void snap_to_nearest_waypoint();

private:
	// position of treasure in a cave (for respawning it)
	struct TreasureSpawn {
		TreasureSpawn()
		    : name(nullptr)
		{
		}

		const char* name; // pellet config
		Vector3f position;
	};

	Game::Pellet* spawn_treasure(const char* config_name);
	void sync_treasure_option(const char* treasure_name, ToggleMenuOption* treasure_collected_opt);
	void focus_treasure(const char* treasure_name);

	void record_spawn_position(const char* config_name, const Vector3f& position);
	bool get_spawn_position(const char* config_name, Vector3f& out);

	ListMenu* treasures;
	Game::Pellet* active_treasure;
	Vector3f initial_position;          // used when moving a treasure
	Vec<TreasureSpawn> spawn_positions; // spawn spots for treasures on the current (cave) floor
	bool enabled;
};

} // namespace gz

#endif
