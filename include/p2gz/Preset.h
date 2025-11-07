#ifndef _GZ_PRESET_H
#define _GZ_PRESET_H

#include <IDelegate.h>
#include <p2gz/gzCollections.h>
#include <p2gz/gzmenu.h>
#include <p2gz/warp.h>
#include <types.h>
#include <Game/PikiContainer.h>
#include <JSystem/J2D/J2DPrint.h>

namespace gz {

struct Warp;
struct WarpDestination;

enum PresetCategory { PoD, AT, General, Generated };

enum EnterAreaKind { FromCave = 0, FromMap = 1, FirstEnter = 2 };

struct Preset {
public:
	Preset(const char* name_, PresetCategory category_);
	Preset(Preset& other);
	~Preset() { }

	void apply();
	void apply_post_load();

	Preset* set_pikmin(int stage, int color, int amount);
	Preset* set_onion_pikmin(int stage, int color, int amount);
	Preset* set_sprays(bool spicies_unlocked_, int spicies, bool bitters_unlocked_, int bitters);
	Preset* set_time(f32 time_);
	Preset* set_cutscene_flags(size_t num_flags, Game::DemoFlags flags[]);
	Preset* set_upgrades(size_t num_upgrades, Game::OlimarData::ItemIndex items[]);
	Preset* set_destroyed_gates(size_t num_gates, const char* gates[]);
	Preset* set_enter_kind(EnterAreaKind kind);
	Preset* set_pokos(int pokos_);

	PresetCategory category;
	const char* name;
	Game::PikiContainer squad;
	Game::PikiContainer onion_pikis;
	bool bitters_unlocked;
	bool spicies_unlocked;
	int num_bitters;
	int num_spicies;
	f32 time;
	Vec<Game::DemoFlags> cutscene_flags;
	Vec<Game::OlimarData::ItemIndex> upgrades;
	Vec<const char*> destroyed_gates; // Will we need a way to store stages?
	EnterAreaKind enter_kind;
	bool apply_pokos;
	int pokos;
};

struct PresetMgr {
public:
	PresetMgr();

	Preset* create();

	Preset* suggested_preset(WarpDestination dest, PresetCategory category);
	Preset* find(const char* name, PresetCategory category);

	Vec<Preset*> presets;
	Preset* last_used_preset;
};

struct PresetMenuOption : public MenuOption {
public:
	PresetMenuOption(IDelegate2<Preset*, int>* on_select_);

	virtual MenuLayer* get_sub_menu() { return preset_category_list; }
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void select();
	void do_on_preset_selected(Preset*);

	Preset* current_preset;

private:
	void select_current_preset(ListMenu* menu, PresetCategory cat);

	ListMenu* preset_category_list;
	ListMenu* pod_presets_menu;
	ListMenu* at_presets_menu;
	ListMenu* general_presets_menu;

	IDelegate2<Preset*, int>* on_select;
};

struct PresetPreviewMenuOption : public MenuOption {
	PresetPreviewMenuOption(Preset* preset_, PresetMenuOption* parent_);

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void select();

	Preset* preset;
	PresetMenuOption* parent;
};

}; // namespace gz

#endif
