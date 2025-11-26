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

enum PresetCategory { PoD, AT, General };
enum PresetOrigin { PO_File, PO_Memcard, PO_Generated };

enum EnterAreaKind { PEK_FromCave = 0, PEK_FromMap = 1, PEK_FirstEnter = 2 };

enum GenSpawnOverride { PSO_Ignore = 0, PSO_DontSpawn = 1, PSO_Spawn = 2, PSO_SpawnAndMove = 3 };

struct TreasureAreaMap {
	u8 id;
	u8 course_idx;
};

struct PresetPreview {
public:
	PresetPreview(); // To be used with `read()`
	PresetPreview(PresetCategory category_, const char* name_, Game::PikiContainer squad_, Game::PikiContainer onion_pikis_);

	void read(const char* filename_);

	PresetCategory category;
	const char* name;
	const char* filename;
	Game::PikiContainer squad;
	Game::PikiContainer onion_pikis;
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

		void read(Stream& input);
		void write(Stream& output);

		Game::EnemyTypeID::EEnemyTypeID enemy_id;
		Vector3f gen_pos;
		GenSpawnOverride spawn_override;
	};

	struct TreasureGenSpawnOverride {
		TreasureGenSpawnOverride()
		{
			id                = 255;
			spawn_override    = PSO_Ignore;
			position_override = Vector3f::zero;
		}
		TreasureGenSpawnOverride(u8 id_, GenSpawnOverride spawn_override_);
		TreasureGenSpawnOverride(u8 id_, GenSpawnOverride spawn_override_, Vector3f position_override_);

		void read(Stream& input);
		void write(Stream& output);

		u8 id;
		GenSpawnOverride spawn_override;
		Vector3f position_override; // only used if spawn_override is PSO_SpawnAndMove
	};

	struct Sprout {
	public:
		Sprout()
		{
			pos            = Vector3f::zero;
			stage_and_kind = 0;
			amount         = 0;
		}
		Sprout(Game::EPikiHappa stage, Game::EPikiKind kind, u8 amount_);    // Onion ring (yum)
		Sprout(Vector3f pos_, Game::EPikiHappa stage, Game::EPikiKind kind); // Single in a fixed spot

		void read(Stream& input);
		void write(Stream& output);

		inline u8 get_stage() { return (stage_and_kind & 0xF0) >> 4; }
		inline u8 get_kind() { return stage_and_kind & 0x0F; }

		Vector3f pos;
		/// 0  -> single sprout in a fixed location
		/// >0 -> n sprouts randomly distributed around an onion
		u8 amount;

	private:
		u8 stage_and_kind; // 4bit:stage + 4bit:kind
	};

	struct StructureOverride {
	public:
		StructureOverride();
		StructureOverride(u8 area_, Vector2f position_, u8 data_);

		void read(Stream& input);
		void write(Stream& output);

		u8 area;
		u8 data; // stage for bridges and gates
		Vector2f position;
	};

public:
	Preset();
	Preset(const char* name_, PresetCategory category_);
	Preset(Preset& other);
	~Preset();

	void read_file(const char* filename);
	void read(Stream& input);
	void write(Stream& output);

	void ref() { ref_count += 1; }
	void del();

	void apply();
	void apply_post_load();

	GenSpawnOverride get_enemy_gen_override(Game::Generator* gen);
	GenSpawnOverride get_treasure_gen_override(int treasure_id, u8 pellet_type);

	Preset* set_pikmin(int stage, int color, int amount);
	Preset* set_onion_pikmin(int stage, int color, int amount);
	Preset* set_sprouts(u32 num_sprouts, Sprout sprouts_[]);
	Preset* set_sprays(bool spicies_unlocked_, int spicies, bool bitters_unlocked_, int bitters);
	Preset* set_time(f32 time_);
	Preset* set_cutscene_flags(u32 num_flags, Game::DemoFlags flags[]);
	Preset* set_ek_cutscene_flags(u32 num_flags, Game::OlimarData::ItemIndex flags[]);
	Preset* set_cave_cutscene_flags(u32 num_flags, CaveIndex flags[]);
	Preset* set_upgrades(u32 num_upgrades, Game::OlimarData::ItemIndex items[]);
	Preset* set_destroyed_gates(u32 num_gates, const char* gates[]);
	Preset* set_finished_bridges(u32 num_bridges, const char* bridges[]);
	Preset* set_bags_flattened(u32 num_bags, const char* bags[]);
	Preset* set_plug_destroyed(bool destroyed);
	Preset* set_enter_kind(EnterAreaKind kind);
	Preset* set_pokos(int pokos_);
	Preset* set_day(u8 day_);
	Preset* set_enemy_spawn_overrides(u32 num_spawns, EnemyGenSpawnOverride overrides[]);
	Preset* set_treasure_spawn_overrides(u32 num_spawns, TreasureGenSpawnOverride overrides[]);

	PresetPreview* preview;
	PresetCategory category;
	PresetOrigin origin;
	const char* name;
	Game::PikiContainer squad;
	Game::PikiContainer onion_pikis;
	Vec<Sprout> sprouts;
	bool bitters_unlocked;
	bool spicies_unlocked;
	BitFlag<u16> upgrades;
	u8 num_bitters;
	u8 num_spicies;
	f32 time;
	CutscenesBitfield cutscenes;
	BitFlag<u16> ek_cutscenes;
	BitFlag<u16> cave_cutscenes;
	Vec<StructureOverride> destroyed_gates;
	Vec<StructureOverride> finished_bridges;
	Vec<StructureOverride> bags_flattened;
	EnterAreaKind enter_kind;
	int pokos;
	bool plug_destroyed; // no more than one plug per level
	bool apply_pokos;
	u8 day;
	Vec<EnemyGenSpawnOverride> enemy_spawn_overrides;
	Vec<TreasureGenSpawnOverride> treasure_spawn_overrides;

private:
	int ref_count;
};

struct PresetMgr {
public:
	PresetMgr();

	void init();

	Preset* create();
	static void fill_current_pikis(Preset* preset);

	PresetPreview* suggested_preset(WarpDestination dest, PresetCategory category);
	PresetPreview* find(const char* name, PresetCategory category);
	Preset* load_preset(PresetPreview*);

	Vec<PresetPreview*> preset_previews;

private:
	CaveIndex which_cave(u32 area, u32 cave);

	void init_at_presets();
	void init_at_vor1_presets();
	void init_at_aw1_presets();
	void init_at_vor2_presets();
	void init_at_pp_presets();
	void init_at_aw2_presets();
	void init_at_ww_presets();

	Vec<Preset*> presets;
};

struct PresetMenuOption : public MenuOption {
public:
	PresetMenuOption(IDelegate2<PresetPreview*, int>* on_select_);

	virtual MenuLayer* get_sub_menu() { return preset_category_list; }
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual bool select();

	void init();
	void do_on_preset_selected(PresetPreview*);

	PresetPreview* current_preview;

private:
	void select_current_preset(ListMenu* menu, PresetCategory cat);

	ListMenu* preset_category_list;
	ListMenu* pod_presets_menu;
	ListMenu* at_presets_menu;
	ListMenu* general_presets_menu;

	IDelegate2<PresetPreview*, int>* on_select;
};

struct PresetPreviewMenuOption : public MenuOption {
	PresetPreviewMenuOption(PresetPreview* preset_preview_, PresetMenuOption* parent_);

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual bool select();

	PresetPreview* preset_preview;
	PresetMenuOption* parent;
};

}; // namespace gz

#endif
