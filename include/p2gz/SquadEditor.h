#ifndef _SQUAD_EDITOR_H
#define _SQUAD_EDITOR_H

#include <Game/Piki.h>
#include <p2gz/Preset.h>
#include <p2gz/gzCollections.h>
#include <p2gz/gzmenu.h>

namespace gz {

struct SquadEditor {
public:
	SquadEditor() { squad_menu = nullptr; }
	~SquadEditor() { squad_menu = nullptr; }

	void init();
	void sync();

	/// @brief Births the given number of pikmin. You must ensure this doesn't go over the field limit.
	void birth_piki(Game::EPikiKind, Game::EPikiHappa, int);

	/// @brief Sets the number of the given pikmin type on the field, birthing or killing as necessary.
	/// Safe to use with out of range values; will be clamped.
	void set_piki_count(Game::EPikiKind, Game::EPikiHappa, s32);

	/// @brief Kills up to the given number of pikmin. Safe to use with out of range values.
	/// Will not kill wild pikmin.
	void kill_piki(Game::EPikiKind, Game::EPikiHappa, int);

	void set_demo_flags_for_color(Game::EPikiKind);
	Game::PikiContainer get_squad();

	/// @brief Kills all pikmin and sprouts on the field, including wild pikmin if specified.
	void clear_all_pikmin(bool kill_wild = true);

private:
	RangeMenuOption* get_option(Game::EPikiKind, Game::EPikiHappa);

	Game::PikiContainer counts;
	GridMenu* squad_menu;
};

} // namespace gz
#endif
