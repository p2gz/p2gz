#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
#include <p2gz/BoundDelegate.h>
#include <Game/Piki.h>
#include <JSystem/J2D/J2DPrint.h>
#include <System.h>

using namespace gz;
using namespace Game;

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

Preset* Preset::set_cutscene_flags(size_t num_flags, int flags[])
{
	cutscene_flags.expandCapacityTo(num_flags);
	for (size_t i = 0; i < num_flags; i++) {
		cutscene_flags.push(flags[i]);
	}
	return this;
}

void Preset::apply()
{
	// TODO: is this necessary?
	// GameStat::mePikis.clear(); // clear sprouts

	// Clear squad
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			p2gz->squad_editor->kill_piki(static_cast<EPikiKind>(color), static_cast<EPikiHappa>(stage), MAX_PIKI_COUNT);
		}
	}

	// Apply squad
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			int amount = squad.getCount(color, stage);
			p2gz->squad_editor->birth_piki(static_cast<EPikiKind>(color), static_cast<EPikiHappa>(stage), amount);
		}
	}

	// Apply onion pikmin
	playData->mPikiContainer = onion_pikis;

	// Apply sprays
	p2gz->spray_editor->set_bitters(num_bitters);
	p2gz->spray_editor->set_spicies(num_spicies);
	p2gz->spray_editor->toggle_bitters(bitters_unlocked);
	p2gz->spray_editor->toggle_spicies(spicies_unlocked);

	// Set cutscene flags
	// TODO: use cutscene flag editor for this
	// for (size_t i = 0; i < preset.cutscene_flags.len(); i++) {
	// 	playData->mDemoFlags.setFlag(preset.cutscene_flags[i]);
	// }
}

PresetMenuOption::PresetMenuOption()
    : MenuOption("preset")
{
	ListMenu* pod_presets_menu     = new ListMenu();
	ListMenu* at_presets_menu      = new ListMenu();
	ListMenu* general_presets_menu = new ListMenu();
	preset_category_list           = (new ListMenu())
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
	MenuLayer* warp_menu   = p2gz->menu->get_option("warp")->get_sub_menu();
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
