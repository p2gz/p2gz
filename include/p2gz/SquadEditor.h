#ifndef _SQUAD_EDITOR_H
#define _SQUAD_EDITOR_H

#include <Game/Piki.h>
#include <p2gz/gzCollections.h>

namespace gz {
struct SquadEditor {
public:
	SquadEditor() { }
	~SquadEditor() { }

	void update();
	void set_squad();

private:
	void birth_piki(Game::EPikiKind, Game::EPikiHappa);
	void kill_piki(Game::EPikiKind, Game::EPikiHappa);
	gz::Vec<s32> get_squad();

	gz::Vec<s32> counts;
};
} // namespace gz
#endif
