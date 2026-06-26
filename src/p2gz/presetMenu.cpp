#include <p2gz/Preset.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/BoundDelegate.h>

using namespace gz;

PresetMenuOption::PresetMenuOption(IDelegate2<PresetPreview*, int>* on_select_)
    : MenuOption("preset")
{
	on_select        = on_select_;
	current_preview  = nullptr; // assigned in Warp::init() instead
	pod_presets_menu = new ListMenu();
	pod_presets_menu->on_opened
	    = new BoundDelegate2<PresetMenuOption, ListMenu*, PresetCategory>(this, &select_current_preset, pod_presets_menu, PoD);
	at_presets_menu = new ListMenu();
	at_presets_menu->on_opened
	    = new BoundDelegate2<PresetMenuOption, ListMenu*, PresetCategory>(this, &select_current_preset, at_presets_menu, AT);
	general_presets_menu = new ListMenu();
	general_presets_menu->on_opened
	    = new BoundDelegate2<PresetMenuOption, ListMenu*, PresetCategory>(this, &select_current_preset, general_presets_menu, General);

	preset_category_list        = (new ListMenu())
	                                  ->push(new PresetPreviewMenuOption(nullptr, this)) // "no preset" option
	                                  ->push(new OpenSubMenuOption("PoD", pod_presets_menu))
	                                  ->push(new OpenSubMenuOption("AT", at_presets_menu))
	                                  ->push(new OpenSubMenuOption("general", general_presets_menu));
	preset_category_list->title = "preset categories";
}

void PresetMenuOption::init()
{
	// Add every preset to its appropriate menu
	for (size_t i = 0; i < p2gz->preset_mgr->preset_previews.len(); i++) {
		PresetPreview* preview = p2gz->preset_mgr->preset_previews[i];
		if (!preview) {
			continue;
		}

		PresetPreviewMenuOption* opt = new PresetPreviewMenuOption(preview, this);
		switch (preview->category) {
		case PoD:
			pod_presets_menu->push(opt);
			break;
		case AT:
			at_presets_menu->push(opt);
			break;
		case General:
			general_presets_menu->push(opt);
			break;
		}
	}
}

/// Adjusts the selection of a category menu when it's opened so the current preset is highlighted
void PresetMenuOption::select_current_preset(ListMenu* menu, PresetCategory cat)
{
	if (!current_preview || current_preview->category != cat || !menu) {
		return;
	}

	bool found          = false;
	int idx_in_category = -1;
	for (size_t i = 0; i < p2gz->preset_mgr->preset_previews.len(); i++) {
		PresetPreview* preset_preview = p2gz->preset_mgr->preset_previews[i];
		if (preset_preview->category == current_preview->category) {
			idx_in_category += 1;
		}
		if (preset_preview == current_preview) {
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

// Draws the squad/onion icons on their own line below the name, so they don't run off the right of the screen (as bad)
static void draw_icon_row(J2DPrint& j2d, f32 indent, f32& z, Game::PikiContainer& squad, Game::PikiContainer& onion_pikis)
{
	// don't add a gap unless we have pikmin/onions to draw
	if (squad.getTotalSum() == 0 && onion_pikis.getTotalSum() == 0) {
		return;
	}

	const f32 vmargin = 6.0f; // extra breathing room above/below the icon row, they're a bit bulky
	z += p2gz->menu->line_height + vmargin;
	f32 x = indent;
	draw_preset_preview(j2d, x, z, squad, onion_pikis);
	z += vmargin;
}

void PresetMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	const f32 icon_indent = x + 20.0f;

	MenuOption::draw(j2d, x, z, selected);
	x += j2d.print(x, z, ": ");

	const size_t cycle_count = p2gz->warp->preset_cycle_count();
	const bool can_cycle     = cycle_count > 1;

	if (current_preview) {
		if (can_cycle) {
			x += j2d.print(x, z, "< ");
		}
		if (current_preview->name) {
			x += j2d.print(x, z, "%s ", current_preview->name);
		}
		if (can_cycle) {
			x += j2d.print(x, z, "> (%d/%d) ", static_cast<int>(p2gz->warp->preset_cycle_index() + 1), static_cast<int>(cycle_count));
		}
		draw_icon_row(j2d, icon_indent, z, current_preview->squad, current_preview->onion_pikis);
	} else {
		x += j2d.print(x, z, "current squad ");
		// only show pikmin that will actually warp (i.e. not wild pikmin, bulbmin, non-blues to SmC)
		Game::PikiContainer current_squad = p2gz->warp->preview_warp_squad();
		draw_icon_row(j2d, icon_indent, z, current_squad, Game::playData->mPikiContainer);
	}

	if (selected) {
		if (can_cycle) {
			p2gz->menu->draw_control(j2d, Controller::PRESS_DPAD_LEFT, "cycle preset");
		}
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "all presets");
	}
}

bool PresetMenuOption::select()
{
	p2gz->menu->push_layer(preset_category_list);
	return false;
}

void PresetMenuOption::update()
{
	// D-pad left/right cycles through the presets relevant to the current warp destination
	// (block_open_close_action stops the double-tap-left menu toggle from firing while cycling)
	p2gz->menu->block_open_close_action();

	u32 btn = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_LEFT) {
		p2gz->warp->cycle_preset(-1);
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		p2gz->warp->cycle_preset(1);
	}
}

void PresetMenuOption::do_on_preset_selected(PresetPreview* preset_preview)
{
	if (on_select) {
		on_select->invoke(preset_preview, PS_Chosen);
	}
	current_preview = preset_preview;
}

PresetPreviewMenuOption::PresetPreviewMenuOption(PresetPreview* preset_preview_, PresetMenuOption* parent_)
    : MenuOption(preset_preview_ ? preset_preview_->name : nullptr)
{
	GZASSERTLINE(parent_);

	preset_preview = preset_preview_;
	parent         = parent_;
}

bool PresetPreviewMenuOption::select()
{
	parent->current_preview = preset_preview;
	parent->do_on_preset_selected(preset_preview);
	MenuLayer* warp_menu = p2gz->menu->get_option("warp")->get_sub_menu();
	while (p2gz->menu->get_active_layer() != warp_menu) {
		p2gz->menu->pop_layer();
	}
	return false;
}

void PresetPreviewMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (preset_preview) {
		const f32 vmargin = 5.0; // some extra vertical space for the images
		z += vmargin;
		MenuOption::draw(j2d, x, z, selected);
		x = 190.0f;
		x += draw_preset_preview(j2d, x, z, preset_preview->squad, preset_preview->onion_pikis);
		z += vmargin;

	} else {
		x += j2d.print(x, z, "no preset (use current squad)");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "use this preset");
	}
}
