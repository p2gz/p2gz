#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
#include <p2gz/BoundDelegate.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <JSystem/J2D/J2DPrint.h>
#include <System.h>
#include <Game/gameGeneratorCache.h>

using namespace gz;

Preset::Preset(const char* name_, PresetCategory category_)
    : upgrades(1)
    , cutscene_flags(1)
{
	name             = name_;
	category         = category_;
	bitters_unlocked = false;
	spicies_unlocked = false;
	num_bitters      = 0;
	num_spicies      = 0;
	time             = 7.0f;

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

	upgrades.expandCapacityTo(other.upgrades.len());
	for (size_t i = 0; i < other.upgrades.len(); i++) {
		upgrades.push(other.upgrades[i]);
	}

	cutscene_flags.expandCapacityTo(other.cutscene_flags.len());
	for (size_t i = 0; i < other.cutscene_flags.len(); i++) {
		cutscene_flags.push(other.cutscene_flags[i]);
	}
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
	cutscene_flags.expandCapacityTo(cutscene_flags.len() + num_flags);
	for (size_t i = 0; i < num_flags; i++) {
		cutscene_flags.push(flags[i]);
	}
	return this;
}

Preset* Preset::set_upgrades(size_t num_upgrades, Game::OlimarData::ItemIndex items[])
{
	upgrades.expandCapacityTo(upgrades.len() + num_upgrades);
	for (size_t i = 0; i < num_upgrades; i++) {
		upgrades.push(items[i]);
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
	p2gz->ek_editor->reset_all();
	for (size_t i = 0; i < upgrades.len(); i++) {
		p2gz->ek_editor->set_upgrade(upgrades[i], true);
	}

	// Set cutscene flags
	p2gz->cutscene_mgr->reset_all();
	for (size_t i = 0; i < cutscene_flags.len(); i++) {
		Game::DemoFlags flag            = cutscene_flags[i];
		CutsceneToggle* cutscene_toggle = p2gz->cutscene_mgr->get_toggle(flag);
		if (cutscene_toggle) {
			cutscene_toggle->set_cutscene_flag(true);
		}
	}
}

void Preset::apply_post_load()
{
	// Destroy gates
	for (size_t i = 0; i < destroyed_gates.len(); i++) {
		p2gz->structure_editor->set_gate_stages_left(destroyed_gates[i], 0);
	}
}

PresetMenuOption::PresetMenuOption(IDelegate2<Preset*, int>* on_select_)
    : MenuOption("preset")
{
	on_select            = on_select_;
	pod_presets_menu     = new ListMenu();
	at_presets_menu      = new ListMenu();
	general_presets_menu = new ListMenu();
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
	current_preset = p2gz->preset_mgr->find("EC", PoD);
	if (on_select) {
		on_select->invoke(current_preset, PS_Stale);
	}
}

static const char* PIKI_IMG_NAMES[15] = {
	"blue_leaf",     "blue_bud",    "blue_flower", "red_leaf",      "red_bud",    "red_flower", "yellow_leaf",  "yellow_bud",
	"yellow_flower", "purple_leaf", "purple_bud",  "purple_flower", "white_leaf", "white_bud",  "white_flower",
};

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
}

void PresetMenuOption::select()
{
	p2gz->menu->push_layer(preset_category_list);
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

void PresetPreviewMenuOption::select()
{
	parent->current_preset = preset;
	parent->do_on_preset_selected(preset);
	MenuLayer* warp_menu = p2gz->menu->get_option("warp")->get_sub_menu();
	while (p2gz->menu->get_active_layer() != warp_menu) {
		p2gz->menu->pop_layer();
	}
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
}
