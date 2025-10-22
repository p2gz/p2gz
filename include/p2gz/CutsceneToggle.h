#ifndef _GZ_CUTSCENE_TOGGLE_H
#define _GZ_CUTSCENE_TOGGLE_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/os.h>
#include <Game/gamePlayData.h>
#include <p2gz/gzmenu.h>
#include <p2gz/gzConstants.h>

namespace gz {

/// Labels for which menus which cutscene flags are under
/// (makes it easier to update just those cutscenes on menu opening)
/// 	NB: need to update this when updating the menu structure.
enum CutsceneMenuGroups {
	GROUP_Tutorial_Firsts,      // basic tutorials and "first X" cutscenes
	GROUP_Area_Enters,          // area enter cutscenes (incl. crash landing)
	GROUP_Cave_Related,         // things that happen in caves (or related to caves)
	GROUP_Pikmin_Colors,        // discovering pikmin/candypops/onions
	GROUP_Pikmin_Counts_Groups, // whites in ship, reds-purples, max on field, etc
	GROUP_Hazards,              // on fire, bubbled, and poisoned
	GROUP_Sprays,               // everything sprays and berries
};

struct CutsceneToggle {

	/// Structure for elements of the big cutscene name-group table
	struct CutsceneMap {
		CutsceneMap(Game::DemoFlags idx_, const char* name_, CutsceneMenuGroups group_)
		    : idx(idx_)
		    , name(name_)
		    , group(group_)
		{
		}

		const char* get_name_from_idx(Game::DemoFlags idx_);
		Game::DemoFlags get_idx_from_name(const char* name_);

		bool is(const char* name_) { return strcmp(name, name_) == 0; }

	private:
		const Game::DemoFlags idx;
		const char* name;
		const CutsceneMenuGroups group;
	};

	struct CutsceneWrapper {
	public:
		CutsceneWrapper(Game::DemoFlags id_)
		    : cutscene_id(id_)
		{
		}

		// set cutscenes as played (trigger disabled) or not played (trigger enabled)
		void set_cutscene_flag(bool played);

		Game::DemoFlags get_cutscene_id() { return cutscene_id; };

	private:
		// for this specifically - which cutscene this option is tracking
		Game::DemoFlags cutscene_id;
	};

public:
	void init();
	void update();

	void playdata_update_setting(Game::DemoFlags id);

private:
	Vec<CutsceneWrapper*> cutscene_wrapper_list;
	Vec<ListMenu*> cutscene_toggle_menu_list;
};

}; // namespace gz

#endif
