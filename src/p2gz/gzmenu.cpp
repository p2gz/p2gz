#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/NaviTools.h>
#include <p2gz/FreeCam.h>
#include <p2gz/DayEditor.h>
#include <p2gz/WaypointViewer.h>
#include <p2gz/SquadEditor.h>
#include <p2gz/SprayEditor.h>
#include <p2gz/BoundDelegate.h>
#include <p2gz/EnemyDebugInfo.h>
#include <p2gz/InputHelpers.h>
#include <p2gz/Preset.h>
#include <p2gz/DismissPositions.h>
#include <p2gz/PokoEditor.h>
#include <p2gz/OnionEditor.h>
#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <System.h>
#include <Controller.h>
#include <Dolphin/os.h>
#include <string.h>
#include <IDelegate.h>
#include <Graphics.h>
#include <Game/MoviePlayer.h>

using namespace gz;

#define COLOR(color)           \
	j2d.mCharColor.set(color); \
	j2d.mGradientColor.set(color)

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
	color_highlight   = JUtility::TColor(232, 40, 40, 255);
	color_breadcrumbs = JUtility::TColor(247, 197, 86, 255);
}

void GZMenu::init_menu()
{
	// clang-format off
	// Structure of GZ menu defined here:
    root_layer = (new ListMenu())
		->push(new OpenSubMenuOption("warp", (new ListMenu(new Delegate<Warp>(p2gz->warp, &Warp::sync)))
			->push(new RadioMenuOption("area", new Delegate1<Warp, size_t>(p2gz->warp, &Warp::set_warp_area)))
			->push(new RadioMenuOption("cave", new Delegate1<Warp, size_t>(p2gz->warp, &Warp::set_warp_cave)))
			->push(new RangeMenuOption("sublevel", 1, 14, 1, RangeMenuOption::WRAP, new Delegate1<Warp, s32>(p2gz->warp, &Warp::set_warp_sublevel)))
			->push(new HexInputOption("seed", "random", new Delegate1<Warp, u32>(p2gz->warp, &Warp::set_seed), new Delegate<Warp>(p2gz->warp, &Warp::set_random_seed)))
			->push(new RadioMenuOption("enter method", new Delegate1<Warp, size_t>(p2gz->warp, &Warp::set_enter_area_type)))
			->push(new RadioMenuOption("captain", new Delegate1<Warp, size_t>(p2gz->warp, &Warp::set_active_captain)))
			->push(new RangeMenuOption("day", 1, 99, 5, RangeMenuOption::CAP, new Delegate1<Warp, s32>(p2gz->warp, &Warp::set_warp_day)))
			->push(new PresetMenuOption(new Delegate2<Warp, PresetPreview*, int>(p2gz->warp, &Warp::set_preset)))
			->push(new PerformActionMenuOption("go", new Delegate<Warp>(p2gz->warp, &Warp::do_warp)))
		))
		->push(new PerformActionMenuOption("freecam", new Delegate<FreeCam>(p2gz->freecam, &FreeCam::enable)))
		->push(new OpenSubMenuOption("pikmin", (new ListMenu())
			->push(new OpenSubMenuOption("squad", (new GridMenu(100.0f, 36.0f, new Delegate<SquadEditor>(p2gz->squad_editor, &SquadEditor::sync)))))
			->push(new OpenSubMenuOption("onions", (new GridMenu(100.0f, 36.0f, new Delegate<OnionEditor>(p2gz->onion_editor, &OnionEditor::sync)))))
			->push(new PerformActionMenuOption("clear all", new Delegate<SquadEditor>(p2gz->squad_editor, &SquadEditor::clear_field_pikmin)))
		))
		->push(new OpenSubMenuOption("trainers", (new ListMenu())
			->push(new PerformActionMenuOption("fast empress", new Delegate<EmpressTrainer>(p2gz->empress_trainer, &EmpressTrainer::start)))
		))
		->push(new OpenSubMenuOption("captain", (new ListMenu())
			->push(new DecimalInputOption("pokos", new Delegate1<PokoEditor, u32>(p2gz->poko_editor, &PokoEditor::set_pokos), new Delegate<PokoEditor>(p2gz->poko_editor, &PokoEditor::sync)))
			->push(new OpenSubMenuOption("sprays", (new ListMenu(new Delegate<SprayEditor>(p2gz->spray_editor, &SprayEditor::sync)))
				->push(new RangeMenuOption("bitters", 0, 99, 0, RangeMenuOption::WRAP, new Delegate1<SprayEditor, s32>(p2gz->spray_editor, &SprayEditor::set_bitters)))
				->push(new RangeMenuOption("spicies", 0, 99, 0, RangeMenuOption::WRAP, new Delegate1<SprayEditor, s32>(p2gz->spray_editor, &SprayEditor::set_spicies)))
				->push(new ToggleMenuOption("bitters unlocked", true, new Delegate1<SprayEditor, bool>(p2gz->spray_editor, &SprayEditor::toggle_bitters)))
				->push(new ToggleMenuOption("spicies unlocked", true, new Delegate1<SprayEditor, bool>(p2gz->spray_editor, &SprayEditor::toggle_spicies)))
			))
			->push(new OpenSubMenuOption("upgrades", new ListMenu(new Delegate<EKEditor>(p2gz->ek_editor, &EKEditor::check_upgrades))))
			->push(new ToggleMenuOption("boing mode", false, new Delegate1<NaviTools, bool>(p2gz->navi_tools, &NaviTools::set_boing_mode)))
			->push(new FloatRangeMenuOption("captain health", 0.1f, 50.0f, 50.0f, new Delegate1<NaviTools, f32>(p2gz->navi_tools, &NaviTools::set_active_navi_hp)))
            ->push(new PerformActionMenuOption("kill captain", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::kill)))
		))

		// Options that edit the current level state
		->push(new OpenSubMenuOption("level", (new ListMenu())
			->push(new OpenSubMenuOption("treasures", (new ListMenu(new Delegate<TreasureEditor>(p2gz->treasure_editor, &TreasureEditor::sync)))))
			->push(new RadioMenuOption("treasure region", new Delegate1<Settings, size_t>(p2gz->settings, &Settings::set_treasure_region)))
			->push(new OpenSubMenuOption("gates", (new ListMenu(new Delegate<StructureEditor>(p2gz->structure_editor, &StructureEditor::sync_gates))))) // Will be populated dynamically by StructureEditor
			->push(new OpenSubMenuOption("bridges", (new ListMenu(new Delegate<StructureEditor>(p2gz->structure_editor, &StructureEditor::sync_bridges))))) // Will be populated dynamically by StructureEditor
			->push(new OpenSubMenuOption("plugs", (new ListMenu(new Delegate<StructureEditor>(p2gz->structure_editor, &StructureEditor::sync_plugs))))) // Will be populated dynamically by StructureEditor
			->push(new OpenSubMenuOption("bags", (new ListMenu(new Delegate<StructureEditor>(p2gz->structure_editor, &StructureEditor::sync_bags))))) // Will be populated dynamically by StructureEditor
			->push(new OpenSubMenuOption("time of day", (new ListMenu(new Delegate<DayEditor>(p2gz->day_editor, &DayEditor::sync)))
				->push(new ToggleMenuOption("pause time", false, new Delegate1<DayEditor, bool>(p2gz->day_editor, &DayEditor::set_time_paused)))
				->push(new FloatRangeMenuOption("current time", 7.0, 19.0, 7.0, new Delegate1<DayEditor, f32>(p2gz->day_editor, &DayEditor::set_time)))
			))
		))
		->push(new OpenSubMenuOption("cutscenes", (new ListMenu()) /* Submenus get added in CutsceneMgr::init */))

		// All viewers for any kind of debug info should go here
		->push(new OpenSubMenuOption("debug info", (new ListMenu())
			->push(new ToggleMenuOption("dismiss positions", false, new Delegate1<DismissPositions, bool>(p2gz->dismiss_positions, &DismissPositions::toggle)))
			->push(new OpenSubMenuOption("enemy debug info", (new ListMenu())
				->push(new ToggleMenuOption("enable", false, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_enabled)))
				->push(new OpenSubMenuOption("display settings", (new ListMenu())
					->push(new FloatRangeMenuOption("max display distance", 100.0f, 2048.0f, 512.0f, new Delegate1<EnemyDebugInfo, f32>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_max_display_dist)))
					->push(new RangeMenuOption("text size", 1, 20, 10, RangeMenuOption::CAP, new Delegate1<EnemyDebugInfo, s32>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_size)))
				))
				->push(new ToggleMenuOption("draw enemy name", true, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_enemy_name_enabled)))
				->push(new ToggleMenuOption("draw current state", true, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_cur_state_enabled)))
				->push(new ToggleMenuOption("draw flick count", true, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_flick_count_enabled)))
				->push(new ToggleMenuOption("draw position", false, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_position_enabled)))
				->push(new ToggleMenuOption("draw collision", false, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_collision_enabled)))
				->push(new ToggleMenuOption("draw timers", false, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_timers_enabled)))
				->push(new ToggleMenuOption("draw stuck pikmin", false, new Delegate1<EnemyDebugInfo, bool>(p2gz->enemy_debug_info, &EnemyDebugInfo::set_draw_stuck_pikmin_enabled)))
			))
			->push(new OpenSubMenuOption("captain debug info", (new ListMenu())
				->push(new ToggleMenuOption("enable", false, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_enabled)))
				->push(new ToggleMenuOption("draw position", true, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_draw_position_enabled)))
				->push(new ToggleMenuOption("draw velocity", true, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_draw_velocity_enabled)))
				->push(new ToggleMenuOption("draw target velocity", false, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_draw_target_velocity_enabled)))
				->push(new ToggleMenuOption("draw face angle", true, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_draw_face_dir_enabled)))
				->push(new ToggleMenuOption("draw current state", true, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_draw_state_enabled)))
				->push(new ToggleMenuOption("show captain swap time", true, new Delegate1<NaviDebugInfo, bool>(p2gz->navi_debug_info, &NaviDebugInfo::set_draw_swap_time_enabled)))
			))
			->push(new OpenSubMenuOption("treasure debug info", (new ListMenu())
				->push(new ToggleMenuOption("enable", false, new Delegate1<TreasureDebugInfo, bool>(p2gz->treasure_debug_info, &TreasureDebugInfo::set_enabled)))
				->push(new ToggleMenuOption("draw name and id", true, new Delegate1<TreasureDebugInfo, bool>(p2gz->treasure_debug_info, &TreasureDebugInfo::set_draw_name_id_enabled)))
				->push(new ToggleMenuOption("draw position", true, new Delegate1<TreasureDebugInfo, bool>(p2gz->treasure_debug_info, &TreasureDebugInfo::set_draw_position_enabled)))
				->push(new ToggleMenuOption("draw carry power", true, new Delegate1<TreasureDebugInfo, bool>(p2gz->treasure_debug_info, &TreasureDebugInfo::set_draw_carry_power_enabled)))
				->push(new ToggleMenuOption("draw velocity", false, new Delegate1<TreasureDebugInfo, bool>(p2gz->treasure_debug_info, &TreasureDebugInfo::set_draw_velocity_enabled)))
			))
			->push(new ToggleMenuOption("generator debug info", false, new Delegate1<GeneratorDebugInfo, bool>(p2gz->generator_debug_info, &GeneratorDebugInfo::set_enabled)))
			->push(new ToggleMenuOption("collision viewer", false, new Delegate1<CollisionViewer, bool>(p2gz->collision_viewer, &CollisionViewer::toggle)))
			->push(new ToggleMenuOption("waypoint viewer", false, new Delegate1<WaypointViewer, bool>(p2gz->waypoint_viewer, &WaypointViewer::toggle)))
			->push(new ToggleMenuOption("spawn point viewer", false, new Delegate1<CaveDebugInfo, bool>(p2gz->cave_debug_info, &CaveDebugInfo::set_draw_spawn_points)))
			->push(new ToggleMenuOption("gate debug info", false, new Delegate1<StructureEditor, bool>(p2gz->structure_editor, &StructureEditor::set_enabled_gate_debug)))
			->push(new ToggleMenuOption("bridge debug info", false, new Delegate1<StructureEditor, bool>(p2gz->structure_editor, &StructureEditor::set_enabled_bridge_debug)))
			->push(new ToggleMenuOption("plug debug info", false, new Delegate1<StructureEditor, bool>(p2gz->structure_editor, &StructureEditor::set_enabled_plug_debug)))
			->push(new ToggleMenuOption("heap memory usage", false, new Delegate1<HeapBarToggle, bool>(p2gz->heap_bar_toggle, &HeapBarToggle::toggle_heapbar)))
		))
		->push(new OpenSubMenuOption("timer", (new ListMenu(new Delegate<Timer>(p2gz->timer, &Timer::sync)))
			->push(new ToggleMenuOption("enabled", p2gz->settings->bool_settings[SETTING_timer_enabled], new CurriedDelegate1<Settings, SettingId, bool>(p2gz->settings, &Settings::set_bool, SETTING_timer_enabled)))
			->push(new ToggleMenuOption("show sub-timer", p2gz->settings->bool_settings[SETTING_timer_subtimer], new CurriedDelegate1<Settings, SettingId, bool>(p2gz->settings, &Settings::set_bool, SETTING_timer_subtimer)))
			->push(new PerformActionMenuOption("reset", new Delegate<Timer>(p2gz->timer, &Timer::on_reset)))
		))

		// General game behaviors and options for how the gz menu looks and behaves
		->push(new OpenSubMenuOption("settings", (new ListMenu())
            ->push(new ToggleMenuOption("skippable cutscenes", p2gz->settings->bool_settings[SETTING_cutscenes_skippable], new CurriedDelegate1<Settings, SettingId, bool>(p2gz->settings, &Settings::set_bool, SETTING_cutscenes_skippable)))
            ->push(new ToggleMenuOption("skip save prompts", p2gz->settings->bool_settings[SETTING_skip_save_prompts], new CurriedDelegate1<Settings, SettingId, bool>(p2gz->settings, &Settings::set_bool, SETTING_skip_save_prompts)))
			->push(new ToggleMenuOption("allow 0 pikmin in caves", true, new Delegate1<Warp, bool>(p2gz->warp, &Warp::set_allow_zero_piki_in_caves)))
			->push(new ToggleMenuOption("eggs always drop mitites", false, new Delegate1<DropEditor, bool>(p2gz->drop_editor, &DropEditor::set_egg_always_mitites)))
			->push(new PerformActionMenuOption("increase text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::increase_text_size)))
            ->push(new PerformActionMenuOption("decrease text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::decrease_text_size)))
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
	// above a certain text size is ridiculous
	if (glyph_width > 35.0f) {
		return;
	}
	glyph_width += 2.0;
	glyph_height += 2.0;
	line_height += 2.0;
}

void GZMenu::decrease_text_size()
{
	// below a certain text size is ridiculous
	if (glyph_width < 7.0f) {
		return;
	}
	glyph_width -= 2.0;
	glyph_height -= 2.0;
	line_height -= 2.0;
}

void focus_captain()
{
	if (!Game::cameraMgr || !Game::naviMgr) {
		return;
	}

	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (!navi) {
		return;
	}

	Game::PlayCamera* camera = Game::cameraMgr->mCameraObjList[navi->getNaviID()];
	if (camera) {
		camera->changeTargetAtPosition();
	}
}

void GZMenu::push_layer(MenuLayer* layer_)
{
	if (layer_) {
		layer = layer_;
		layer->reset_selection();
		if (layer->title) {
			breadcrumbs.push(layer->title);
		}
		if (layer->on_opened) {
			layer->on_opened->invoke();
		}
		MenuOption* initial_option = layer->cur_option();
		if (initial_option && initial_option->on_focus) {
			initial_option->on_focus->invoke();
		}
	}
}

void GZMenu::pop_layer()
{
	if (layer->parent) {
		layer = layer->parent;
		breadcrumbs.pop();
		focus_captain();
		if (layer->on_opened) {
			layer->on_opened->invoke();
			MenuOption* initial_option = layer->cur_option();
			if (initial_option && initial_option->on_focus) {
				initial_option->on_focus->invoke();
			}
		}
	} else {
		close();
	}
}

void GZMenu::open()
{
	if (enabled)
		return;

	// If freecam is active, don't open the menu
	if (p2gz->freecam && p2gz->freecam->is_enabled())
		return;

	// Don't open P2GZ menu during the following:
	// - cutscenes (causes gameplay desync) - One exception:
	// Note that technically a cutscene is playing in the background during day end results, so add exception to cutscene check
	// - loading (causes gameplay desync)
	// - end of day (causes gameplay desync)
	// - cave results but only in the very beginning before the blo loads (crashes when warping out)
	// - day results but only in the very beginning before the blo loads (crashes when warping out)
	if ((!in_end_of_day_result() && Game::moviePlayer->isFlag(Game::MVP_IsActive))
	    || (in_end_of_day_result() && !in_end_of_day_result_safe_to_warp()) || in_day_end_sunset() || in_load()
	    || (in_cave_results() && !in_cave_results_safe_to_warp()) || p2gz->warp->is_warp_lockout()) {
		return;
	}

	// Hide options that control current game state when editing those game states isn't valid.
	const bool in_gameplay         = in_cave_gameplay() || in_above_ground_gameplay();
	get_option("freecam")->visible = in_gameplay;
	get_option("pikmin")->visible  = in_gameplay;
	get_option("captain")->visible = in_gameplay;
	get_option("level")->visible   = in_gameplay;

	layer = root_layer;
	layer->reset_selection();
	breadcrumbs.clear();
	enabled = true;
	lock    = true;

	Game::gameSystem->setPause(true, "gzmenu", 3);
	p2gz->timer->pause();
}

void GZMenu::close()
{
	if (!enabled)
		return;

	enabled = false;
	// Don't unpause the game if we're in the enter cave/escape cave submenues
	Game::SingleGameSection* gameSec = get_SGS();
	if (!gameSec->mOpenMenuFlags) { // if nonzero, we're in at least one of the menus
		Game::gameSystem->setPause(false, "gzmenu", 3);
	}

	// don't unpause timer if we're in freecam mode (until we exit that mode)
	if (!p2gz->timer->is_freecam_mode()) {
		p2gz->timer->unpause();
	}
}

void GZMenu::draw()
{
	if (!enabled || !layer) {
		return;
	}

	controls.reset();

	J2DPrint j2d(gP2JMEMgr->mFont, 0.0f);
	j2d.initiate();
	j2d.mGlyphWidth  = glyph_width;
	j2d.mGlyphHeight = glyph_height;

	f32 x = breadcrumb_start_offset_x;
	f32 z = start_offset_z;

	if (breadcrumbs.len() > 0) {
		for (size_t i = 0; i < breadcrumbs.len(); i++) {
			COLOR(color_std);
			x += j2d.print(x, z, " > ");

			COLOR(color_breadcrumbs);
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

MenuLayer::~MenuLayer()
{
	if (on_opened) {
		delete on_opened;
	}
}

ListMenu::~ListMenu()
{
	clear();
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

ListMenu* ListMenu::push(MenuOption* option)
{
	options.push(option);
	MenuLayer* sub_menu = option->get_sub_menu();
	if (sub_menu) {
		sub_menu->parent = this;
	}
	return this;
}

void ListMenu::clear()
{
	for (size_t i = 0; i < options.len(); i++) {
		delete options[i];
	}
	options.clear();
}

void ListMenu::update()
{
	// Make sure the selection starts in bounds in case it was changed from somewhere else
	if (selected < scroll) {
		selected = scroll;
	}

	u32 btn = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_B) {
		p2gz->menu->pop_layer();
		return;
	}

	if (options.len() == 0) {
		return;
	}

	// Adjust selection if currently selected option is hidden
	size_t num_checks = 0;
	while (options.len() > 0 && !options[selected]->visible) {
		selected = selected + 1;
		if (selected >= options.len()) {
			selected = 0;
		}
		num_checks += 1;
		if (num_checks >= options.len()) {
			return;
		}
	}

	// handle inputs
	if (pah_up.check(p2gz->controller)) {
		if (selected == 0) {
			selected = options.len();
		}
		do {
			selected = (selected + options.len() - 1) % options.len(); // subtract with wrap
		} while (!options[selected]->visible);

		if (options[selected]->on_focus) {
			options[selected]->on_focus->invoke();
		}
	}
	if (pah_down.check(p2gz->controller) && options.len() > 0) {
		if (selected >= options.len() - 1) {
			selected = -1;
		}
		do {
			selected += 1;
		} while (!options[selected]->visible);

		if (options[selected]->on_focus) {
			options[selected]->on_focus->invoke();
		}
	}
	if (btn & Controller::PRESS_A) {
		options[selected]->select();
	}

	// ... but if we scrolled off the edge deliberately, adjust the scroll
	if (selected < scroll) {
		scroll = selected;
	}

	if (options.len() > 0 && selected < options.len() && options[selected]->visible) {
		options[selected]->update();
	}
}

void ListMenu::draw(J2DPrint& j2d, f32& x, f32& z)
{
	const f32 start_x = x;
	const f32 start_z = z;
	for (size_t i = scroll; i < options.len(); i++) {
		if (!options[i]->visible || z < start_z) {
			continue;
		}

		bool is_selected = i == selected;

		if (is_selected) {
			COLOR(p2gz->menu->color_highlight);
		} else {
			COLOR(p2gz->menu->color_std);
		}

		options[i]->draw(j2d, x, z, i == selected);
		z += p2gz->menu->line_height;
		x = start_x;

		// Don't need to draw things too far down on the screen
		if (z > 400.0f) {
			if (selected > i) { // Make sure selection stays within visible range
				scroll += 1;
			}
			break;
		}
	}

	p2gz->menu->draw_control(j2d, Controller::PRESS_B, "back");
}

GridMenu::~GridMenu()
{
	for (size_t row = 0; row < options.len(); row++) {
		for (size_t col = 0; col < options[row]->len(); col++) {
			delete (*options[row])[col];
		}
		delete options[row];
	}
}

GridMenu* GridMenu::push_to_row(MenuOption* option)
{
	options[selected_row]->push(option);
	MenuLayer* sub_menu = option->get_sub_menu();
	if (sub_menu) {
		sub_menu->parent = this;
	}
	return this;
}

GridMenu* GridMenu::end_row()
{
	selected_row += 1;
	options.push(new Vec<MenuOption*>);
	return this;
}

void GridMenu::update()
{
	p2gz->menu->block_open_close_action();

	u32 btn = p2gz->controller->getButtonDown();
	if (editing_option) {
		if (btn & Controller::PRESS_A || btn & Controller::PRESS_B) {
			editing_option = false;
		} else {
			cur_option()->update();
		}
	} else {
		if (pah_up.check(p2gz->controller)) {
			if (selected_row == 0) {
				selected_row = options.len();
			}
			do {
				selected_row -= 1;
				if (options[selected_row]->len() <= selected_col) {
					selected_col = options[selected_row]->len() - 1;
				}
			} while (!cur_option()->visible);
		}
		if (pah_down.check(p2gz->controller) && options.len() > 0) {
			if (selected_row >= options.len() - 1) {
				selected_row = -1;
			}
			do {
				selected_row += 1;
				if (options[selected_row]->len() <= selected_col) {
					selected_col = options[selected_row]->len() - 1;
				}
			} while (!cur_option()->visible);
		}
		if (pah_left.check(p2gz->controller)) {
			if (selected_col == 0) {
				selected_col = options[selected_row]->len();
			}
			do {
				selected_col -= 1;
			} while (!cur_option()->visible);
		}
		if (pah_right.check(p2gz->controller) && options[selected_row]->len() > 0) {
			if (selected_col >= options[selected_row]->len() - 1) {
				selected_col = -1;
			}
			do {
				selected_col += 1;
			} while (!cur_option()->visible);
		}

		if (btn & Controller::PRESS_A) {
			editing_option = cur_option()->select();
		}
		if (btn & Controller::PRESS_B) {
			p2gz->menu->pop_layer();
		}
	}
}

void GridMenu::draw(J2DPrint& j2d, f32& x, f32& z)
{
	const f32 start_x = x;
	for (size_t row_idx = 0; row_idx < options.len(); row_idx++) {
		for (size_t col_idx = 0; col_idx < options[row_idx]->len(); col_idx++) {
			MenuOption* option = (*options[row_idx])[col_idx];
			if (!option || !option->visible) {
				continue;
			}
			bool is_selected = col_idx == selected_col && row_idx == selected_row;

			if (is_selected) {
				if (editing_option) {
					COLOR(p2gz->menu->color_highlight);
				} else {
					COLOR(p2gz->menu->color_breadcrumbs);
				}
			} else {
				COLOR(p2gz->menu->color_std);
			}

			const f32 col_start_x = x;
			option->draw(j2d, x, z, is_selected && editing_option);
			x = col_start_x + opt_width;
		}
		x = start_x;
		z += opt_height;
	}

	if (!editing_option) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "select");
		p2gz->menu->draw_control(j2d, Controller::PRESS_B, "back");
	} else {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "done");
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

HexKeypad::HexKeypad(const char* title_, const char* cancel_text_, IDelegate1<u32>* on_selected_, IDelegate* on_unselected_)
{
	title         = title_;
	value         = 0;
	cur_digit     = 0;
	unselected    = true;
	cancel_text   = cancel_text_;
	on_selected   = on_selected_;
	on_unselected = on_unselected_;

	// clang-format off
	keypad = (new GridMenu(16.0f, p2gz->menu->line_height))
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
		->push_to_row(new PerformActionMenuOption(cancel_text, new Delegate<HexKeypad>(this, &set_unselected_and_pop)));
	// clang-format on
}

HexKeypad::~HexKeypad()
{
	if (on_selected) {
		delete on_selected;
	}
	if (on_unselected) {
		delete on_unselected;
	}
	delete keypad;
}

void HexKeypad::select_digit(u32 digit)
{
	unselected = false;

	const u32 shift_amt   = (7 - cur_digit) * 4; // digit 0 = most significant bit; shift 4 bits for each hex char
	const u32 clear_digit = (0xFFFFFFF0 << shift_amt) | (0xFFFFFFF0 >> (32 - shift_amt)); // bit rotate
	value &= clear_digit;
	value |= digit << shift_amt;

	if (cur_digit < 7) {
		cur_digit += 1;
	}
	if (on_selected) {
		on_selected->invoke(value);
	}
}

void HexKeypad::set_unselected_and_pop()
{
	unselected = true;
	if (on_unselected) {
		on_unselected->invoke();
	}
	p2gz->menu->pop_layer();
}

void HexKeypad::submit()
{
	unselected = false;
	if (on_selected) {
		on_selected->invoke(value);
	}
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

void HexKeypad::draw(J2DPrint& j2d, f32& x, f32& z)
{
	static const char* hex_digits = "0123456789ABCDEF";
	f32 initial_x                 = x;

	for (u8 i = 0; i < 8; i++) {
		bool is_selected = i == cur_digit;
		if (is_selected) {
			COLOR(p2gz->menu->color_highlight);
		} else {
			COLOR(p2gz->menu->color_std);
		}

		u8 digit = (value >> ((7 - i) * 4)) & 0xF;
		x += j2d.print(x, z, "%c", hex_digits[digit]);
	}

	z += p2gz->menu->line_height;
	keypad->opt_width = p2gz->menu->line_height;
	x                 = initial_x;
	keypad->draw(j2d, x, z);

	p2gz->menu->draw_control(j2d, Controller::PRESS_L, ""); // display L and R next to each other
	p2gz->menu->draw_control(j2d, Controller::PRESS_R, "move cursor");
}

DecimalKeypad::DecimalKeypad(const char* title_, IDelegate1<u32>* on_selected_, IDelegate* on_opened_)
    : MenuLayer(on_opened_)
{
	title         = title_;
	value         = 0;
	cur_digit     = 0;
	is_unselected = true;
	on_selected   = on_selected_;

	// clang-format off
	keypad = (new GridMenu(16.0f, p2gz->menu->line_height))
		->push_to_row(new PerformActionMenuOption("0", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 0)))
		->push_to_row(new PerformActionMenuOption("1", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 1)))
		->push_to_row(new PerformActionMenuOption("2", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 2)))
		->push_to_row(new PerformActionMenuOption("3", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 3)))
		->push_to_row(new PerformActionMenuOption("4", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 4)))
		->end_row()
		->push_to_row(new PerformActionMenuOption("5", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 5)))
		->push_to_row(new PerformActionMenuOption("6", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 6)))
		->push_to_row(new PerformActionMenuOption("7", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 7)))
		->push_to_row(new PerformActionMenuOption("8", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 8)))
		->push_to_row(new PerformActionMenuOption("9", new BoundDelegate1<DecimalKeypad, u32>(this, &select_digit, 9)));
	// clang-format on
}

