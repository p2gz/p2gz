#ifndef _SQUAD_EDITOR_H
#define _SQUAD_EDITOR_H

#include <Game/Piki.h>

namespace gz {
struct SquadEditor {
public:
	SquadEditor() { }
	~SquadEditor() { }

private:
	void birth_piki(Game::EPikiKind, Game::EPikiHappa);
	void kill_piki(Game::EPikiKind color, Game::EPikiHappa stage);
};
} // namespace gz
#endif
