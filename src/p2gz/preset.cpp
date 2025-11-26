#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
#include <p2gz/BoundDelegate.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <Game/Entities/ItemPikihead.h>
#include <JSystem/J2D/J2DPrint.h>
#include <System.h>
#include <Game/gameGeneratorCache.h>
#include <Game/Entities/PelletOtakara.h>
#include <Dolphin/rand.h>
#include <p2gz/StructureEditor.h>

using namespace gz;

static const TreasureAreaMap AG_treasure_IDs[] = {
	{ 47, COURSE_VoR },  // fossilized ursidae
	{ 62, COURSE_VoR },  // pink menace
	{ 71, COURSE_VoR },  // unspeakable wonder
	{ 73, COURSE_VoR },  // temporal mechanism
	{ 87, COURSE_VoR },  // utter scrap
	{ 142, COURSE_VoR }, // courage reactor
	{ 157, COURSE_VoR }, // spiny alien treat

	{ 11, COURSE_AW },  // geographic projection (NB: actually an item ID not treasure ID, but 11 is in SCx3 so it's fine)
	{ 42, COURSE_AW },  // sunseed berry
	{ 44, COURSE_AW },  // decorative goo
	{ 130, COURSE_AW }, // pilgrim bulb
	{ 155, COURSE_AW }, // chance totem
	{ 173, COURSE_AW }, // healing cask/hypnotic platter/seat of enlightenment
	{ 185, COURSE_AW }, // air brake

	{ 53, COURSE_PP },  // onion replica
	{ 72, COURSE_PP },  // aquatic mine
	{ 77, COURSE_PP },  // impediment scourge/lightning bolt
	{ 118, COURSE_PP }, // massage girdle
	{ 140, COURSE_PP }, // optical illustration/abstract masterpiece/yell battery
	{ 152, COURSE_PP }, // fortified delicacy
	{ 172, COURSE_PP }, // gherkin gate

	{ 27, COURSE_WW },  // armored nut
	{ 45, COURSE_WW },  // anti-hiccup fungus
	{ 50, COURSE_WW },  // conifer spire
	{ 76, COURSE_WW },  // doomsday apparatus
	{ 183, COURSE_WW }, // seed of greed
};

static const u8 AG_treasure_count = ARRAY_SIZE(AG_treasure_IDs);

PresetPreview::PresetPreview()
{
	name = nullptr;
}

PresetPreview::PresetPreview(PresetCategory category_, const char* name_, Game::PikiContainer squad_, Game::PikiContainer onion_pikis_)
{
	category    = category_;
	name        = name_;
	squad       = squad_;
	onion_pikis = onion_pikis_;
}

Preset::Preset()
    : destroyed_gates(0)
    , finished_bridges(0)
    , bags_flattened(0)
    , enemy_spawn_overrides(0)
    , treasure_spawn_overrides(0)
    , sprouts(0)
    , ref_count(1)
{
	name    = nullptr;
	preview = nullptr;
	squad.clear();
	onion_pikis.clear();
}

Preset::Preset(const char* name_, PresetCategory category_)
    : destroyed_gates(0)
    , finished_bridges(0)
    , bags_flattened(0)
    , enemy_spawn_overrides(0)
    , treasure_spawn_overrides(0)
    , sprouts(0)
    , ref_count(1)
{
	name             = name_;
	category         = category_;
	bitters_unlocked = false;
	spicies_unlocked = false;
	num_bitters      = 0;
	num_spicies      = 0;
	time             = 7.0f;
	plug_destroyed   = false;
	day              = 5;
	preview          = nullptr;

	squad.clear();
	onion_pikis.clear();
}

