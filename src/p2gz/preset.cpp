#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
#include <p2gz/BoundDelegate.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <Game/gamePlayData.h>
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
	name                 = nullptr;
	preview              = nullptr;
	bridge_glitch_active = true;
	category             = PoD;   // default to pod so we don't get errors for null presets
	time                 = 7.0f;  // default to start of day
	play_repay_demo      = false; // default to no percent cutscene
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

bool Preset::AreaStructureState::has_any_state() const
{
	return destroyed_gates.len() > 0 || finished_bridges.len() > 0 || bags_flattened.len() > 0 || plug_destroyed;
}

bool Preset::AreaStructureState::is_gate_destroyed(const char* name) const
{
	for (u32 i = 0; i < destroyed_gates.len(); i++) {
		if (strcmp(destroyed_gates[i], name) == 0) {
			return true;
		}
	}
	return false;
}

bool Preset::AreaStructureState::is_bridge_finished(const char* name) const
{
	for (u32 i = 0; i < finished_bridges.len(); i++) {
		if (strcmp(finished_bridges[i], name) == 0) {
			return true;
		}
	}
	return false;
}

bool Preset::AreaStructureState::is_bag_flattened(const char* name) const
{
	for (u32 i = 0; i < bags_flattened.len(); i++) {
		if (strcmp(bags_flattened[i], name) == 0) {
			return true;
		}
	}
	return false;
}

Preset::KilledEnemy Preset::get_killed_enemy(int course, Game::Generator* gen)
{
	if (gen->mObject->mTypeID == 'teki') {
		Game::GenObjectEnemy* gen_obj_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
		for (u32 i = 0; i < killed_enemies.len(); i++) {
			KilledEnemy& kill = killed_enemies[i];
			if (kill.course != course) {
				continue; // belongs to a different area
			}
			if (kill.enemy_id == gen_obj_enemy->mEnemyID && kill.gen_pos.sqrDistance(gen->mPosition) < 25.0f) {
				return kill;
			}
		}
	}
	return KilledEnemy(); // course == 0xFF => not killed
}

Preset::CarriedTreasure Preset::get_treasure_override(int course, int treasure_id, u8 pellet_type)
{
	for (u32 i = 0; i < carried_treasures.len(); i++) {
		CarriedTreasure& treasure = carried_treasures[i];
		if (treasure.course != course) {
			continue; // belongs to a different area
		}
		if (treasure_id == treasure.id) {
			return treasure;
		}
	}
	return CarriedTreasure(); // course == 0xFF => spawns as normal
}