DecimalKeypad::~DecimalKeypad()
{
	if (on_selected) {
		delete on_selected;
	}
	delete keypad;
}

void DecimalKeypad::select_digit(u32 digit)
{
	is_unselected = false;

	static const u32 pow10[5] = { 10000, 1000, 100, 10, 1 };

	u32 old_digit = (value / pow10[cur_digit]) % 10;
	value -= old_digit * pow10[cur_digit];
	value += digit * pow10[cur_digit];

	if (cur_digit < 4) {
		cur_digit += 1;
	}

	if (on_selected) {
		on_selected->invoke(value);
	}
}

void DecimalKeypad::set_unselected()
{
	is_unselected = true;
	p2gz->menu->pop_layer();
}

void DecimalKeypad::submit()
{
	is_unselected = false;
	if (on_selected) {
		on_selected->invoke(value);
	}
	p2gz->menu->pop_layer();
}

void DecimalKeypad::update()
{
	u32 btn = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_R) {
		if (cur_digit >= 4) {
			cur_digit = 0;
		} else {
			cur_digit += 1;
		}
	}
	if (btn & Controller::PRESS_L) {
		if (cur_digit == 0) {
			cur_digit = 4;
		} else {
			cur_digit -= 1;
		}
	}

	keypad->update();
}