Preset::Preset(Preset& other)
{
	ref_count = 1;

	name             = other.name;
	category         = other.category;
	bitters_unlocked = other.bitters_unlocked;
	spicies_unlocked = other.spicies_unlocked;
	num_bitters      = other.num_bitters;
	num_spicies      = other.num_spicies;
	squad            = other.squad;
	onion_pikis      = other.onion_pikis;
	time             = other.time;
	day              = other.day;
	apply_pokos      = false;
	pokos            = 0;
	enter_kind       = PEK_FromCave;
	plug_destroyed   = other.plug_destroyed;
	upgrades         = other.upgrades;
	cutscenes        = other.cutscenes;
	ek_cutscenes     = other.ek_cutscenes;
	cave_cutscenes   = other.cave_cutscenes;
	preview          = other.preview;

	destroyed_gates.expandCapacityTo(other.destroyed_gates.len());
	for (u32 i = 0; i < other.destroyed_gates.len(); i++) {
		destroyed_gates.push(other.destroyed_gates[i]);
	}

	finished_bridges.expandCapacityTo(other.finished_bridges.len());
	for (u32 i = 0; i < other.finished_bridges.len(); i++) {
		finished_bridges.push(other.finished_bridges[i]);
	}

	bags_flattened.expandCapacityTo(other.bags_flattened.len());
	for (u32 i = 0; i < other.bags_flattened.len(); i++) {
		bags_flattened.push(other.bags_flattened[i]);
	}

	enemy_spawn_overrides.expandCapacityTo(other.enemy_spawn_overrides.len());
	for (u32 i = 0; i < other.enemy_spawn_overrides.len(); i++) {
		enemy_spawn_overrides.push(other.enemy_spawn_overrides[i]);
	}

	treasure_spawn_overrides.expandCapacityTo(other.treasure_spawn_overrides.len());
	for (u32 i = 0; i < other.treasure_spawn_overrides.len(); i++) {
		treasure_spawn_overrides.push(other.treasure_spawn_overrides[i]);
	}

	sprouts.expandCapacityTo(other.sprouts.len());
	for (u32 i = 0; i < other.sprouts.len(); i++) {
		sprouts.push(other.sprouts[i]);
	}
}

Preset::~Preset()
{
	delete name;
	preview = nullptr;
	name    = nullptr;
}

void Preset::del()
{
	ref_count -= 1;
	if (ref_count <= 0) {
		delete this;
	}
}

Preset::EnemyGenSpawnOverride::EnemyGenSpawnOverride(Game::EnemyTypeID::EEnemyTypeID enemy_id_, Vector3f gen_pos_,
                                                     GenSpawnOverride spawn_override_)
{
	enemy_id       = enemy_id_;
	gen_pos        = gen_pos_;
	spawn_override = spawn_override_;
}

Preset::TreasureGenSpawnOverride::TreasureGenSpawnOverride(u8 id_, GenSpawnOverride spawn_override_)
{
	id             = id_;
	spawn_override = spawn_override_;
}

Preset::TreasureGenSpawnOverride::TreasureGenSpawnOverride(u8 id_, GenSpawnOverride spawn_override_, Vector3f position_override_)
{
	GZASSERTLINE(spawn_override_ == PSO_SpawnAndMove); // Doesn't make sense to use this ctor otherwise
	id                = id_;
	spawn_override    = spawn_override_;
	position_override = position_override_;
}

// Onion ring ctor
Preset::Sprout::Sprout(Game::EPikiHappa stage, Game::EPikiKind kind, u8 amount_)
{
	pos            = Vector3f::zero;
	amount         = amount_;
	stage_and_kind = ((static_cast<u8>(stage) & 0x0F) << 4) | (static_cast<u8>(kind) & 0x0F);
}

// Single sprout in a fixed spot ctor
Preset::Sprout::Sprout(Vector3f pos_, Game::EPikiHappa stage, Game::EPikiKind kind)
{
	pos            = pos_;
	amount         = 0;
	stage_and_kind = ((static_cast<u8>(stage) & 0x0F) << 4) | (static_cast<u8>(kind) & 0x0F);
}

Preset::StructureOverride::StructureOverride()
{
	area = 0xFF;
	data = 0xFF;
}

