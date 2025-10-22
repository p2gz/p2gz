#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/NaviTools.h>
#include <p2gz/FreeCam.h>
#include <p2gz/DayEditor.h>
#include <p2gz/WaypointViewer.h>
#include <p2gz/SprayEditor.h>
#include <p2gz/BoundDelegate.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <System.h>
#include <Controller.h>
#include <Dolphin/os.h>
#include <string.h>
#include <IDelegate.h>

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
			->push(new HexInputOption("seed", "random"))
			->push(new RangeMenuOption("day", 1, 99, 3, RangeMenuOption::CAP, new Delegate1<Warp, s32>(p2gz->warp, &Warp::set_warp_day)))
			->push(new PerformActionMenuOption("go", new Delegate<Warp>(p2gz->warp, &Warp::do_warp)))
		))
		->push(new OpenSubMenuOption("items", (new ListMenu())
			->push(new OpenSubMenuOption("sprays", (new ListMenu())
				->push(new RangeMenuOption("bitters", 0, 99, 0, RangeMenuOption::WRAP, new Delegate1<SprayEditor, s32>(p2gz->spray_editor, &SprayEditor::set_bitters)))
				->push(new RangeMenuOption("spicies", 0, 99, 0, RangeMenuOption::WRAP, new Delegate1<SprayEditor, s32>(p2gz->spray_editor, &SprayEditor::set_spicies)))
				->push(new ToggleMenuOption("bitters unlocked", true, new Delegate1<SprayEditor, bool>(p2gz->spray_editor, &SprayEditor::toggle_bitters)))
				->push(new ToggleMenuOption("spicies unlocked", true, new Delegate1<SprayEditor, bool>(p2gz->spray_editor, &SprayEditor::toggle_spicies)))
			))
		))
        ->push(new OpenSubMenuOption("captain", (new ListMenu())
            ->push(new PerformActionMenuOption("kill", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::kill)))
            ->push(new PerformActionMenuOption("boing", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::jump)))
        ))
		->push(new OpenSubMenuOption("map", (new ListMenu())
			->push(new OpenSubMenuOption("structures", (new ListMenu())
				->push(new OpenSubMenuOption("gates", (new ListMenu()))) // Will be populated dynamically by StructureEditor
			))
			->push(new ToggleMenuOption("collision viewer", false, new Delegate1<CollisionViewer, bool>(p2gz->collision_viewer, &CollisionViewer::toggle)))
			->push(new ToggleMenuOption("waypoint viewer", false, new Delegate1<WaypointViewer, bool>(p2gz->waypoint_viewer, &WaypointViewer::toggle)))
		))
        ->push(new OpenSubMenuOption("settings", (new ListMenu())
            ->push(new PerformActionMenuOption("increase text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::increase_text_size)))
            ->push(new PerformActionMenuOption("decrease text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::decrease_text_size)))
            ->push(new ToggleMenuOption("skippable treasure cutscenes", true, new Delegate1<SkippableTreasureCS, bool>(p2gz->skippable_treasure_cutscenes, &SkippableTreasureCS::toggle_skippable)))
            ->push(new ToggleMenuOption("skip save prompts", false, new Delegate1<SkipSave, bool>(p2gz->skip_save, &SkipSave::toggle_save_skip)))
			->push(new ToggleMenuOption("image test", true, nullptr, "red_leaf", false))
			->push(new OpenSubMenuOption("grid menu demo", (new GridMenu(24.0))
				->push_to_row(new OpenSubMenuOption("1", nullptr))->push_to_row(new OpenSubMenuOption("2", nullptr))->push_to_row(new OpenSubMenuOption("3", nullptr))->end_row()
				->push_to_row(new OpenSubMenuOption("4", nullptr))->push_to_row(new OpenSubMenuOption("5", nullptr))->push_to_row(new OpenSubMenuOption("6", nullptr))->end_row()
				->push_to_row(new OpenSubMenuOption("7", nullptr))->push_to_row(new OpenSubMenuOption("8", nullptr))->push_to_row(new OpenSubMenuOption("9", nullptr))
			))
        ))
		->push(new OpenSubMenuOption("tools", (new ListMenu())
			->push(new PerformActionMenuOption("freecam", new Delegate<FreeCam>(p2gz->freecam, &FreeCam::enable)))
			->push(new ToggleMenuOption("toggle heap bar", false, new Delegate1<HeapBarToggle, bool>(p2gz->heap_bar_toggle, &HeapBarToggle::toggle_heapbar)))
			->push(new OpenSubMenuOption("time controls", (new ListMenu())
				->push(new ToggleMenuOption("pause time", false, new Delegate1<DayEditor, bool>(p2gz->day_editor, &DayEditor::set_time_paused)))
				->push(new FloatRangeMenuOption("time", 7.0, 19.0, 7.0, new Delegate1<DayEditor, f32>(p2gz->day_editor, &DayEditor::set_time)))
			))
		))
		->push(new OpenSubMenuOption("timer", (new ListMenu())
			->push(new ToggleMenuOption("enabled", true, new Delegate1<Timer, bool>(p2gz->timer, &Timer::set_enabled)))
			->push(new ToggleMenuOption("show sub-timer", true, new Delegate1<Timer, bool>(p2gz->timer, &Timer::set_sub_timer_enabled)))
			->push(new PerformActionMenuOption("reset", new Delegate<Timer>(p2gz->timer, &Timer::reset_main_timer)))
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

		// If A or B button was pressed, set the lock for one frame (disable accidental inputs for other submenus as we navigate between
		// menus)
		if (enabled && ((controller->getButton() & Controller::PRESS_A) || (controller->getButton() & Controller::PRESS_B))) {
			set_lock();
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
	set_lock();
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
			if (options[selected_row]->len() <= selected_col) {
				selected_col = options[selected_row]->len() - 1;
			}
		} while (!cur_option()->visible);
	}
	if (btn & Controller::PRESS_DPAD_DOWN && options.len() > 0 && selected_row < options.len() - 1) {
		do {
			selected_row += 1;
			if (options[selected_row]->len() <= selected_col) {
				selected_col = options[selected_row]->len() - 1;
			}
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

HexKeypad::HexKeypad(const char* title_)
{
	title         = title_;
	value         = 0;
	cur_digit     = 0;
	is_unselected = true;

	// clang-format off
	keypad = (new GridMenu(16.0f))
		->push_to_row(new PerformActionMenuOption("0", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x0)))
		->push_to_row(new PerformActionMenuOption("1", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x1)))
		->push_to_row(new PerformActionMenuOption("2", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x2)))
		->push_to_row(new PerformActionMenuOption("3", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x3)))
		->end_row()
		->push_to_row(new PerformActionMenuOption("4", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x4)))
		->push_to_row(new PerformActionMenuOption("5", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x5)))
		->push_to_row(new PerformActionMenuOption("6", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x6)))
		->push_to_row(new PerformActionMenuOption("7", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x7)))
		->end_row()
		->push_to_row(new PerformActionMenuOption("8", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x8)))
		->push_to_row(new PerformActionMenuOption("9", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0x9)))
		->push_to_row(new PerformActionMenuOption("A", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0xA)))
		->push_to_row(new PerformActionMenuOption("B", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0xB)))
		->end_row()
		->push_to_row(new PerformActionMenuOption("C", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0xC)))
		->push_to_row(new PerformActionMenuOption("D", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0xD)))
		->push_to_row(new PerformActionMenuOption("E", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0xE)))
		->push_to_row(new PerformActionMenuOption("F", new BoundDelegate1<HexKeypad, u32>(this, &select_digit, 0xF)))
		->end_row()
		->push_to_row(new PerformActionMenuOption("submit", new Delegate<HexKeypad>(this, &submit)))
		->end_row()
		->push_to_row(new PerformActionMenuOption("use random seed", new Delegate<HexKeypad>(this, &set_unselected)));
	// clang-format on
}

