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
{
	ref_count            = 1;
	name    = nullptr;
	preview = nullptr;
	bridge_glitch_active = true;
	squad.clear();
	onion_pikis.clear();
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

Preset::StructureOverride::StructureOverride()
{
	area = 0xFF;
	data = 0xFF;
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
	p2gz->warp->set_warp_day(day);
	Game::playData->resetContainerFlag(); // Reset container flags for onions/ship space unlocks

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
	p2gz->structure_editor->set_bridge_glitch(bridge_glitch_active);
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