Preset::StructureOverride::StructureOverride(u8 area_, Vector2f position_, u8 data_)
{
	area     = area_;
	data     = data_;
	position = position_;
}

GenSpawnOverride Preset::get_enemy_gen_override(Game::Generator* gen)
{
	if (gen->mObject->mTypeID == 'teki') {
		Game::GenObjectEnemy* gen_obj_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
		for (u32 i = 0; i < enemy_spawn_overrides.len(); i++) {
			EnemyGenSpawnOverride& oride = enemy_spawn_overrides[i];
			if (oride.enemy_id == gen_obj_enemy->mEnemyID && absF(oride.gen_pos.sqrDistance(gen->mPosition)) < 25.0f) {
				return oride.spawn_override;
			}
		}
	}
	return PSO_Ignore;
}

GenSpawnOverride Preset::get_treasure_gen_override(int treasure_id, u8 pellet_type)
{
	for (u32 i = 0; i < treasure_spawn_overrides.len(); i++) {
		TreasureGenSpawnOverride& oride = treasure_spawn_overrides[i];
		if (treasure_id == oride.id) {
			return oride.spawn_override;
		}
	}
	return PSO_Ignore;
}

Preset* Preset::set_pikmin(int stage, int color, int amount)
{
	squad.getCount(color, stage) = amount;
	return this;
}

Preset* Preset::set_onion_pikmin(int stage, int color, int amount)
{
	onion_pikis.getCount(color, stage) = amount;
	return this;
}

Preset* Preset::set_sprouts(u32 num_sprouts, Sprout sprouts_[])
{
	sprouts.expandCapacityTo(num_sprouts);
	for (u32 i = 0; i < num_sprouts; i++) {
		sprouts.push(sprouts_[i]);
	}
	return this;
}

Preset* Preset::set_sprays(bool spicies_unlocked_, int spicies, bool bitters_unlocked_, int bitters)
{
	spicies_unlocked = spicies_unlocked_;
	bitters_unlocked = bitters_unlocked_;
	num_spicies      = spicies;
	num_bitters      = bitters;
	return this;
}

Preset* Preset::set_time(f32 time_)
{
	time = time_;
	return this;
}

Preset* Preset::set_cutscene_flags(u32 num_flags, Game::DemoFlags flags[])
{
	for (u32 i = 0; i < num_flags; i++) {
		cutscenes.set_cutscene_played(flags[i]);
	}
	return this;
}

Preset* Preset::set_ek_cutscene_flags(u32 num_flags, Game::OlimarData::ItemIndex flags[])
{
	for (u32 i = 0; i < num_flags; i++) {
		const u16 bit = 1 << flags[i];
		ek_cutscenes.set(bit);
	}
	return this;
}

Preset* Preset::set_cave_cutscene_flags(u32 num_flags, CaveIndex flags[])
{
	for (u32 i = 0; i < num_flags; i++) {
		const u16 bit = 1 << flags[i];
		cave_cutscenes.set(bit);
	}
	return this;
}

Preset* Preset::set_upgrades(u32 num_upgrades, Game::OlimarData::ItemIndex items[])
{
	for (u32 i = 0; i < num_upgrades; i++) {
		const u16 bit = 1 << items[i];
		upgrades.set(bit);
	}
	return this;
}