void DecimalKeypad::draw(J2DPrint& j2d, f32& x, f32& z)
{
	static const char* decimal_digits = "0123456789";
	static const u32 pow10[5]         = { 10000, 1000, 100, 10, 1 };
	f32 initial_x                     = x;

	for (u8 i = 0; i < 5; i++) {
		bool is_selected = i == cur_digit;
		if (is_selected) {
			COLOR(p2gz->menu->color_highlight);
		} else {
			COLOR(p2gz->menu->color_std);
		}

		u8 digit = (value % 100000 / pow10[i]) % 10;
		x += j2d.print(x, z, "%c", decimal_digits[digit]);
	}

	z += p2gz->menu->line_height;
	keypad->opt_width = p2gz->menu->line_height;
	x                 = initial_x;
	keypad->draw(j2d, x, z);

	p2gz->menu->draw_control(j2d, Controller::PRESS_L, ""); // display L and R next to each other
	p2gz->menu->draw_control(j2d, Controller::PRESS_R, "move cursor");
}

void MenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		x += p2gz->images->draw(image_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
		x += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title && !image_only) {
		x += j2d.print(x, z, title);
	}
}

ToggleMenuOption::~ToggleMenuOption()
{
	if (on_selected) {
		delete on_selected;
	}
}

void ToggleMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		x += p2gz->images->draw(image_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
		x += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}

	if (image_only) {
		x += j2d.print(x, z, "%s", on ? "true" : "false");
	} else if (title) {
		x += j2d.print(x, z, "%s: %s", title, on ? "true" : "false");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "toggle");
	}
}

