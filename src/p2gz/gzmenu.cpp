#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/NaviTools.h>
#include <p2gz/FreeCam.h>
#include <p2gz/DayEditor.h>
#include <p2gz/WaypointViewer.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <System.h>
#include <Controller.h>
#include <Dolphin/os.h>
#include <string.h>
#include <IDelegate.h>
#include <p2gz/gzConstants.h>

using namespace gz;

GZMenu::GZMenu()
    : open_close_action(DoublePress(Controller::PRESS_DPAD_LEFT, 15))
    , enabled(false)
    , lock(false)
    , eat_inputs(true)
{
	glyph_width               = 18.0f;
	glyph_height              = 18.0f;
	start_offset_x            = 32.0f;
	breadcrumb_start_offset_x = 10.0f;
	start_offset_z            = 32.0f;
	line_height               = 22.0f;

	color_std         = JUtility::TColor(255, 255, 255, 255);
	color_highlight   = JUtility::TColor(255, 40, 40, 255);
	color_breadcrumbs = JUtility::TColor(226, 192, 116, 255);
}

void GZMenu::init_menu()
{
	// clang-format off
	// Structure of GZ menu defined here:
    root_layer = (new ListMenu())
		->push(new OpenSubMenuOption("warp", (new ListMenu())
			->push(new RadioMenuOption("area", new Delegate1<Warp, size_t>(p2gz->warp, &Warp::set_warp_area)))
			->push(new RadioMenuOption("cave", new Delegate1<Warp, size_t>(p2gz->warp, &Warp::set_warp_cave)))
			->push(new RangeMenuOption("sublevel", 1, 14, 1, RangeMenuOption::WRAP, new Delegate1<Warp, s32>(p2gz->warp, &Warp::set_warp_sublevel)))
			->push(new RangeMenuOption("day", 1, 99, 3, RangeMenuOption::CAP, new Delegate1<Warp, s32>(p2gz->warp, &Warp::set_warp_day)))
			->push(new PerformActionMenuOption("go", new Delegate<Warp>(p2gz->warp, &Warp::do_warp)))
		))
        ->push(new OpenSubMenuOption("captain", (new ListMenu())
            ->push(new PerformActionMenuOption("kill", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::kill)))
            ->push(new PerformActionMenuOption("boing", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::jump)))
        ))
		->push(new OpenSubMenuOption("map", (new ListMenu())
			->push(new ToggleMenuOption("collision", false, new Delegate1<CollisionViewer, bool>(p2gz->collision_viewer, &CollisionViewer::toggle)))
			->push(new ToggleMenuOption("waypoints", false, new Delegate1<WaypointViewer, bool>(p2gz->waypoint_viewer, &WaypointViewer::toggle)))
		))
        ->push(new OpenSubMenuOption("settings", (new ListMenu())
            ->push(new PerformActionMenuOption("increase text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::increase_text_size)))
            ->push(new PerformActionMenuOption("decrease text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::decrease_text_size)))
            ->push(new ToggleMenuOption("skippable treasure cutscenes", true, new Delegate1<SkippableTreasureCS, bool>(p2gz->skippable_treasure_cutscenes, &SkippableTreasureCS::toggle_skippable)))
			->push(new OpenSubMenuOption("grid menu demo", (new GridMenu(24.0))
				->push_to_row(new OpenSubMenuOption("1", nullptr))->push_to_row(new OpenSubMenuOption("2", nullptr))->push_to_row(new OpenSubMenuOption("3", nullptr))->end_row()
				->push_to_row(new OpenSubMenuOption("4", nullptr))->push_to_row(new OpenSubMenuOption("5", nullptr))->push_to_row(new OpenSubMenuOption("6", nullptr))->end_row()
				->push_to_row(new OpenSubMenuOption("7", nullptr))->push_to_row(new OpenSubMenuOption("8", nullptr))->push_to_row(new OpenSubMenuOption("9", nullptr))
			))
        ))
		->push(new OpenSubMenuOption("tools", (new ListMenu())
			->push(new PerformActionMenuOption("freecam", new Delegate<FreeCam>(p2gz->freecam, &FreeCam::enable)))
			->push(new OpenSubMenuOption("time controls", (new ListMenu())
				->push(new ToggleMenuOption("pause time", false, new Delegate1<DayEditor, bool>(p2gz->day_editor, &DayEditor::set_time_paused)))
				->push(new FloatRangeMenuOption("time", 7.0, 19.0, 7.0, new Delegate1<DayEditor, f32>(p2gz->day_editor, &DayEditor::set_time)))
			))
		))
		->push(new OpenSubMenuOption("timer", (new ListMenu())
			->push(new ToggleMenuOption("enabled", true, new Delegate1<Timer, bool>(p2gz->timer, &Timer::set_enabled)))
			->push(new ToggleMenuOption("show sub-timer", true, new Delegate1<Timer, bool>(p2gz->timer, &Timer::set_sub_timer_enabled)))
			->push(new PerformActionMenuOption("reset", new Delegate<Timer>(p2gz->timer, &Timer::reset_main_timer)))
		))
		// Cutscene re-enable menu
		->push(new OpenSubMenuOption("cutscenes", (new ListMenu())
			->push(new CutsceneToggleMenuOption("Meet Red Pikmin", P2GZ_CT_MET_RED_PIKI,
				new Delegate2<CutsceneToggle, bool, P2GZ_CUTSCENE_TOGGLE_ID>(p2gz->cutscene_toggle, &CutsceneToggle::set_cutscene_flag)))
		));
	// clang-format on

	layer = root_layer;
}

void GZMenu::update()
{
	// When other classes control the menu (via navigate_to etc.) this prevents
	// their button inputs from affecting the menu the same frame they happen.
	if (lock) {
		lock = false;
		return;
	}

	Controller* controller = p2gz->controller;

	if (controller) {
		// If we ever press A the double press to open/close the menu should be ignored
		// so we don't do it accidentally when switching pikmin or something
		if (!enabled && controller->getButton() & Controller::PRESS_A) {
			open_close_action.reset();
		}

		// Open/close the menu
		if (open_close_action.check(controller)) {
			if (enabled)
				close();
			else
				open();
		}
	}

	if (enabled && layer && controller) {
		layer->update();
	}
}

void GZMenu::increase_text_size()
{
	glyph_width += 2.0;
	glyph_height += 2.0;
	line_height += 2.0;
}

void GZMenu::decrease_text_size()
{
	glyph_width -= 2.0;
	glyph_height -= 2.0;
	line_height -= 2.0;
}

void GZMenu::push_layer(MenuLayer* layer_)
{
	if (layer_) {
		layer = layer_;
		layer->reset_selection();
		if (layer->title) {
			breadcrumbs.push(layer->title);
		}
	}
}

void GZMenu::pop_layer()
{
	if (layer->parent) {
		layer = layer->parent;
		breadcrumbs.pop();
	} else {
		close();
	}
}

void GZMenu::open()
{
	if (enabled)
		return;

	layer = root_layer;
	layer->reset_selection();
	breadcrumbs.clear();
	enabled = true;
	lock    = true;
}

void GZMenu::close()
{
	if (!enabled)
		return;

	enabled = false;
}

void GZMenu::draw()
{
	if (!enabled || !layer) {
		return;
	}

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = glyph_width;
	j2d.mGlyphHeight = glyph_height;

	f32 x = breadcrumb_start_offset_x;
	f32 z = start_offset_z;

	if (breadcrumbs.len() > 0) {
		for (size_t i = 0; i < breadcrumbs.len(); i++) {
			j2d.mCharColor.set(color_std);
			j2d.mGradientColor.set(color_std);
			x += j2d.print(x, z, " > ");

			j2d.mCharColor.set(color_breadcrumbs);
			j2d.mGradientColor.set(color_breadcrumbs);
			x += j2d.print(x, z, breadcrumbs[i]);
		}
		z += line_height;
	}

	x = start_offset_x; // reset x to the left
	layer->draw(j2d, x, z);
}

MenuOption* GZMenu::get_option(const char* path)
{
	if (!root_layer || !path) {
		return nullptr;
	}

	MenuOption* opt = root_layer->get_option(path);
	if (!opt) {
		OSReport("Option \"%s\" does not exist in GZ menu\n", path);
	}
	return opt;
}

void GZMenu::navigate_to(const char* path)
{
	close();
	open();
	root_layer->navigate_to(path);
}

MenuOption* ListMenu::get_option(const char* path)
{
	if (!path) {
		return nullptr;
	}

	const char* name_end         = strchr(path, '/');
	bool is_final_path_component = false;
	int name_len;
	if (name_end == nullptr) {
		name_len                = strlen(path);
		is_final_path_component = true;
	} else {
		name_len = name_end - path;
	}

	for (size_t i = 0; i < options.len(); i++) {
		if (strncmp(options[i]->title, path, name_len) == 0) {
			if (is_final_path_component) {
				return options[i];
			} else {
				MenuLayer* sub_menu = options[i]->get_sub_menu();
				if (sub_menu) {
					return sub_menu->get_option(name_end + 1);
				} else {
					return nullptr;
				}
			}
		}
	}

	return nullptr;
}

void ListMenu::navigate_to(const char* path)
{
	const char* name_end         = strchr(path, '/');
	bool is_final_path_component = false;
	int name_len;
	if (name_end == nullptr) {
		name_len                = strlen(path);
		is_final_path_component = true;
	} else {
		name_len = name_end - path;
	}

	for (size_t i = 0; i < options.len(); i++) {
		if (strncmp(options[i]->title, path, name_len) == 0) {
			selected = i;
			if (!is_final_path_component) {
				MenuLayer* sub_menu = options[i]->get_sub_menu();
				if (sub_menu) {
					p2gz->menu->push_layer(sub_menu);
					sub_menu->navigate_to(name_end + 1);
				}
			}
			return;
		}
	}
}

void ListMenu::update()
{
	u32 btn = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_UP && selected > 0) {
		do {
			selected -= 1;
		} while (!options[selected]->visible);
	}
	if (btn & Controller::PRESS_DPAD_DOWN && options.len() > 0 && selected < options.len() - 1) {
		do {
			selected += 1;
		} while (!options[selected]->visible);
	}
	if (btn & Controller::PRESS_A) {
		options[selected]->select();
	}
	if (btn & Controller::PRESS_B) {
		p2gz->menu->pop_layer();
	}

	options[selected]->update();
}

void ListMenu::draw(J2DPrint& j2d, f32 x, f32 z)
{
	for (size_t i = 0; i < options.len(); i++) {
		if (!options[i]->visible) {
			continue;
		}

		bool is_selected = i == selected;

		if (is_selected) {
			j2d.mCharColor.set(p2gz->menu->color_highlight);
			j2d.mGradientColor.set(p2gz->menu->color_highlight);
		} else {
			j2d.mCharColor.set(p2gz->menu->color_std);
			j2d.mGradientColor.set(p2gz->menu->color_std);
		}

		options[i]->draw(j2d, x, z, i == selected);
		z += p2gz->menu->line_height;
	}
}

void GridMenu::update()
{
	p2gz->menu->block_open_close_action();

	u32 btn = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_UP && selected_row > 0) {
		do {
			selected_row -= 1;
		} while (!cur_option()->visible);
	}
	if (btn & Controller::PRESS_DPAD_DOWN && options.len() > 0 && selected_row < options.len() - 1) {
		do {
			selected_row += 1;
		} while (!cur_option()->visible);
	}
	if (btn & Controller::PRESS_DPAD_LEFT && selected_col > 0) {
		do {
			selected_col -= 1;
		} while (!cur_option()->visible);
	}
	if (btn & Controller::PRESS_DPAD_RIGHT && options[selected_row]->len() > 0 && selected_col < options[selected_row]->len() - 1) {
		do {
			selected_col += 1;
		} while (!cur_option()->visible);
	}
	if (btn & Controller::PRESS_A) {
		cur_option()->select();
	}
	if (btn & Controller::PRESS_B) {
		p2gz->menu->pop_layer();
	}

	cur_option()->update();
}

void GridMenu::draw(J2DPrint& j2d, f32 x, f32 z)
{
	f32 start_x = x;
	for (size_t row_idx = 0; row_idx < options.len(); row_idx++) {
		for (size_t col_idx = 0; col_idx < options[row_idx]->len(); col_idx++) {
			MenuOption* option = (*options[row_idx])[col_idx];
			if (!option || !option->visible) {
				continue;
			}
			bool is_selected = col_idx == selected_col && row_idx == selected_row;

			if (is_selected) {
				j2d.mCharColor.set(p2gz->menu->color_highlight);
				j2d.mGradientColor.set(p2gz->menu->color_highlight);
			} else {
				j2d.mCharColor.set(p2gz->menu->color_std);
				j2d.mGradientColor.set(p2gz->menu->color_std);
			}

			option->draw(j2d, x, z, is_selected);
			x += column_width;
		}
		x = start_x;
		z += p2gz->menu->line_height;
	}
}

MenuOption* GridMenu::get_option(const char* path)
{
	if (!path) {
		return nullptr;
	}

	const char* name_end         = strchr(path, '/');
	bool is_final_path_component = false;
	int name_len;
	if (name_end == nullptr) {
		name_len                = strlen(path);
		is_final_path_component = true;
	} else {
		name_len = name_end - path;
	}

	for (size_t row_idx = 0; row_idx < options.len(); row_idx++) {
		for (size_t col_idx = 0; col_idx < options[row_idx]->len(); col_idx++) {
			MenuOption* option = (*options[row_idx])[col_idx];
			if (strncmp(option->title, path, name_len) == 0) {
				if (is_final_path_component) {
					return option;
				} else {
					MenuLayer* sub_menu = option->get_sub_menu();
					if (sub_menu) {
						return sub_menu->get_option(name_end + 1);
					} else {
						return nullptr;
					}
				}
			}
		}
	}

	return nullptr;
}

void GridMenu::navigate_to(const char* path)
{
	const char* name_end         = strchr(path, '/');
	bool is_final_path_component = false;
	int name_len;
	if (name_end == nullptr) {
		name_len                = strlen(path);
		is_final_path_component = true;
	} else {
		name_len = name_end - path;
	}

	for (size_t row_idx = 0; row_idx < options.len(); row_idx++) {
		for (size_t col_idx = 0; col_idx < options[row_idx]->len(); col_idx++) {
			MenuOption* option = (*options[row_idx])[col_idx];
			if (strncmp(option->title, path, name_len) == 0) {
				selected_col = col_idx;
				selected_row = row_idx;
				if (!is_final_path_component) {
					MenuLayer* sub_menu = option->get_sub_menu();
					if (sub_menu) {
						p2gz->menu->push_layer(sub_menu);
						sub_menu->navigate_to(name_end + 1);
					}
				}
				return;
			}
		}
	}
}

f32 MenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	if (title) {
		return j2d.print(x, z, title);
	}

	return 0.0f;
}

