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

struct CutsceneToggle {

	struct CutsceneWrapper {
	public:
		CutsceneWrapper(Game::DemoFlags id_)
		    : cutscene_id(id_)
		{
		}

		// Enable or disable cutscene flags, allowing them to either play again or not
		// If setFlag is true, the cutscene will be disabled (Tell the game it already played); false means the cutscene can be triggered
		// again
		void set_cutscene_flag(bool setFlag);

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
