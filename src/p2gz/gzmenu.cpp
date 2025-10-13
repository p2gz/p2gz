#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/NaviTools.h>
#include <p2gz/FreeCam.h>
#include <JSystem/J2D/J2DPrint.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include <P2JME/P2JME.h>
#include <Graphics.h>
#include <System.h>
#include <Controller.h>
#include <Dolphin/os.h>
#include <string.h>
#include <IDelegate.h>

using namespace gz;

GZMenu::GZMenu()
    : openCloseAction(DoublePress(Controller::PRESS_DPAD_LEFT, 15))
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
        ->push(new OpenSubMenuOption("captain", (new ListMenu())
            ->push(new PerformActionMenuOption("kill", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::kill)))
            ->push(new PerformActionMenuOption("boing", new Delegate<NaviTools>(p2gz->navi_tools, &NaviTools::jump)))
        ))
		->push(new OpenSubMenuOption("map", (new ListMenu())
			->push(new ToggleMenuOption("collision", false, new Delegate1<CollisionViewer, bool>(p2gz->collision_viewer, &CollisionViewer::toggle)))
		))
        ->push(new OpenSubMenuOption("settings", (new ListMenu())
            ->push(new PerformActionMenuOption("increase text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::increase_text_size)))
            ->push(new PerformActionMenuOption("decrease text size", new Delegate<GZMenu>(p2gz->menu, &GZMenu::decrease_text_size)))
            ->push(new ToggleMenuOption("toggle demo", true, nullptr))
        ))
		->push(new OpenSubMenuOption("tools", (new ListMenu())
			->push(new PerformActionMenuOption("freecam", new Delegate<FreeCam>(p2gz->freecam, &FreeCam::enable)))
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

	JUTGamePad* controller = JUTGamePad::getGamePad(0);

	if (controller) {
		// If we ever press A the double press to open/close the menu should be ignored
		// so we don't do it accidentally when switching pikmin or something
		if (!enabled && controller->getButton() & Controller::PRESS_A) {
			openCloseAction.reset();
		}

		// Open/close the menu
		if (openCloseAction.check(controller)) {
			if (enabled)
				close();
			else
				open();
		}
	}

	if (enabled && layer && controller) {
		layer->update(controller);
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

void GZMenu::draw(Graphics* gfx)
{
	if (!enabled || !layer) {
		return;
	}

	gfx->initPerspPrintf(gfx->mCurrentViewport);
	gfx->initPrimDraw(nullptr);
	gfx->mOrthoGraph.setPort();

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
	if (!root_layer) {
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

void ListMenu::update(JUTGamePad* controller)
{
	// Menu navigation
	u32 btn = controller->getButtonDown();
	if (btn & Controller::PRESS_DPAD_UP && selected > 0) {
		selected -= 1;
	}
	if (btn & Controller::PRESS_DPAD_DOWN && options.len() > 0 && selected < options.len() - 1) {
		selected += 1;
	}
	if (btn & Controller::PRESS_A) {
		options[selected]->select();
	}
	if (btn & Controller::PRESS_B) {
		p2gz->menu->pop_layer();
	}
}

void ListMenu::draw(J2DPrint& j2d, f32 x, f32 z)
{
	for (size_t i = 0; i < options.len(); i++) {
		if (!options[i]->visible) {
			continue;
		}

		if (i == selected) {
			j2d.mCharColor.set(p2gz->menu->color_highlight);
			j2d.mGradientColor.set(p2gz->menu->color_highlight);
		} else {
			j2d.mCharColor.set(p2gz->menu->color_std);
			j2d.mGradientColor.set(p2gz->menu->color_std);
		}

		options[i]->draw(j2d, x, z);
		z += p2gz->menu->line_height;
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

void OpenSubMenuOption::select()
{
	p2gz->menu->push_layer(sub_menu);
}