Preset* Preset::set_destroyed_gates(u32 num_gates, const char* gates[])
{
	static const StructureEditor::NameCoordinateMap GATE_COORD_TO_NAME[] = {
		// VoR
		StructureEditor::NameCoordinateMap(-591.0f, 1427.0f, 0, "EC gate"),
		StructureEditor::NameCoordinateMap(369.0f, 1565.0f, 0, "water gate"),
		StructureEditor::NameCoordinateMap(798.0f, -520.0f, 0, "SCx gate"),
		// AW
		StructureEditor::NameCoordinateMap(-503.0f, 1639.0f, 1, "black gate"),
		StructureEditor::NameCoordinateMap(450.0f, 1905.0f, 1, "plug gate"),
		StructureEditor::NameCoordinateMap(-1463.0f, 1767.0f, 1, "globe gate"),
		StructureEditor::NameCoordinateMap(460.0f, 994.0f, 1, "WFG gate"),
		StructureEditor::NameCoordinateMap(470.0f, 1575.0f, 1, "BK gate"),
		StructureEditor::NameCoordinateMap(-375.0f, 375.0f, 1, "blue onion gate"),
		StructureEditor::NameCoordinateMap(-820.0f, 4280.0f, 1, "SH gate"),
		// PP
		StructureEditor::NameCoordinateMap(-1180.0f, 980.0f, 2, "onion replica gate"),
		StructureEditor::NameCoordinateMap(-1510.0f, -2550.0f, 2, "SMC area gate"),
		StructureEditor::NameCoordinateMap(-390.0f, -1290.0f, 2, "yellow onion gate"),
		StructureEditor::NameCoordinateMap(-1162.0f, -2375.0f, 2, "GK gate"),
		StructureEditor::NameCoordinateMap(-1612.0f, -1798.0f, 2, "zirconium rotor gate"),
		StructureEditor::NameCoordinateMap(1390.0f, 1083.0f, 2, "SR gate"),
		// WW
		StructureEditor::NameCoordinateMap(-350.0f, 1025.0f, 3, "DD gate"),
		StructureEditor::NameCoordinateMap(-1220.0f, 727.0f, 3, "bridge gate"),
		StructureEditor::NameCoordinateMap(-2819.0f, 2463.0f, 3, "HoH gate"),
		StructureEditor::NameCoordinateMap(-3514.0f, 1153.0f, 3, "crawmad gate"),
	};

	destroyed_gates.expandCapacityTo(destroyed_gates.len() + num_gates);
	for (u32 i = 0; i < num_gates; i++) {
		GZASSERTLINE(gates[i]);
		StructureOverride oride;
		oride.data = 0;
		for (u32 j = 0; j < ARRAY_SIZE(GATE_COORD_TO_NAME); j++) {
			if (strcmp(GATE_COORD_TO_NAME[j].name, gates[i]) == 0) {
				oride.area     = GATE_COORD_TO_NAME[j].area;
				oride.position = Vector2f(GATE_COORD_TO_NAME[j].x, GATE_COORD_TO_NAME[j].z);
				destroyed_gates.push(oride);
				break;
			}
		}
	}
	return this;
}

Preset* Preset::set_finished_bridges(u32 num_bridges, const char* bridges[])
{
	static const StructureEditor::NameCoordinateMap BRIDGE_COORD_TO_NAME[] = {
		// VoR
		StructureEditor::NameCoordinateMap(540.0f, 775.0f, 0, "water bridge"),
		StructureEditor::NameCoordinateMap(-316.8f, -1664.5f, 0, "FC bridge"),
		// AW
		StructureEditor::NameCoordinateMap(-1250.0f, 2260.0f, 1, "globe bridge"),
		StructureEditor::NameCoordinateMap(-1517.3f, 3529.5f, 1, "air brake bridge to globe"),
		StructureEditor::NameCoordinateMap(-1353.2f, 3760.6f, 1, "air brake bridge to SH"),
		// PP
		StructureEditor::NameCoordinateMap(326.0f, -759.0f, 2, "bitter plant bridge"),
		StructureEditor::NameCoordinateMap(1431.0f, 404.0f, 2, "SR bridge"),
		StructureEditor::NameCoordinateMap(-2159.0f, -857.0f, 2, "CoS bridge"),
		// WW
		StructureEditor::NameCoordinateMap(-1479.3f, 558.3f, 3, "DD bridge"),
	};

	finished_bridges.expandCapacityTo(destroyed_gates.len() + num_bridges);
	for (u32 i = 0; i < num_bridges; i++) {
		GZASSERTLINE(bridges[i]);
		StructureOverride oride;
		oride.data = 0;
		for (u32 j = 0; j < ARRAY_SIZE(BRIDGE_COORD_TO_NAME); j++) {
			if (strcmp(BRIDGE_COORD_TO_NAME[j].name, bridges[i]) == 0) {
				oride.area     = BRIDGE_COORD_TO_NAME[j].area;
				oride.position = Vector2f(BRIDGE_COORD_TO_NAME[j].x, BRIDGE_COORD_TO_NAME[j].z);
				finished_bridges.push(oride);
				break;
			}
		}
	}
	return this;
}