OpenSubMenuOption::OpenSubMenuOption(const char* title_, MenuLayer* sub_menu_)
    : MenuOption(title_)
    , sub_menu(sub_menu_)
{
	if (sub_menu) {
		sub_menu->title = title_;
	}
}

OpenSubMenuOption::~OpenSubMenuOption()
{
	{
		if (sub_menu) {
			delete sub_menu;
		}
	}
}

bool OpenSubMenuOption::select()
{
	if (sub_menu) {
		p2gz->menu->push_layer(sub_menu);
	}
	return false;
}

void OpenSubMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	MenuOption::draw(j2d, x, z, selected);
	x += j2d.print(x, z, " >");

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "open");
	}
}

PerformActionMenuOption::~PerformActionMenuOption()
{
	if (on_selected) {
		delete on_selected;
	}
}

void PerformActionMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	MenuOption::draw(j2d, x, z, selected);

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "select");
	}
}

RadioMenuOption::~RadioMenuOption()
{
	if (on_selected) {
		delete on_selected;
	}
}

void RadioMenuOption::update()
{
	p2gz->menu->block_open_close_action();

	size_t init_selected_idx = selected_idx;
	u32 btn                  = p2gz->controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_LEFT) {
		if (selected_idx == 0) {
			selected_idx = options.len() - 1;
		} else {
			selected_idx = selected_idx - 1;
		}
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		selected_idx = selected_idx + 1;
		if (selected_idx >= options.len()) {
			selected_idx = 0;
		}
	}

	if (init_selected_idx != selected_idx) {
		if (on_selected) {
			on_selected->invoke(selected_idx);
		}
	}
}

