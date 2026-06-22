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

enum EnterAreaKind { PEK_FromCave = 0, PEK_FromMap = 1 };

// how thorough should the treasure-tracking apply with the preset?
// 0=not at all, 1=only things in this cave, 2=all prior to this
enum TreasureMode { TM_Off = 0, TM_CaveFloor = 1, TM_Checkpoint = 2 };

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
	// per-area structure state used during generator cache reconstruction, indexed by CourseIndex (0=VoR, etc)
	struct AreaStructureState {
		AreaStructureState()
		    : plug_destroyed(false)
		{
		}

		bool has_any_state() const;
		bool is_gate_destroyed(const char* name) const;
		bool is_bridge_finished(const char* name) const;
		bool is_bag_flattened(const char* name) const;

		Vec<const char*> destroyed_gates;
		Vec<const char*> finished_bridges;
		Vec<const char*> bags_flattened;
		bool plug_destroyed;
	};

	// enemy (above ground) to be tracked as killed
	struct KilledEnemy {
		KilledEnemy()
		{
			course   = 0xFF; // invalid unless set
			kill_day = -1;   // default = use day from preset
		}

		void read(Stream& input);
		void write(Stream& output);

		u8 course;
		Game::EnemyTypeID::EEnemyTypeID enemy_id;
		Vector3f gen_pos;
		int kill_day; // day enemy (should've been) killed, to set respawn day correctly (-1 = killed on day we're warping to)
	};

	// treasure removed from its fresh state, either collected or carried part-way
	struct CarriedTreasure {
		CarriedTreasure()
		{
			course = 0xFF; // invalid unless set
			id     = 255;  // invalid unless set
			moved  = false;
		}

		void read(Stream& input);
		void write(Stream& output);

		u8 course;
		u8 id;
		bool moved;                 // true = spawn at position_override (moved part-way)
		Vector3f position_override; // only used if moved
	};

	struct HeldPellet {
		HeldPellet()
		    : kind(0)
		    , id(0)
		{
		}
		HeldPellet(u8 kind_, u16 id_)
		    : kind(kind_)
		    , id(id_)
		{
		}

		void read(Stream& input);
		void write(Stream& output);

		u8 kind; // 0=otakara (treasure); 1=item (upgrade)
		u16 id;  // config index
	};

	// store info on what changes treasure-wise between sublevels in a multi-sublevel preset
	struct SublevelDelta {
		SublevelDelta()
		    : sublevel(0)
		    , poko_delta(0)
		{
		}

		void read(Stream& input);
		void write(Stream& output);

		u8 sublevel;            // sublevel these are collected on
		int poko_delta;         // pokos collected on this floor
		Vec<HeldPellet> caught; // treasures collected on this floor
	};

	// snapshot of the collected-treasure state
	struct TreasureState {
		TreasureState()
		    : mode(TM_Off)
		    , debt(-1)
		    , treasure_count(0)
		    , poko_count(0)
		    , cave_poko_count(0)
		{
		}

		void read(Stream& input);
		void write(Stream& output);

		// restore this snapshot onto playData, using 0-indexed floor (to resolve deltas)
		void restore(u8 dest_sublevel);

		TreasureMode mode;         // how much to restore (see TreasureMode)
		s8 debt;                   // -1=dont touch flag; 0=force unpaid; 1=force paid
		Vec<u16> zukan_otakara;    // collected treasure config indices
		Vec<u16> zukan_item;       // collected exploration kit config indices
		Vec<HeldPellet> cave_held; // cave crop memory at the group's "first" floor - deltas added on top
		int treasure_count;
		int poko_count;
		int cave_poko_count;                // cave poko count at the group's "first" floor - deltas added on top
		Vec<SublevelDelta> sublevel_deltas; // per-floor deltas for grouped cave presets (empty otherwise)
	};

	struct HeldPellet {
		HeldPellet()
		    : kind(0)
		    , id(0)
		{
		}
		HeldPellet(u8 kind_, u16 id_)
		    : kind(kind_)
		    , id(id_)
		{
		}

		void read(Stream& input);
		void write(Stream& output);

		u8 kind; // 0=otakara (treasure); 1=item (upgrade)
		u16 id;  // config index
	};

	// store info on what changes treasure-wise between sublevels in a multi-sublevel preset
	struct SublevelDelta {
		SublevelDelta()
		    : sublevel(0)
		    , poko_delta(0)
		{
		}

		void read(Stream& input);
		void write(Stream& output);

		u8 sublevel;            // sublevel these are collected on
		int poko_delta;         // pokos collected on this floor
		Vec<HeldPellet> caught; // treasures collected on this floor
	};

	// snapshot of the collected-treasure state
	struct TreasureState {
		TreasureState()
		    : mode(TM_Off)
		    , debt(-1)
		    , treasure_count(0)
		    , poko_count(0)
		    , cave_poko_count(0)
		{
		}

		void read(Stream& input);
		void write(Stream& output);

		// restore this snapshot onto playData, using 0-indexed floor (to resolve deltas)
		void restore(u8 dest_sublevel);

		TreasureMode mode;         // how much to restore (see TreasureMode)
		s8 debt;                   // -1=dont touch flag; 0=force unpaid; 1=force paid
		Vec<u16> zukan_otakara;    // collected treasure config indices
		Vec<u16> zukan_item;       // collected exploration kit config indices
		Vec<HeldPellet> cave_held; // cave crop memory at the group's "first" floor - deltas added on top
		int treasure_count;
		int poko_count;
		int cave_poko_count;                // cave poko count at the group's "first" floor - deltas added on top
		Vec<SublevelDelta> sublevel_deltas; // per-floor deltas for grouped cave presets (empty otherwise)
	};

	struct Sprout {
	public:
		Sprout()
		{
			pos            = Vector3f::zero;
			stage_and_kind = 0;
			amount         = 0;
		}

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

		void read(Stream& input);
		void write(Stream& output);

		u8 area;
		u8 data; // stage for bridges and gates
		Vector2f position;
	};