Preset* Preset::set_bags_flattened(u32 num_bags, const char* bags[])
{
	static const StructureEditor::NameCoordinateMap BAG_COORD_TO_NAME[] = {
		// VoR
		StructureEditor::NameCoordinateMap(-910.7f, 2769.2f, 0, "landing area bag (15)"),
		StructureEditor::NameCoordinateMap(-1150.0f, 2455.0f, 0, "hubcap bag (35)"),
		// AW
		StructureEditor::NameCoordinateMap(-395.0f, 1115.0f, 1, "WFG bag (200)"),
	};

	bags_flattened.expandCapacityTo(bags_flattened.len() + num_bags);
	for (u32 i = 0; i < num_bags; i++) {
		GZASSERTLINE(bags[i]);
		StructureOverride oride;

		oride.data = 0;
		for (u32 j = 0; j < ARRAY_SIZE(BAG_COORD_TO_NAME); j++) {
			if (strcmp(BAG_COORD_TO_NAME[j].name, bags[i]) == 0) {
				oride.area     = BAG_COORD_TO_NAME[j].area;
				oride.position = Vector2f(BAG_COORD_TO_NAME[j].x, BAG_COORD_TO_NAME[j].z);
				bags_flattened.push(oride);
				break;
			}
		}
	}
	return this;
}

Preset* Preset::set_plug_destroyed(bool destroyed)
{
	plug_destroyed = destroyed;
	return this;
}

Preset* Preset::set_enter_kind(EnterAreaKind kind)
{
	enter_kind = kind;
	return this;
}

Preset* Preset::set_pokos(int pokos_)
{
	pokos       = pokos_;
	apply_pokos = true;
	return this;
}

Preset* Preset::set_day(u8 day_)
{
	day = day_;
}

Preset* Preset::set_enemy_spawn_overrides(u32 num_spawns, EnemyGenSpawnOverride overrides[])
{
	enemy_spawn_overrides.expandCapacityTo(enemy_spawn_overrides.len() + num_spawns);
	for (u32 i = 0; i < num_spawns; i++) {
		enemy_spawn_overrides.push(overrides[i]);
	}
	return this;
}

Preset* Preset::set_treasure_spawn_overrides(u32 num_spawns, TreasureGenSpawnOverride overrides[])
{
	treasure_spawn_overrides.expandCapacityTo(treasure_spawn_overrides.len() + num_spawns);
	for (u32 i = 0; i < num_spawns; i++) {
		treasure_spawn_overrides.push(overrides[i]);
	}
	return this;
}

Game::ItemPikihead::Item* birth_sprout(u8 kind, u8 stage)
{
	Game::ItemPikihead::Item* new_sprout = Game::ItemPikihead::mgr->birth();
	GZASSERTLINE(new_sprout);

	Game::ItemPikihead::InitArg new_sprout_arg(static_cast<Game::EPikiKind>(kind), Vector3f::zero, true, stage, 0.0f);
	new_sprout->init(&new_sprout_arg);
	return new_sprout;
}