bool RadioMenuOption::select()
{
	if (on_selected) {
		on_selected->invoke(selected_idx);
	}
	return true;
}

void RadioMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		x += p2gz->images->draw(image_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
		x += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title) {
		if (image_only) {
			x += j2d.print(x, z, " < ");
		} else {
			x += j2d.print(x, z, "%s: < ", title);
		}

		COLOR(p2gz->menu->color_std);
		x += j2d.print(x, z, options[selected_idx]);

		if (selected) {
			COLOR(p2gz->menu->color_highlight);
		}
		x += j2d.print(x, z, " >");
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_DPAD_LEFT, "change value");
	}
}

RangeMenuOption::~RangeMenuOption()
{
	if (on_selected) {
		delete on_selected;
	}
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
		update_selection(-delta);
	}
	if (btn & Controller::PRESS_DPAD_RIGHT) {
		update_selection(delta);
	}

	if (init_selected_val != selected_val) {
		if (on_selected) {
			on_selected->invoke(selected_val);
		}
	}
}

bool RangeMenuOption::select()
{
	if (on_selected) {
		on_selected->invoke(selected_val);
	}
	return true;
}

void RangeMenuOption::update_selection(s32 delta)
{
	switch (overflow_behavior) {
	case CAP:
		selected_val = MAX(MIN(selected_val + delta, max), min);
		break;
	case WRAP:
		if (selected_val == max && delta > 0) {
			selected_val = min;
		} else if (selected_val == min && delta < 0) {
			selected_val = max;
		} else {
			selected_val = MAX(MIN(selected_val + delta, max), min);
		}
		break;
	}
}

void RangeMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		x += p2gz->images->draw(image_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
		x += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}
	if (title) {
		JUtility::TColor arrow_color = j2d.mCharColor;

		if (!image_only) {
			x += j2d.print(x, z, "%s: ", title);
		}

		if (overflow_behavior == RangeMenuOption::WRAP || selected_val > min) {
			x += j2d.print(x, z, "< ");
		}

		COLOR(p2gz->menu->color_std);
		x += j2d.print(x, z, "%d", selected_val);

		COLOR(arrow_color);
		if (overflow_behavior == RangeMenuOption::WRAP || selected_val < max) {
			x += j2d.print(x, z, " >");
		}
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_DPAD_LEFT, "change value");
		p2gz->menu->draw_control(j2d, Controller::PRESS_X, "(hold) x10");
	}
}

void RangeMenuOption::set_bounds(s32 min_, s32 max_)
{
	GZASSERTLINE(min <= max);
	min = min_;
	max = max_;

	if (selected_val < min) {
		selected_val = min;
	} else if (selected_val > max) {
		selected_val = max;
	}
}

FloatRangeMenuOption::~FloatRangeMenuOption()
{
	if (on_selected) {
		delete on_selected;
	}
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
		if (on_selected) {
			on_selected->invoke(selected_val);
		}
	}
}

