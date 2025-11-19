#ifndef _SQUAD_EDITOR_H
#define _SQUAD_EDITOR_H

#include <Game/Piki.h>
#include <p2gz/Preset.h>
#include <p2gz/gzCollections.h>
#include <p2gz/gzmenu.h>

namespace gz {

struct SquadEditor {
public:
	SquadEditor() { }
	~SquadEditor() { }

	void init();
	void update();
	void set_squad(s32);

	bool is_open() { return open; }

	void birth_piki(Game::EPikiKind, Game::EPikiHappa, int);
	void kill_piki(Game::EPikiKind, Game::EPikiHappa, int);
	void set_demo_flags_for_color(Game::EPikiKind);
	Game::PikiContainer get_squad();

	void clear_all_pikmin();

private:
	Game::PikiContainer counts;
	gz::GridMenu* squad_menu;
	bool open;
};

} // namespace gz
#endif
