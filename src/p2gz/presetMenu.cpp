#include <p2gz/Preset.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/BoundDelegate.h>

using namespace gz;

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

f32 draw_preset_preview(J2DPrint& j2d, f32 x, f32 z, Game::PikiContainer& squad, Game::PikiContainer& onion_pikis)
{
	const f32 img_number_margin   = 2.0;
	const f32 number_right_margin = 5.0;

	// squad pikmin
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			int amount = squad.getCount(color, stage);
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
			amount += onion_pikis.getCount(color, stage);
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
		if (current_preset->name) {
			x += j2d.print(x, z, "%s ", current_preset->name);
		}
		draw_preset_preview(j2d, x, z, current_preset->squad, current_preset->onion_pikis);
	} else {
		x += j2d.print(x, z, "current squad ");
		Game::PikiContainer current_squad = p2gz->squad_editor->get_squad();
		draw_preset_preview(j2d, x, z, current_squad, Game::playData->mPikiContainer);
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
		x += draw_preset_preview(j2d, x, z, preset->squad, preset->onion_pikis);
		z += vmargin;

	} else {
		x += j2d.print(x, z, "no preset (use current squad)");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "use this preset");
	}
}
