#include <p2gz/gzmenu.h>
#include <JSystem/J2D/J2DPrint.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include <P2JME/P2JME.h>
#include <Graphics.h>
#include <System.h>
#include <Controller.h>
#include <Dolphin/os.h>
#include <p2gz/p2gz.h>
#include <string.h>

using namespace gz;

GZMenu::GZMenu()
    : openCloseAction(DoublePress(Controller::PRESS_DPAD_LEFT, 15))
    , enabled(false)
    , eat_inputs(true)
{
	glyph_width               = 16.0f;
	glyph_height              = 16.0f;
	start_offset_x            = 16.0f;
	breadcrumb_start_offset_x = 10.0f;
	start_offset_z            = 16.0f;
	line_height               = 20.0f;

	color_std         = JUtility::TColor(255, 255, 255, 255);
	color_highlight   = JUtility::TColor(255, 40, 40, 255);
	color_breadcrumbs = JUtility::TColor(226, 192, 116, 255);

	// clang-format off
	// Structure of GZ menu defined here:
    root_layer = (new ListMenu())
        ->push(new OpenSubMenuOption("captain", (new ListMenu())
            ->push(new PerformActionMenuOption("die painfully"))
            ->push(new PerformActionMenuOption("boing"))
        ))
        ->push(new OpenSubMenuOption("settings", (new ListMenu())
            ->push(new PerformActionMenuOption("increase text size"))
            ->push(new PerformActionMenuOption("decrease text size"))
            ->push(new ToggleMenuOption("toggle demo", true))
        ))
		->push(new OpenSubMenuOption("tools", (new ListMenu())
			->push(new ToggleMenuOption("freecam", false))
		));
	// clang-format on

	layer = root_layer;
}

void GZMenu::update()
{
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

	update_menu_settings();
}

void GZMenu::update_menu_settings()
{
	MenuOption* opt;

	// some demo stuff for the menu
	opt = get_option("settings/toggle demo");
	if (opt && opt->check_selected()) {
		// do something
	}
	opt = get_option("settings/increase text size");
	if (opt && opt->check_selected()) {
		glyph_width += 2.0;
		glyph_height += 2.0;
	}
	opt = get_option("settings/decrease text size");
	if (opt && opt->check_selected()) {
		glyph_width -= 2.0;
		glyph_height -= 2.0;
	}
	opt = p2gz->menu->get_option("tools/freecam");
	if (opt) {
		if (opt->check_selected() && !p2gz->freecam->is_enabled()) {
			p2gz->freecam->enable();
		} else if (opt->check_selected()) {
			p2gz->freecam->update();
		} else if (!opt->check_selected() && p2gz->freecam->is_enabled()) {
			p2gz->freecam->disable();
		}
	}
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
		options[selected]->on_selected();
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

void OpenSubMenuOption::on_selected()
{
	if (!sub_menu)
		return;
	p2gz->menu->push_layer(sub_menu);
}
