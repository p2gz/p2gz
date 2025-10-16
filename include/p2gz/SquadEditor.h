#ifndef _SQUAD_EDITOR_H
#define _SQUAD_EDITOR_H

#include <Game/Piki.h>

namespace gz {
struct SquadEditor {
public:
	SquadEditor() { }
	~SquadEditor() { }

	void set_count(Game::EPikiKind, Game::EPikiHappa, s32);
	void set_squad();

private:
	void birth_piki(Game::EPikiKind, Game::EPikiHappa);
	void kill_piki(Game::EPikiKind, Game::EPikiHappa);

	s32 counts[Game::PikiColorCount * Game::PikiHappaCount];
};
} // namespace gz
#endif