bool FloatRangeMenuOption::select()
{
	if (on_selected) {
		on_selected->invoke(selected_val);
	}
	return true;
}

void FloatRangeMenuOption::check_overflow()
{
	if (selected_val > max) {
		selected_val = max;
	} else if (selected_val < min) {
		selected_val = min;
	}
}

void FloatRangeMenuOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (image_name) {
		// image drawing is from top-left, font is bottom-left, so need to shift image up
		x += p2gz->images->draw(image_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
		x += p2gz->images->spacing();
		// re-initialise the text printer to prevent the GPU dying
		j2d.initiate();
	}

	if (title) {
		JUtility::TColor arrow_color = j2d.mCharColor;

		x += j2d.print(x, z, "%s: ", title);

		if (selected_val > min) {
			x += j2d.print(x, z, "< ");
		}

		COLOR(p2gz->menu->color_std);
		x += j2d.print(x, z, "%.2f", selected_val);

		COLOR(arrow_color);
		if (selected_val < max) {
			x += j2d.print(x, z, " >");
		}
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_DPAD_LEFT, "change value");
		p2gz->menu->draw_control(j2d, Controller::PRESS_X, "(hold) change faster");
	}
}

HexInputOption::HexInputOption(const char* title_, const char* value_if_unselected_, IDelegate1<u32>* on_selected, IDelegate* on_unselected,
                               const char* image_name_, bool image_only_)
    : MenuOption(title_, image_name_, image_only_)
{
	keypad              = new HexKeypad(title_, value_if_unselected_, on_selected, on_unselected);
	value_if_unselected = value_if_unselected_;
}