void HexKeypad::select_digit(u32 digit)
{
	is_unselected = false;
	value |= digit << ((7 - cur_digit) * 4);
	if (cur_digit < 7) {
		cur_digit += 1;
	}
}

void HexKeypad::set_unselected()
{
	is_unselected = true;
	p2gz->menu->pop_layer();
}

void HexKeypad::submit()
{
	p2gz->menu->pop_layer();
}

void HexKeypad::update()
{
	u32 btn = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_R) {
		if (cur_digit >= 7) {
			cur_digit = 0;
		} else {
			cur_digit += 1;
		}
	}
	if (btn & Controller::PRESS_L) {
		if (cur_digit == 0) {
			cur_digit = 7;
		} else {
			cur_digit -= 1;
		}
	}

	keypad->update();
}

void HexKeypad::draw(J2DPrint& j2d, f32 x, f32 z)
{
	static const char* hex_digits = "0123456789ABCDEF";
	f32 initial_x                 = x;

	for (u8 i = 0; i < 8; i++) {
		bool is_selected = i == cur_digit;
		if (is_selected) {
			j2d.mCharColor.set(p2gz->menu->color_highlight);
			j2d.mGradientColor.set(p2gz->menu->color_highlight);
		} else {
			j2d.mCharColor.set(p2gz->menu->color_std);
			j2d.mGradientColor.set(p2gz->menu->color_std);
		}

		u8 digit = (value >> ((7 - i) * 4)) & 0xF;
		x += j2d.print(x, z, "%c", hex_digits[digit]);
	}

	z += p2gz->menu->line_height;
	keypad->column_width = p2gz->menu->line_height;
	keypad->draw(j2d, initial_x, z);
}

