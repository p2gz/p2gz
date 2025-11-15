#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
#include <p2gz/BoundDelegate.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <JSystem/J2D/J2DPrint.h>
#include <System.h>
#include <Game/gameGeneratorCache.h>
#include <Game/Entities/PelletOtakara.h>

using namespace gz;

Preset::Preset(const char* name_, PresetCategory category_)
    : destroyed_gates(0)
    , finished_bridges(0)
    , bags_flattened(0)
    , enemy_spawn_overrides(0)
    , treasure_spawn_overrides(0)
{
	name             = name_;
	category         = category_;
	bitters_unlocked = false;
	spicies_unlocked = false;
	num_bitters      = 0;
	num_spicies      = 0;
	time             = 7.0f;
	plug_destroyed   = false;
	upgrades         = 0;
	day              = 5;

	squad.clear();
	onion_pikis.clear();
}

Preset::Preset(Preset& other)
{
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

	destroyed_gates.expandCapacityTo(other.destroyed_gates.len());
	for (size_t i = 0; i < other.destroyed_gates.len(); i++) {
		destroyed_gates.push(other.destroyed_gates[i]);
	}

	finished_bridges.expandCapacityTo(other.finished_bridges.len());
	for (size_t i = 0; i < other.finished_bridges.len(); i++) {
		finished_bridges.push(other.finished_bridges[i]);
	}

	bags_flattened.expandCapacityTo(other.bags_flattened.len());
	for (size_t i = 0; i < other.bags_flattened.len(); i++) {
		bags_flattened.push(other.bags_flattened[i]);
	}

	enemy_spawn_overrides.expandCapacityTo(other.enemy_spawn_overrides.len());
	for (size_t i = 0; i < other.enemy_spawn_overrides.len(); i++) {
		enemy_spawn_overrides.push(other.enemy_spawn_overrides[i]);
	}

	treasure_spawn_overrides.expandCapacityTo(other.treasure_spawn_overrides.len());
	for (size_t i = 0; i < other.treasure_spawn_overrides.len(); i++) {
		treasure_spawn_overrides.push(other.treasure_spawn_overrides[i]);
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

GenSpawnOverride Preset::get_enemy_gen_override(Game::Generator* gen)
{
	if (gen->mObject->mTypeID == 'teki') {
		Game::GenObjectEnemy* gen_obj_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
		for (size_t i = 0; i < enemy_spawn_overrides.len(); i++) {
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
	for (size_t i = 0; i < treasure_spawn_overrides.len(); i++) {
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

Preset* Preset::set_cutscene_flags(size_t num_flags, Game::DemoFlags flags[])
{
	for (size_t i = 0; i < num_flags; i++) {
		cutscenes.set_cutscene_played(flags[i]);
	}
	return this;
}

Preset* Preset::set_upgrades(size_t num_upgrades, Game::OlimarData::ItemIndex items[])
{
	for (size_t i = 0; i < num_upgrades; i++) {
		const u16 bit = 1 << items[i];
		upgrades |= bit;
	}
	return this;
}

Preset* Preset::set_destroyed_gates(size_t num_gates, const char* gates[])
{
	destroyed_gates.expandCapacityTo(destroyed_gates.len() + num_gates);
	for (size_t i = 0; i < num_gates; i++) {
		GZASSERTLINE(gates[i]);
		destroyed_gates.push(gates[i]);
	}
	return this;
}

Preset* Preset::set_finished_bridges(size_t num_bridges, const char* bridges[])
{
	finished_bridges.expandCapacityTo(finished_bridges.len() + num_bridges);
	for (size_t i = 0; i < num_bridges; i++) {
		GZASSERTLINE(bridges[i]);
		finished_bridges.push(bridges[i]);
	}
	return this;
}

Preset* Preset::set_bags_flattened(size_t num_bags, const char* bags[])
{
	bags_flattened.expandCapacityTo(bags_flattened.len() + num_bags);
	for (size_t i = 0; i < num_bags; i++) {
		GZASSERTLINE(bags[i]);
		bags_flattened.push(bags[i]);
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

Preset* Preset::set_enemy_spawn_overrides(size_t num_spawns, EnemyGenSpawnOverride overrides[])
{
	enemy_spawn_overrides.expandCapacityTo(enemy_spawn_overrides.len() + num_spawns);
	for (size_t i = 0; i < num_spawns; i++) {
		enemy_spawn_overrides.push(overrides[i]);
	}
	return this;
}

Preset* Preset::set_treasure_spawn_overrides(size_t num_spawns, TreasureGenSpawnOverride overrides[])
{
	treasure_spawn_overrides.expandCapacityTo(treasure_spawn_overrides.len() + num_spawns);
	for (size_t i = 0; i < num_spawns; i++) {
		treasure_spawn_overrides.push(overrides[i]);
	}
	return this;
}

void Preset::apply()
{
	// TODO: is this necessary?
	// GameStat::mePikis.clear(); // clear sprouts

	if (Game::naviMgr && Game::naviMgr->mArray) {
		for (int i = 0; i < 2; i++) {
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
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
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
	for (size_t i = Game::OlimarData::ODII_FIRST_EXPLORATION_KIT_ITEM; i < Game::OlimarData::ODII_LAST_EXPLORATION_KIT_ITEM; i++) {
		const u16 mask = 1 << i;
		p2gz->ek_editor->set_upgrade(static_cast<Game::OlimarData::ItemIndex>(i), upgrades & mask);
	}

	// Set cutscene flags
	p2gz->cutscene_mgr->reset_all();
	for (size_t i = 0; i < Game::DEMO_FLAG_COUNT; i++) {
		const Game::DemoFlags flag = static_cast<Game::DemoFlags>(i);
		if (cutscenes.cutscene_played(flag)) {
			CutsceneToggle* cutscene_toggle = p2gz->cutscene_mgr->get_toggle(flag);
			if (cutscene_toggle) {
				cutscene_toggle->set_cutscene_flag(true);
			}
		}
	}

	p2gz->warp->set_enter_area_type(enter_kind);

	if (apply_pokos) {
		p2gz->poko_editor->set_pokos(pokos);
	}
}

void Preset::apply_post_load()
{
	p2gz->structure_editor->reset_all_structures();

	for (size_t i = 0; i < destroyed_gates.len(); i++) {
		p2gz->structure_editor->set_gate_stages_left(destroyed_gates[i], 0);
	}
	for (size_t i = 0; i < finished_bridges.len(); i++) {
		p2gz->structure_editor->set_bridge_stages_left(finished_bridges[i], 0);
	}
	for (size_t i = 0; i < bags_flattened.len(); i++) {
		p2gz->structure_editor->set_bag_flattened(bags_flattened[i], true);
	}
	if (plug_destroyed) {
		p2gz->structure_editor->set_plug_destroyed(true);
	}

	// Make sure all navi have max health
	if (Game::naviMgr && Game::naviMgr->mArray) {
		for (int i = 0; i < 2; i++) {
			Game::Navi* navi = Game::naviMgr->getAt(i);
			if (navi && navi->isAlive()) {
				navi->mHealth = 50.0f;
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
			for (size_t i = 0; i < treasure_spawn_overrides.len(); i++) {
				TreasureGenSpawnOverride& oride = treasure_spawn_overrides[i];
				if (oride.id == treasure_id) {
					if (oride.spawn_override == PSO_Spawn && gen->mCreature == nullptr) {
						gen->generate();
					} else if (oride.spawn_override == PSO_DontSpawn && gen->mCreature) {
						Game::PelletKillArg arg;
						gen->mCreature->kill(&arg);
					}
				}
			}
		}
	}
}

PresetMenuOption::PresetMenuOption(IDelegate2<Preset*, int>* on_select_)
    : MenuOption("preset")
{
	on_select        = on_select_;
	pod_presets_menu = new ListMenu();
	pod_presets_menu->on_opened
	    = new BoundDelegate2<PresetMenuOption, ListMenu*, PresetCategory>(this, &select_current_preset, pod_presets_menu, PoD);
	at_presets_menu = new ListMenu();
	at_presets_menu->on_opened
	    = new BoundDelegate2<PresetMenuOption, ListMenu*, PresetCategory>(this, &select_current_preset, at_presets_menu, AT);
	general_presets_menu = new ListMenu();
	general_presets_menu->on_opened
	    = new BoundDelegate2<PresetMenuOption, ListMenu*, PresetCategory>(this, &select_current_preset, general_presets_menu, General);

	preset_category_list = (new ListMenu())
	                           ->push(new PresetPreviewMenuOption(nullptr, this)) // "no preset" option
	                           ->push(new OpenSubMenuOption("PoD", pod_presets_menu))
	                           ->push(new OpenSubMenuOption("AT", at_presets_menu))
	                           ->push(new OpenSubMenuOption("general", general_presets_menu));
	preset_category_list->title = "preset categories";

	// Add every preset to its appropriate menu
	for (size_t i = 0; i < p2gz->preset_mgr->presets.len(); i++) {
		Preset* preset = p2gz->preset_mgr->presets[i];
		if (!preset) {
			continue;
		}

		PresetPreviewMenuOption* opt = new PresetPreviewMenuOption(preset, this);
		if (preset->category == PoD) {
			pod_presets_menu->push(opt);
		} else if (preset->category == AT) {
			at_presets_menu->push(opt);
		} else if (preset->category == General) {
			general_presets_menu->push(opt);
		}
	}

	// Set the current preset to a PoD one so PresetMgr can suggest an appropriate preset
	// when changing the warp menu selections
	current_preset = p2gz->preset_mgr->find("EC1", PoD);
	if (on_select) {
		on_select->invoke(current_preset, PS_Stale);
	}
}

/// Adjusts the selection of a category menu when it's opened so the current preset is highlighted
void PresetMenuOption::select_current_preset(ListMenu* menu, PresetCategory cat)
{
	if (!current_preset || current_preset->category != cat || !menu) {
		return;
	}

	bool found          = false;
	int idx_in_category = -1;
	for (size_t i = 0; i < p2gz->preset_mgr->presets.len(); i++) {
		Preset* preset = p2gz->preset_mgr->presets[i];
		if (preset->category == current_preset->category) {
			idx_in_category += 1;
		}
		if (preset == current_preset) {
			found = true;
			break;
		}
	}

	if (!found) {
		return;
	}

	const size_t num_options = menu->options.len();
	GZASSERTLINE(num_options > 0);
	GZASSERTLINE(idx_in_category < num_options);

	menu->selected = idx_in_category;
}

static const char* PIKI_IMG_NAMES[18] = { "blue_leaf",   "blue_bud",   "blue_flower",   "red_leaf",     "red_bud",     "red_flower",
	                                      "yellow_leaf", "yellow_bud", "yellow_flower", "purple_leaf",  "purple_bud",  "purple_flower",
	                                      "white_leaf",  "white_bud",  "white_flower",  "bulbmin_leaf", "bulbmin_bud", "bulbmin_flower" };

static const char* ONION_IMG_NAMES[5] = {
	"onion_blue", "onion_red", "onion_yellow", "ship_purple", "ship_white",
};

f32 draw_preset_preview(J2DPrint& j2d, f32 x, f32 z, Preset* preset)
{
	const f32 img_number_margin   = 2.0;
	const f32 number_right_margin = 5.0;

	// squad pikmin
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			int amount = preset->squad.getCount(color, stage);
			if (amount == 0) {
				continue;
			}

			const char* img_name = PIKI_IMG_NAMES[color * 3 + stage];
			if (!img_name) {
				continue;
			}

			x += p2gz->images->draw(img_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
			x += img_number_margin;

			j2d.initiate();
			x += j2d.print(x, z, "%d", amount);
			x += number_right_margin;
		}
	}

	// onion pikmin
	for (int color = 0; color < 5; color++) {
		int amount = 0;
		for (int stage = 0; stage < 3; stage++) {
			amount += preset->onion_pikis.getCount(color, stage);
		}
		if (amount == 0) {
			continue;
		}

		const char* img_name = ONION_IMG_NAMES[color];
		if (!img_name) {
			continue;
		}

		x += p2gz->images->draw(img_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
		x += img_number_margin;

		j2d.initiate();
		x += j2d.print(x, z, "%d", amount);
		x += number_right_margin;
	}

	return x;
}

void PresetMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	MenuOption::draw(j2d, x, z, selected);
	x += j2d.print(x, z, ": ");
	if (current_preset) {
		draw_preset_preview(j2d, x, z, current_preset);
	} else {
		x += j2d.print(x, z, "none");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "open presets menu");
	}
}

bool PresetMenuOption::select()
{
	p2gz->menu->push_layer(preset_category_list);
	return false;
}

void PresetMenuOption::do_on_preset_selected(Preset* preset)
{
	if (on_select) {
		on_select->invoke(preset, PS_Chosen);
	}
}

PresetPreviewMenuOption::PresetPreviewMenuOption(Preset* preset_, PresetMenuOption* parent_)
    : MenuOption(preset_ ? preset_->name : nullptr)
{
	GZASSERTLINE(parent_);

	preset = preset_;
	parent = parent_;
}

bool PresetPreviewMenuOption::select()
{
	parent->current_preset = preset;
	parent->do_on_preset_selected(preset);
	MenuLayer* warp_menu = p2gz->menu->get_option("warp")->get_sub_menu();
	while (p2gz->menu->get_active_layer() != warp_menu) {
		p2gz->menu->pop_layer();
	}
	return false;
}

void PresetPreviewMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (preset) {
		const f32 vmargin = 5.0; // some extra vertical space for the images
		z += vmargin;
		MenuOption::draw(j2d, x, z, selected);
		x = 190.0f;
		x += draw_preset_preview(j2d, x, z, preset);
		z += vmargin;

	} else {
		x += j2d.print(x, z, "no preset (use current squad)");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "use this preset");
	}
}