HexInputOption::~HexInputOption()
{
	delete keypad;
}

MenuLayer* HexInputOption::get_sub_menu()
{
	return keypad;
}

bool HexInputOption::select()
{
	p2gz->menu->push_layer(keypad);
	return false;
}

void HexInputOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	MenuOption::draw(j2d, x, z, selected);
	if (keypad->is_unselected()) {
		x += j2d.print(x, z, ": %s", value_if_unselected);
	} else {
		x += j2d.print(x, z, ": %08X", keypad->get_value());
	}

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "open");
	}
}

bool HexInputOption::is_selected()
{
	return !keypad->is_unselected();
}

u32 HexInputOption::get_selected_val()
{
	return keypad->get_value();
}

void HexInputOption::set_selected_val(u32 val)
{
	keypad->set_value(val);
}

void HexInputOption::set_unselected()
{
	keypad->unselected = true;
}

DecimalInputOption::DecimalInputOption(const char* title_, IDelegate1<u32>* on_selected, IDelegate* on_opened, const char* image_name_,
                                       bool image_only_)
    : MenuOption(title_, image_name_, image_only_)
{
	keypad     = new DecimalKeypad(title_, on_selected, on_opened);
	sync_value = on_opened;
}

DecimalInputOption::~DecimalInputOption()
{
	if (sync_value) {
		delete sync_value;
	}
	delete keypad;
}

MenuLayer* DecimalInputOption::get_sub_menu()
{
	return keypad;
}

bool DecimalInputOption::select()
{
	p2gz->menu->push_layer(keypad);
	return false;
}

void DecimalInputOption::draw(J2DPrint& j2d, f32& x, f32& z, bool selected)
{
	if (sync_value) {
		sync_value->invoke();
	}
	MenuOption::draw(j2d, x, z, selected);
	x += j2d.print(x, z, ": %05u", keypad->get_value());

	if (selected) {
		p2gz->menu->draw_control(j2d, Controller::PRESS_A, "open");
	}
}

bool DecimalInputOption::is_selected()
{
	return !keypad->is_unselected;
}

u32 DecimalInputOption::get_selected_val()
{
	return keypad->get_value();
}

void DecimalInputOption::set_selected_val(u32 val)
{
	keypad->set_value(val);
}

void BottomControlsDisplay::draw_ctrl(J2DPrint& j2d, Controller::EButton button, const char* action)
{
	const char* button_img = nullptr;
	switch (button) {
	case Controller::PRESS_L:
		button_img = "l_btn";
		break;
	case Controller::PRESS_R:
		button_img = "r_btn";
		break;
	case Controller::PRESS_X:
		button_img = "x_btn";
		break;
	case Controller::PRESS_Y:
		button_img = "y_btn";
		break;
	case Controller::PRESS_A:
		button_img = "a_btn";
		break;
	case Controller::PRESS_B:
		button_img = "b_btn";
		break;
	case Controller::PRESS_DPAD_DOWN:
	case Controller::PRESS_DPAD_UP:
		button_img = "dpad_updown";
		break;
	case Controller::PRESS_DPAD_LEFT:
	case Controller::PRESS_DPAD_RIGHT:
		button_img = "dpad_leftright";
		break;
	case Controller::PRESS_Z:
		button_img = "z_btn";
		break;
	}

	if (!button_img) {
		OSReport("no menu image for button %X\n", button);
		return;
	}

	COLOR(p2gz->menu->color_std);

	x += p2gz->images->draw(button_img, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
	x += p2gz->images->spacing() / 2.0f;
	j2d.initiate();

	x += j2d.print(x, z, "%s", action);
	x += margin;
}