f32 MenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	f32 cursor = x;
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		cursor += p2gz->images->draw(image_name, cursor, z - p2gz->images->height());
		cursor += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		cursor += j2d.print(cursor, z, title);
	}

	return cursor;
}

f32 ToggleMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	f32 cursor = x;
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		cursor += p2gz->images->draw(image_name, cursor, z - p2gz->images->height());
		cursor += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		cursor += j2d.print(cursor, z, "%s: %s", title, on ? "true" : "false");
	}
	return cursor;
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
	if (sub_menu) {
		p2gz->menu->push_layer(sub_menu);
	}
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
	f32 cursor = x;
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		cursor += p2gz->images->draw(image_name, cursor, z - p2gz->images->height());
		cursor += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		cursor += j2d.print(cursor, z, "%s: < ", title);

		j2d.mCharColor.set(p2gz->menu->color_std);
		j2d.mGradientColor.set(p2gz->menu->color_std);
		cursor += j2d.print(cursor, z, options[selected_idx]);

		if (selected) {
			j2d.mCharColor.set(p2gz->menu->color_highlight);
			j2d.mGradientColor.set(p2gz->menu->color_highlight);
		}
		cursor += j2d.print(cursor, z, " >");
	}
	return cursor;
}

void RangeMenuOption::update()
{
	p2gz->menu->block_open_close_action();

	size_t init_selected_val = selected_val;
	u32 btn                  = p2gz->controller->getButtonDown();
	s32 delta                = 1;

	// If holding X, change in increments of 10. Just like how
	// onions in P1 Switch work.
	if (p2gz->controller->getButton() & Controller::PRESS_X) {
		delta = 10;
	}

	if (btn & Controller::PRESS_DPAD_LEFT) {
		selected_val -= delta;
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		selected_val += delta;
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
	f32 cursor = x;
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		cursor += p2gz->images->draw(image_name, cursor, z - p2gz->images->height());
		cursor += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		cursor += j2d.print(cursor, z, "%s: ", title);

		if (overflow_behavior == RangeMenuOption::WRAP || selected_val > min) {
			cursor += j2d.print(cursor, z, "< ");
		}

		j2d.mCharColor.set(p2gz->menu->color_std);
		j2d.mGradientColor.set(p2gz->menu->color_std);
		cursor += j2d.print(cursor, z, "%d", selected_val);

		if (selected) {
			j2d.mCharColor.set(p2gz->menu->color_highlight);
			j2d.mGradientColor.set(p2gz->menu->color_highlight);
		}
		if (overflow_behavior == RangeMenuOption::WRAP || selected_val < max) {
			cursor += j2d.print(cursor, z, " >");
		}
	}
	return cursor;
}

void FloatRangeMenuOption::update()
{
	p2gz->menu->block_open_close_action();

	size_t init_selected_val = selected_val;
	u32 btn                  = p2gz->controller->getButton();
	f32 delta_per_frame      = (max - min) / 90.0f; // takes 3 seconds to go from one side to the other

	// If holding X, move the selection faster
	if (p2gz->controller->getButton() & Controller::PRESS_X) {
		delta_per_frame *= 3.0f;
	}

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
	f32 cursor = x;
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		cursor += p2gz->images->draw(image_name, cursor, z - p2gz->images->height());
		cursor += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		cursor += j2d.print(cursor, z, "%s: ", title);

		if (selected_val > min) {
			cursor += j2d.print(cursor, z, "< ");
		}

		j2d.mCharColor.set(p2gz->menu->color_std);
		j2d.mGradientColor.set(p2gz->menu->color_std);
		cursor += j2d.print(cursor, z, "%.2f", selected_val);

		if (selected) {
			j2d.mCharColor.set(p2gz->menu->color_highlight);
			j2d.mGradientColor.set(p2gz->menu->color_highlight);
		}
		if (selected_val < max) {
			cursor += j2d.print(cursor, z, " >");
		}
	}
	return cursor;
}

HexInputOption::HexInputOption(const char* title_, const char* value_if_unselected_, const char* image_name_, bool image_only_)
    : MenuOption(title_, image_name_, image_only_)
{
	keypad              = new HexKeypad(title_);
	value_if_unselected = value_if_unselected_;
}

MenuLayer* HexInputOption::get_sub_menu()
{
	return keypad;
}

void HexInputOption::select()
{
	p2gz->menu->push_layer(keypad);
}

f32 HexInputOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	x = MenuOption::draw(j2d, x, z, selected);
	if (keypad->is_unselected) {
		return x + j2d.print(x, z, ": %s", value_if_unselected);
	}
	return x + j2d.print(x, z, ": %08X", keypad->get_value());
}

bool HexInputOption::is_selected()
{
	return !keypad->is_unselected;
}

u32 HexInputOption::get_selected_val()
{
	return keypad->get_value();
}