f32 ToggleMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	return j2d.print(x, z, "%s: %s", title, on ? "true" : "false");
}

OpenSubMenuOption::OpenSubMenuOption(const char* title_, MenuLayer* sub_menu_)
    : MenuOption(title_)
    , sub_menu(sub_menu_)
{
	if (sub_menu) {
		sub_menu->title = title_;
	}
}

void OpenSubMenuOption::select()
{
	p2gz->menu->push_layer(sub_menu);
}

void RadioMenuOption::update()
{
	p2gz->menu->block_open_close_action();

	size_t init_selected_idx = selected_idx;
	u32 btn                  = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_LEFT) {
		selected_idx = (((int)selected_idx) - 1) % options.len();
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		selected_idx = (selected_idx + 1) % options.len();
	}

	if (init_selected_idx != selected_idx) {
		on_selected->invoke(selected_idx);
	}
}

void RadioMenuOption::select()
{
	on_selected->invoke(selected_idx);
}

f32 RadioMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	x += j2d.print(x, z, "%s: < ", title);

	j2d.mCharColor.set(p2gz->menu->color_std);
	j2d.mGradientColor.set(p2gz->menu->color_std);
	x += j2d.print(x, z, options[selected_idx]);

	if (selected) {
		j2d.mCharColor.set(p2gz->menu->color_highlight);
		j2d.mGradientColor.set(p2gz->menu->color_highlight);
	}
	x += j2d.print(x, z, " >");

	return x;
}