void Preset::apply()
{
	if (Game::naviMgr && Game::naviMgr->mArray) {
		for (u32 i = 0; i < 2; i++) {
			Game::Navi* navi = Game::naviMgr->getAt(i);
			if (navi && navi->isAlive() && navi->isStickTo()) {
				navi->endStick();
			}
		}
	}

	p2gz->day_editor->set_time(time);
	p2gz->squad_editor->clear_all_pikmin();
	Game::playData->resetContainerFlag();                     // Reset container flags for onions/ship space unlocks
	p2gz->squad_editor->birth_piki(Game::Red, Game::Leaf, 0); // set red onion container flag since it's pretty much always expected

	// Apply squad
	for (u32 color = 0; color < 6; color++) {
		for (u32 stage = 0; stage < 3; stage++) {
			int amount       = squad.getCount(color, stage);
			int onion_amount = 0;
			if (color < 5) { // no bulbmin onion
				onion_amount = onion_pikis.getCount(color, stage);
			}
			if (amount > 0 || onion_amount > 0) {
				// If warping from a menu, birthing the pikis will fail but we need to set their demo flags either way
				p2gz->squad_editor->set_demo_flags_for_color(static_cast<Game::EPikiKind>(color));
				p2gz->squad_editor->birth_piki(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage), amount);
			}
			if (amount > 0) {
				Game::playData->mCaveSaveData.mCavePikis.getCount(color, stage) += amount;
			}
		}
	}

	// Apply onion pikmin
	Game::playData->mPikiContainer = onion_pikis;

	// Apply sprays
	p2gz->spray_editor->set_bitters(num_bitters);
	p2gz->spray_editor->set_spicies(num_spicies);
	p2gz->spray_editor->toggle_bitters(bitters_unlocked);
	p2gz->spray_editor->toggle_spicies(spicies_unlocked);

	// Apply upgrades
	for (u32 i = Game::OlimarData::ODII_FIRST_EXPLORATION_KIT_ITEM; i <= Game::OlimarData::ODII_LAST_EXPLORATION_KIT_ITEM; i++) {
		const u16 mask = 1 << i;
		p2gz->ek_editor->set_upgrade(static_cast<Game::OlimarData::ItemIndex>(i), upgrades.isSet(mask));
	}

	// Set cutscene flags
	p2gz->cutscene_mgr->reset_all();
	// set regular cutscenes
	for (u32 i = 0; i < Game::DEMO_FLAG_COUNT; i++) {
		const Game::DemoFlags flag = static_cast<Game::DemoFlags>(i);
		if (cutscenes.cutscene_played(flag)) {
			CutsceneToggle* cutscene_toggle = p2gz->cutscene_mgr->get_toggle(flag);
			if (cutscene_toggle) {
				cutscene_toggle->set_cutscene_flag(true);
			}
		}
	}
	// set exploration kit discovery cutscenes
	for (u32 i = 0; i < Game::OlimarData::ODII_COUNT; i++) {
		const Game::OlimarData::ItemIndex flag = static_cast<Game::OlimarData::ItemIndex>(i);
		const u16 mask                         = 1 << i;
		if (ek_cutscenes.isSet(mask)) {
			EKCutsceneToggle* cutscene_toggle = p2gz->cutscene_mgr->get_ek_toggle(flag);
			if (cutscene_toggle) {
				cutscene_toggle->set_cutscene_flag(true);
			}
		}
	}
	// set cave discovery cutscenes
	// (from 1 because we can't "discover" above-ground)
	for (u32 i = 1; i < CAVE_COUNT; i++) {
		const CaveIndex flag = static_cast<CaveIndex>(i);
		const u16 mask       = 1 << i;
		if (cave_cutscenes.isSet(mask)) {
			CaveCutsceneToggle* cutscene_toggle = p2gz->cutscene_mgr->get_cave_toggle(flag);
			if (cutscene_toggle) {
				cutscene_toggle->set_cutscene_flag(true);
			}
		}
	}

	// calc treasure counts for areas
	u8 treasure_counts[4];
	treasure_counts[COURSE_VoR] = treasure_counts[COURSE_AW] = treasure_counts[COURSE_PP] = treasure_counts[COURSE_WW] = 0;

	for (u32 i = 0; i < treasure_spawn_overrides.len(); i++) {
		// only interested in treasures we've "collected"
		if (treasure_spawn_overrides[i].spawn_override != PSO_DontSpawn) {
			continue;
		}

		u32 id = treasure_spawn_overrides[i].id;
		for (u8 i = 0; i < AG_treasure_count; i++) {
			if (AG_treasure_IDs[i].id == id) {
				treasure_counts[AG_treasure_IDs[i].course_idx]++;
				break;
			}
		}
	}

	// set treasure counts
	for (u32 i = 0; i < 4; i++) {
		Game::playData->mGroundOtakaraCollected[i]    = treasure_counts[i];
		Game::playData->mGroundOtakaraCollectedOld[i] = treasure_counts[i];
	}

	p2gz->warp->set_enter_area_type(enter_kind);

	if (apply_pokos) {
		p2gz->poko_editor->set_pokos(pokos);
	}
}

