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

#define EK_MENU_TITLE "exploration kit"

#define CAVE_MENU_TITLE "cave discovery"

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

/// Structure for elements of the smaller exploration kit discovery cutscene table
struct EKCutsceneMap {
	EKCutsceneMap(Game::OlimarData::ItemIndex idx_, const char* name_, const char* submenu_name_)
	    : idx(idx_)
	    , name(name_)
	    , submenu_name(submenu_name_)
	{
	}

	static const char* get_name_from_idx(Game::OlimarData::ItemIndex idx_);
	static const char* get_menu_from_idx(Game::OlimarData::ItemIndex idx_);

	bool is(const char* name_) { return strcmp(name, name_) == 0; }

	const Game::OlimarData::ItemIndex idx;
	const char* name;
	const char* submenu_name;
};

/// Structure for elements of the smaller cave discovery cutscene table
struct CaveCutsceneMap {
	CaveCutsceneMap(CaveIndex cave_, const char* name_, const char* submenu_name_)
	    : idx(cave_)
	    , name(name_)
	    , submenu_name(submenu_name_)
	{
	}

	static const char* get_name_from_idx(CaveIndex cave_);
	static const char* get_menu_from_idx(CaveIndex cave_);

	bool is(const char* name_) { return strcmp(name, name_) == 0; }

	const CaveIndex idx;
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

struct EKCutsceneToggle {
public:
	EKCutsceneToggle(Game::OlimarData::ItemIndex id_)
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

	Game::OlimarData::ItemIndex get_cutscene_id() { return cutscene_id; };
	CutsceneMenuOption* get_option()
	{
		if (is_initialised) {
			return option;
		}
		return nullptr;
	}

private:
	// for this specifically - which cutscene this option is tracking
	Game::OlimarData::ItemIndex cutscene_id;
	bool is_initialised;
	CutsceneMenuOption* option;
};

struct CaveCutsceneToggle {
public:
	CaveCutsceneToggle(CaveIndex id_)
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

	CaveIndex get_cutscene_id() { return cutscene_id; };
	CutsceneMenuOption* get_option()
	{
		if (is_initialised) {
			return option;
		}
		return nullptr;
	}

private:
	// for this specifically - which cutscene this option is tracking
	CaveIndex cutscene_id;
	bool is_initialised;
	CutsceneMenuOption* option;
};

struct CutscenesBitfield {
public:
	CutscenesBitfield()
	{
		bf1 = 0;
		bf2 = 0;
	}
	CutscenesBitfield(CutscenesBitfield& other)
	{
		bf1 = other.bf1;
		bf2 = other.bf2;
	}

	bool cutscene_played(Game::DemoFlags flag)
	{
		if (flag < 32) {
			return bf1 & (1 << flag);
		} else {
			return bf2 & (1 << (flag - 32));
		}
	}

	void set_cutscene_played(Game::DemoFlags flag)
	{
		if (flag < 32) {
			bf1 |= (1 << flag);
		} else {
			bf2 |= (1 << (flag - 32));
		}
	}

private:
	u32 bf1;
	u32 bf2;
};

struct CutsceneMgr {
public:
	CutsceneMgr();

	void init();
	void update();

	CutsceneToggle* get_toggle(Game::DemoFlags id);
	EKCutsceneToggle* get_ek_toggle(Game::OlimarData::ItemIndex id);
	CaveCutsceneToggle* get_cave_toggle(CaveIndex id);
	CutscenesBitfield get_cur_cutscenes();
	BitFlag<u16> get_cur_ek_cutscenes();
	BitFlag<u16> get_cur_cave_cutscenes();
	void reset_all();

	static CutsceneMenuOption* create_option(Game::DemoFlags id);
	static CutsceneMenuOption* create_ek_option(Game::OlimarData::ItemIndex id);
	static CutsceneMenuOption* create_cave_option(CaveIndex id);

private:
	Vec<CutsceneToggle*> cutscene_list;
	Vec<EKCutsceneToggle*> ek_cutscene_list;
	Vec<CaveCutsceneToggle*> cave_cutscene_list;
};

}; // namespace gz

#endif