bool Preset::is_area_visited(int course) const
{
	// only rebuild this area if it actually has gen edits tagged for it
	// (structures, enemies, treasures)
	if (area_states[course].has_any_state()) {
		return true;
	}
	for (u32 i = 0; i < killed_enemies.len(); i++) {
		if (killed_enemies[i].course == course) {
			return true;
		}
	}
	for (u32 i = 0; i < carried_treasures.len(); i++) {
		if (carried_treasures[i].course == course) {
			return true;
		}
	}
	return false;
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

	// Clear open area flags before applying upgrades, so we don't have persistent open areas.
	// Skip for in-place replay: we're keeping the other areas' generator caches, so their PDCF_Visited
	// flags must stay set to match. Otherwise the area loads BOTH initgen (because it looks unvisited)
	// and the cache, doubling the generators and overrunning the pellet pool (genPellet "GENERATOR ERR").
	if (!p2gz->warp->only_rebuild_current_area) {
		for (int i = 1; i < 4; i++) {
			Game::playData->mBitfieldPerCourse[i] = Game::PlayData::PDCF_Unset;
		}
	}

	// Apply upgrades
	for (u32 i = Game::OlimarData::ODII_FIRST_EXPLORATION_KIT_ITEM; i <= Game::OlimarData::ODII_LAST_EXPLORATION_KIT_ITEM; i++) {
		const u16 mask = 1 << i;
		p2gz->ek_editor->set_upgrade(static_cast<Game::OlimarData::ItemIndex>(i), upgrades.isSet(mask));
	}

	// make sure map-screen zoom effect is correct
	for (int i = 0; i < 4; i++) {
		if (Game::playData->courseOpen(i) && !new_area_zoom.isSet(1 << i)) {
			Game::playData->mBitfieldPerCourse[i] |= Game::PlayData::PDCF_JustOpen;
		}
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

	for (u32 i = 0; i < carried_treasures.len(); i++) {
		// moved treasures = not collected
		if (carried_treasures[i].moved) {
			continue;
		}

		u32 id = carried_treasures[i].id;
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

	// set whether %cutscene should be forced to play or not
	if (play_repay_demo) {
		p2gz->poko_editor->repay_demo_enabled = true;
	}

	// only wipe all areas if we warp, not if we replay/retry a segment
	if (!p2gz->warp->only_rebuild_current_area) {
		Game::generatorCache->clearCache();
		Game::playData->clearVisitAllCourses();
		Game::playData->mLimitGen->mNonLoops.all_zero();
	}

	// Convert position-based StructureOverride data into per-area name-based state
	// for use by reconstruct_generator_cache() during the upcoming load.
	// clear area_states so it doesn't accumulate incorrectly across loads
	for (int i = 0; i < 4; i++) {
		area_states[i].destroyed_gates.clear();
		area_states[i].finished_bridges.clear();
		area_states[i].bags_flattened.clear();
		area_states[i].plug_destroyed = false;
	}
	for (u32 i = 0; i < destroyed_gates.len(); i++) {
		const StructureOverride& o = destroyed_gates[i];
		const char* name           = p2gz->structure_editor->find_gate_name(o.position, o.area);
		if (name)
			area_states[o.area].destroyed_gates.push(name);
	}
	for (u32 i = 0; i < finished_bridges.len(); i++) {
		const StructureOverride& o = finished_bridges[i];
		const char* name           = p2gz->structure_editor->find_bridge_name(o.position, o.area);
		if (name)
			area_states[o.area].finished_bridges.push(name);
	}
	for (u32 i = 0; i < bags_flattened.len(); i++) {
		const StructureOverride& o = bags_flattened[i];
		const char* name           = p2gz->structure_editor->find_bag_name(o.position, o.area);
		if (name)
			area_states[o.area].bags_flattened.push(name);
	}
	for (u32 i = 0; i < plugs_destroyed.len(); i++) {
		const StructureOverride& o = plugs_destroyed[i];
		const char* name           = p2gz->structure_editor->find_plug_name(o.position, o.area);
		if (name)
			area_states[o.area].plug_destroyed = true;
	}

	// Set red onion flags so that it appears in the correct place for Day 2 presets
	if (day > 1) {
		Game::playData->setBootContainer(Game::Red);
		Game::playData->setContainer(Game::Red);
	}

	// Restore the collected treasure state
	treasure_state.restore(p2gz->warp->get_dest().sublevel);
}

void Preset::TreasureState::restore(u8 dest_sublevel)
{
	if (mode != TM_Off) {
		// deal with when we're in the middle of a cave
		Game::PelletCropMemory* cave = Game::playData->mCaveCropMemory;
		int cpoko                    = cave_poko_count;
		if (cave) {
			cave->mOtakara.clear();
			cave->mItem.clear();
			// base state (start of preset range)
			for (u32 i = 0; i < cave_held.len(); i++) {
				const HeldPellet& h = cave_held[i];
				if (h.kind == 0) {
					cave->mOtakara(h.id) = 1;
				} else {
					cave->mItem(h.id) = 1;
				}
			}
			// per-floor deltas for floors beyond the target (if preset is multi-floor)
			for (u32 i = 0; i < sublevel_deltas.len(); i++) {
				const SublevelDelta& d = sublevel_deltas[i];
				if (d.sublevel > dest_sublevel) {
					continue;
				}
				cpoko += d.poko_delta;
				for (u32 j = 0; j < d.caught.len(); j++) {
					const HeldPellet& h = d.caught[j];
					if (h.kind == 0) {
						cave->mOtakara(h.id) += 1;
					} else {
						cave->mItem(h.id) += 1;
					}
				}
			}
		}
		Game::playData->mCavePokoCount = cpoko;

		// restore piklopedia on retry
		Game::PelletFirstMemory* zukan = Game::playData->mZukanStat;
		if (zukan) {
			zukan->mOtakara.clear();
			for (u32 i = 0; i < zukan_otakara.len(); i++) {
				zukan->mOtakara(zukan_otakara[i]) = Game::KindCounter::KCF_Earned;
			}
			zukan->mItem.clear();
			for (u32 i = 0; i < zukan_item.len(); i++) {
				zukan->mItem(zukan_item[i]) = Game::KindCounter::KCF_Earned;
			}
		}

		// also restore counts/pokos from before we entered the cave/current segment
		if (mode == TM_Checkpoint) {
			Game::PelletCropMemory* main = Game::playData->mMainCropMemory;
			if (main) {
				main->mOtakara.clear();
				main->mItem.clear();
				// no treasures are held above-ground, so leave the main crop memory cleared
			}

			Game::playData->mTreasureCount = treasure_count;
			Game::playData->mPokoCount     = poko_count;

			// mark the debt-repayment levels these pokos reach as already-seen
			// (we'll toggle the correct one on manually)
			Game::playData->experienceRepayLevelFirstClear();
		}
	}

	if (debt != -1) {
		// force debt flags
		if (debt) {
			Game::playData->mStoryFlags |= Game::STORY_DebtPaid;
		} else {
			Game::playData->mStoryFlags &= ~Game::STORY_DebtPaid;
		}
		// always leave the all-treasures flag clear so the ending can (re)fire after collecting the rest
		Game::playData->mStoryFlags &= ~Game::STORY_AllTreasuresCollected;
	}
}

void Preset::apply_post_load()
{
	// Gates, bridges (stage count), bags, plugs are pre-baked into the generator cache
	// by reconstruct_generator_cache() before initGenerators() runs.

	// Bridge glitch is a runtime flag on the live ItemBridge object, not a cached field,
	// so it must be applied here after the destination area has loaded.
	p2gz->structure_editor->set_bridge_glitch(bridge_glitch_active);

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

	// Carve out for Day 2 presets. Makes the onion emit a single seed instead of playing the extinction cutscene immediately.
	if (in_above_ground_play() && squad.getTotalSum() == 0 && onion_pikis.getTotalSum() == 0 && day > 1) {
		Game::SingleGame::GameState* game_state = static_cast<Game::SingleGame::GameState*>(get_SGS()->mCurrentState);
		game_state->mIsPostExtinct              = true;
	}
}