void RangeMenuOption::update()
{
	p2gz->menu->block_open_close_action();

	size_t init_selected_val = selected_val;
	u32 btn                  = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_LEFT) {
		selected_val -= 1;
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		selected_val += 1;
	}
	check_overflow();

	if (init_selected_val != selected_val) {
		on_selected->invoke(selected_val);
	}
}

void RangeMenuOption::select()
{
	on_selected->invoke(selected_val);
}

void RangeMenuOption::check_overflow()
{
	if (selected_val > max) {
		if (overflow_behavior == RangeMenuOption::CAP) {
			selected_val = max;
		} else {
			selected_val = min;
		}
	} else if (selected_val < min) {
		if (overflow_behavior == RangeMenuOption::CAP) {
			selected_val = min;
		} else {
			selected_val = max;
		}
	}
}

f32 RangeMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	x += j2d.print(x, z, "%s: ", title);

	if (overflow_behavior == RangeMenuOption::WRAP || selected_val > min) {
		x += j2d.print(x, z, "< ");
	}

	j2d.mCharColor.set(p2gz->menu->color_std);
	j2d.mGradientColor.set(p2gz->menu->color_std);
	x += j2d.print(x, z, "%d", selected_val);

	if (selected) {
		j2d.mCharColor.set(p2gz->menu->color_highlight);
		j2d.mGradientColor.set(p2gz->menu->color_highlight);
	}
	if (overflow_behavior == RangeMenuOption::WRAP || selected_val < max) {
		x += j2d.print(x, z, " >");
	}

	return x;
}

