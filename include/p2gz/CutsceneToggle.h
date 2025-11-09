#ifndef _GZ_CUTSCENE_TOGGLE_H
#define _GZ_CUTSCENE_TOGGLE_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/os.h>
#include <Game/gamePlayData.h>
#include <p2gz/gzmenu.h>

#define TUTORIAL_MENU_TITLE     "tutorials and firsts"
#define AREA_ENTER_MENU_TITLE   "area enters"
#define CAVE_RELATED_MENU_TITLE "cave related"
#define PIKMIN_COLOR_MENU_TITLE "pikmin colors"
#define PIKMIN_GROUP_MENU_TITLE "pikmin groups"
#define HAZARD_MENU_TITLE       "hazards"
#define SPRAYS_MENU_TITLE       "sprays"

namespace gz {

struct CutsceneMenuOption : public MenuOption {
public:
	CutsceneMenuOption(const char* title_, bool on_, IDelegate1<bool>* on_selected_, const char* image_name_ = nullptr,
	                   bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
	    , on(on_)
	    , on_selected(on_selected_)
	{
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);

	virtual bool select()
	{
		on = !on;
		if (on_selected) {
			on_selected->invoke(on);
		}
		return false;
	}

	void set_selection(bool selected) { on = selected; }

private:
	bool on;
	IDelegate1<bool>* on_selected;
};

/// Structure for elements of the big cutscene name-group table
struct CutsceneMap {
	CutsceneMap(Game::DemoFlags idx_, const char* name_, const char* submenu_name_)
	    : idx(idx_)
	    , name(name_)
	    , submenu_name(submenu_name_)
	{
	}

	static const char* get_name_from_idx(Game::DemoFlags idx_);
	static const char* get_menu_from_idx(Game::DemoFlags idx_);

	bool is(const char* name_) { return strcmp(name, name_) == 0; }

	const Game::DemoFlags idx;
	const char* name;
	const char* submenu_name;
};

struct CutsceneToggle {
public:
	CutsceneToggle(Game::DemoFlags id_)
	    : cutscene_id(id_)
	    , is_initialised(false)
	    , option(nullptr)
	{
	}

	// set cutscenes as played (trigger disabled) or not played (trigger enabled)
	void set_cutscene_flag(bool played);

	void init(CutsceneMenuOption* option_)
	{
		option         = option_;
		is_initialised = true;
	}

	Game::DemoFlags get_cutscene_id() { return cutscene_id; };
	CutsceneMenuOption* get_option()
	{
		if (is_initialised) {
			return option;
		}
		return nullptr;
	}

private:
	// for this specifically - which cutscene this option is tracking
	Game::DemoFlags cutscene_id;
	bool is_initialised;
	CutsceneMenuOption* option;
};

struct CutsceneMgr {
public:
	CutsceneMgr();

	void init();
	void update();

	CutsceneToggle* get_toggle(Game::DemoFlags id);
	void reset_all();

	static CutsceneMenuOption* create_option(Game::DemoFlags id);

private:
	Vec<CutsceneToggle*> cutscene_list;
};

}; // namespace gz

#endif