public:
	Preset();
	Preset(Preset& other) { GZEXPECT(false, "do not use Preset copy ctor"); }
	~Preset();

	void read_file(const char* filename);
	void read(Stream& input);
	void write(Stream& output);

	void ref() { ref_count += 1; }
	void del();

	void apply();
	void apply_post_load();

	KilledEnemy get_killed_enemy(int course, Game::Generator* gen);
	CarriedTreasure get_treasure_override(int course, int treasure_id, u8 pellet_type = 0);

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
	Vec<StructureOverride> plugs_destroyed;
	EnterAreaKind enter_kind;
	u8 day;
	Vec<KilledEnemy> killed_enemies;
	Vec<CarriedTreasure> carried_treasures;
	TreasureState treasure_state; // collected-treasure snapshot (zukan/crops/counts/pokos)
	bool bridge_glitch_active;
	BitFlag<u16> new_area_zoom;        // bit per course: VoR=1 AW=2 PP=4 WW=8; set = allow zoom on next world-map visit
	bool play_repay_demo;              // tracks whether or not to play a % cutscene
	AreaStructureState area_states[4]; // per-area structure state, indexed by CourseIndex (0=VoR…3=WW)

	bool is_area_visited(int course) const;

private:
	int ref_count;
};

struct PresetMgr {
public:
	PresetMgr();

	void init();

	Preset* create();
	static void fill_current_pikis(Preset* preset);
	static void fill_current_treasures(Preset* preset);
	static void fill_current_treasure_state(Preset* preset, WarpDestination dest);

	PresetPreview* suggested_preset(WarpDestination dest, PresetCategory category);
	PresetPreview* find(const char* name, PresetCategory category);
	Preset* load_preset(PresetPreview*);

	Vec<PresetPreview*> preset_previews;

private:
	CaveIndex which_cave(u32 area, u32 cave);

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