void FloatRangeMenuOption::update()
{
	p2gz->menu->block_open_close_action();

	size_t init_selected_val = selected_val;
	u32 btn                  = p2gz->controller->getButton();
	f32 delta_per_frame      = (max - min) / 90.0f; // takes 3 seconds to go from one side to the other
	if (btn & Controller::PRESS_DPAD_LEFT) {
		selected_val -= delta_per_frame;
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		selected_val += delta_per_frame;
	}
	check_overflow();

	if (init_selected_val != selected_val) {
		on_selected->invoke(selected_val);
	}
}

void FloatRangeMenuOption::select()
{
	on_selected->invoke(selected_val);
}

void FloatRangeMenuOption::check_overflow()
{
	if (selected_val > max) {
		selected_val = max;
	} else if (selected_val < min) {
		selected_val = min;
	}
}

f32 FloatRangeMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	x += j2d.print(x, z, "%s: ", title);

	if (selected_val > min) {
		x += j2d.print(x, z, "< ");
	}

	j2d.mCharColor.set(p2gz->menu->color_std);
	j2d.mGradientColor.set(p2gz->menu->color_std);
	x += j2d.print(x, z, "%.2f", selected_val);

	if (selected) {
		j2d.mCharColor.set(p2gz->menu->color_highlight);
		j2d.mGradientColor.set(p2gz->menu->color_highlight);
	}
	if (selected_val < max) {
		x += j2d.print(x, z, " >");
	}

	return x;
}

void CutsceneToggleMenuOption::update()
{
	// p2gz->menu->block_open_close_action();

	switch (cutscene_id) {
	// Depending on which cutscene this option belongs to, check different points to see if the cutscene was already played or not
	// Update cutscene_played flag with this value so it's up-to-date in case a cutscene played recently
	case P2GZ_CT_MET_RED_PIKI: {
		cutscene_played = Game::playData->isDemoFlag(Game::DEMO_Meet_Red_Pikmin);
	} break;
	}
}

f32 CutsceneToggleMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	return j2d.print(x, z, "%s: %s", title, cutscene_played ? "already played" : "not played yet");
}