void Preset::apply_post_load()
{
	p2gz->structure_editor->reset_all_structures();

	for (u32 i = 0; i < destroyed_gates.len(); i++) {
		p2gz->structure_editor->set_gate_stages_left(destroyed_gates[i].position, destroyed_gates[i].data);
	}
	for (u32 i = 0; i < finished_bridges.len(); i++) {
		p2gz->structure_editor->set_bridge_stages_left(finished_bridges[i].position, finished_bridges[i].data);
	}
	for (u32 i = 0; i < bags_flattened.len(); i++) {
		p2gz->structure_editor->set_bag_flattened(bags_flattened[i].position, true);
	}
	if (plug_destroyed) {
		p2gz->structure_editor->set_plug_destroyed(true);
	}

	// Apply sprouts
	if (!Game::playData->mCaveSaveData.mIsInCave) {
		for (u32 i = 0; i < sprouts.len(); i++) {
			Sprout sprout = sprouts[i];
			if (sprout.amount == 0) {
				Game::ItemPikihead::Item* new_sprout = birth_sprout(sprout.get_kind(), sprout.get_stage());
				new_sprout->setPosition(sprout.pos, false);
			} else {
				// randomly disperse around the base of an onion
				static const f32 ONION_DIST = 65.8767f; // Determined by printing in ItemPikihead::BuryState::init

				Game::Onyon* onion = Game::ItemOnyon::mgr->getOnyon(sprout.get_kind());
				if (!onion) {
					OSReport("No onion of kind %d found for preset application. Skipping\n");
					continue;
				}
				const Vector3f onion_pos = onion->getPosition();

				for (u32 num_sprouts = 0; num_sprouts < sprout.amount; num_sprouts++) {
					Game::ItemPikihead::Item* new_sprout = birth_sprout(sprout.get_kind(), sprout.get_stage());
					const f32 angle                      = randFloat() * TAU;
					Vector3f sprout_pos                  = onion_pos + Vector3f(sinf(angle) * ONION_DIST, 0.0f, cosf(angle) * ONION_DIST);
					new_sprout->setPosition(sprout_pos, false);
				}
			}
		}
	}

	// Force spawn or despawn treasures
	FOREACH_NODE(Game::Generator, Game::generatorCache->getFirstGenerator(), gen)
	{
		if (gen->mObject->mTypeID == 'pelt') {
			Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(gen->mObject);
			int treasure_id             = gen_pellet->mGenParm->mIndex;
			int kind                    = gen_pellet->mPelType;
			for (u32 i = 0; i < treasure_spawn_overrides.len(); i++) {
				TreasureGenSpawnOverride& oride = treasure_spawn_overrides[i];
				if (oride.id == treasure_id) {
					if (oride.spawn_override >= PSO_Spawn) {
						if (!gen->mCreature) {
							gen->generate();
						}
						GZASSERTLINE(gen->mCreature);
						if (oride.spawn_override == PSO_SpawnAndMove) {
							gen->mCreature->setPosition(oride.position_override, false);
						} else {
							gen->mCreature->setPosition(gen->mPosition, false);
						}
					} else if (oride.spawn_override == PSO_DontSpawn && gen->mCreature) {
						Game::PelletKillArg arg;
						gen->mCreature->kill(&arg);
					}
				}
			}
		}
	}
}
