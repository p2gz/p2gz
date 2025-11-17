#ifndef _GZ_PRESET_H
#define _GZ_PRESET_H

#include <BitFlag.h>
#include <IDelegate.h>
#include <p2gz/gzCollections.h>
#include <p2gz/gzmenu.h>
#include <p2gz/warp.h>
#include <p2gz/CutsceneToggle.h>
#include <types.h>
#include <Game/PikiContainer.h>
#include <JSystem/J2D/J2DPrint.h>

namespace gz {

struct Warp;
struct WarpDestination;

enum PresetCategory { PoD, AT, General, Generated };

enum EnterAreaKind { PEK_FromCave = 0, PEK_FromMap = 1, PEK_FirstEnter = 2 };

enum GenSpawnOverride { PSO_Ignore = 0, PSO_DontSpawn = 1, PSO_Spawn = 2, PSO_SpawnAndMove = 3 };

enum CourseIndex { COURSE_VoR, COURSE_AW, COURSE_PP, COURSE_WW };

struct TreasureAreaMap {
	u8 id;
	u8 course_idx;
};

struct Preset {
	struct EnemyGenSpawnOverride {
		EnemyGenSpawnOverride()
		{
			enemy_id       = Game::EnemyTypeID::EnemyID_Armor;
			gen_pos        = Vector3f::zero;
			spawn_override = PSO_Ignore;
		}
		EnemyGenSpawnOverride(Game::EnemyTypeID::EEnemyTypeID enemy_id_, Vector3f gen_pos_, GenSpawnOverride spawn_override_);

		Game::EnemyTypeID::EEnemyTypeID enemy_id;
		Vector3f gen_pos;
		GenSpawnOverride spawn_override;
	};

	struct TreasureGenSpawnOverride {
		TreasureGenSpawnOverride()
		{
			id             = 255;
			spawn_override = PSO_Ignore;
			position_override = Vector3f::zero;
		}
		TreasureGenSpawnOverride(u8 id_, GenSpawnOverride spawn_override_);
		TreasureGenSpawnOverride(u8 id_, GenSpawnOverride spawn_override_, Vector3f position_override_);

		u8 id;
		GenSpawnOverride spawn_override;
		Vector3f position_override; // only used if spawn_override is PSO_SpawnAndMove
	};

public:
	Preset(const char* name_, PresetCategory category_);
	Preset(Preset& other);
	~Preset() { }

	void apply();
	void apply_post_load();

	GenSpawnOverride get_enemy_gen_override(Game::Generator* gen);
	GenSpawnOverride get_treasure_gen_override(int treasure_id, u8 pellet_type);

	Preset* set_pikmin(int stage, int color, int amount);
	Preset* set_onion_pikmin(int stage, int color, int amount);
	Preset* set_sprays(bool spicies_unlocked_, int spicies, bool bitters_unlocked_, int bitters);
	Preset* set_time(f32 time_);
	Preset* set_cutscene_flags(size_t num_flags, Game::DemoFlags flags[]);
	Preset* set_upgrades(size_t num_upgrades, Game::OlimarData::ItemIndex items[]);
	Preset* set_destroyed_gates(size_t num_gates, const char* gates[]);
	Preset* set_finished_bridges(size_t num_bridges, const char* bridges[]);
	Preset* set_bags_flattened(size_t num_bags, const char* bags[]);
	Preset* set_plug_destroyed(bool destroyed);
	Preset* set_enter_kind(EnterAreaKind kind);
	Preset* set_pokos(int pokos_);
	Preset* set_day(u8 day_);
	Preset* set_enemy_spawn_overrides(size_t num_spawns, EnemyGenSpawnOverride overrides[]);
	Preset* set_treasure_spawn_overrides(size_t num_spawns, TreasureGenSpawnOverride overrides[]);

	PresetCategory category;
	const char* name;
	Game::PikiContainer squad;
	Game::PikiContainer onion_pikis;
	bool bitters_unlocked;
	bool spicies_unlocked;
	BitFlag<u16> upgrades;
	u8 num_bitters;
	u8 num_spicies;
	f32 time;
	CutscenesBitfield cutscenes;
	Vec<const char*> destroyed_gates;
	Vec<const char*> finished_bridges;
	Vec<const char*> bags_flattened;
	EnterAreaKind enter_kind;
	int pokos;
	bool plug_destroyed; // no more than one plug per level
	bool apply_pokos;
	u8 day;
	Vec<EnemyGenSpawnOverride> enemy_spawn_overrides;
	Vec<TreasureGenSpawnOverride> treasure_spawn_overrides;
};

struct PresetMgr {
public:
	PresetMgr();

	Preset* create();
	static void fill_current_pikis(Preset* preset);

	Preset* suggested_preset(WarpDestination dest, PresetCategory category);
	Preset* find(const char* name, PresetCategory category);

	Vec<Preset*> presets;
};

struct PresetMenuOption : public MenuOption {
public:
	PresetMenuOption(IDelegate2<Preset*, int>* on_select_);

	virtual MenuLayer* get_sub_menu() { return preset_category_list; }
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual bool select();
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
	virtual bool select();

	Preset* preset;
	PresetMenuOption* parent;
};

}; // namespace gz

#endif
