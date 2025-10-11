#include <p2gz/gzmenu.h>
#include <JSystem/J2D/J2DPrint.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include <P2JME/P2JME.h>
#include <Graphics.h>
#include <System.h>
#include <Controller.h>
#include <Dolphin/os.h>
#include <p2gz/p2gz.h>

using namespace gz;

GZMenu::GZMenu() : openCloseAction(DoublePress(Controller::PRESS_DPAD_LEFT, 10)) {
    enabled = false;

    glyph_width = 16.0f;
    glyph_height = 16.0f;
    start_offset_x = 16.0f;
    breadcrumb_start_offset_x = 10.0f;
    start_offset_z = 16.0f;
    line_height = 20.0f;

    color_std = JUtility::TColor(255, 255, 255, 255);
    color_highlight = JUtility::TColor(255, 40, 40, 255);
    color_breadcrumbs = JUtility::TColor(226, 192, 116, 255);

    root_layer = new GZMenuLayer();
    layer = root_layer;
}

void GZMenu::update() {
    JUTGamePad* controller = JUTGamePad::getGamePad(0);

    // If we ever press A the double press to open/close the menu should be ignored
    // so we don't do it accidentally when switching pikmin or something
    if (!enabled && controller->getButton() & Controller::PRESS_A) {
        openCloseAction.reset();
    }

    // Open/close the menu
    if (openCloseAction.check(controller)) {
        if (enabled) close();
        else open();
    }

    // Menu navigation
    if (enabled && layer) {
        u32 btn = controller->getButtonDown();
        if (btn & Controller::PRESS_DPAD_UP && layer->selected > 0) {
            layer->selected -= 1;
        }
        if (btn & Controller::PRESS_DPAD_DOWN && layer->options.len() > 0 && layer->selected < layer->options.len() - 1) {
            layer->selected += 1;
        }
        if (btn & Controller::PRESS_A) {
            GZMenuOption* option = layer->options[layer->selected];
            option->select();
            if (option->sub_menu && option->sub_menu->options.len() > 0) {
                layer = option->sub_menu;
                breadcrumbs.push(option->title);
            }
        }
        if (btn & Controller::PRESS_B) {
            GZMenuLayer* parent = layer->parent;
            if (parent) {
                layer = parent;
                breadcrumbs.pop();
            }
            else {
                close();
            }
        }
    }

    check_menu_settings();
}

// some demo stuff for the menu
void GZMenu::check_menu_settings() {
    if (get("menu settings")->get("increase text size")->selected()) {
        glyph_width += 2.0;
        glyph_height += 2.0;
    }
    if (get("menu settings")->get("decrease text size")->selected()) {
        glyph_width -= 2.0;
        glyph_height -= 2.0;
    }
}

void GZMenu::open() {
    if (enabled) return;

    layer = root_layer;
    enabled = true;
}

void GZMenu::close() {
    if (!enabled) return;

    enabled = false;
}

void GZMenu::draw(Graphics* gfx) {
    if (!enabled || !gfx || !layer) {
        return;
    }

    gfx->initPerspPrintf(gfx->mCurrentViewport);
    gfx->initPrimDraw(nullptr);
    gfx->mOrthoGraph.setPort();

    J2DPrint menu_text(gP2JMEMgr->mFont, 0.0f);
    menu_text.initiate();
    menu_text.mGlyphWidth = glyph_width;
    menu_text.mGlyphHeight = glyph_height;

    f32 x = breadcrumb_start_offset_x;
    f32 z = start_offset_z;

    if (breadcrumbs.len() > 0) {
        for (size_t i = 0; i < breadcrumbs.len(); i++) {
            menu_text.mCharColor.set(color_std);
            menu_text.mGradientColor.set(color_std);
            x += menu_text.print(x, z, " > ");

            menu_text.mCharColor.set(color_breadcrumbs);
            menu_text.mGradientColor.set(color_breadcrumbs);
            x += menu_text.print(x, z, breadcrumbs[i]);
        }
        z += line_height;
    }

    x = start_offset_x;
    for (size_t i = 0; i < layer->options.len(); i++) {
        if (!layer->options[i]->visible) {
            continue;
        }

        if (i == layer->selected) {
            menu_text.mCharColor.set(color_highlight);
            menu_text.mGradientColor.set(color_highlight);
        }
        else {
            menu_text.mCharColor.set(color_std);
            menu_text.mGradientColor.set(color_std);
        }

        const char* option_txt = layer->options[i]->title;
        menu_text.print(x, z, option_txt);
        z += line_height;
    }
}

GZMenuOption* GZMenuLayer::get(const char* name) {
    for (size_t i = 0; i < options.len(); i++) {
        GZMenuOption* option = options[i];
        if (strcmp(option->title, name) == 0) {
            return option;
        }
    }

    GZMenuOption* new_option = new GZMenuOption(name);
    new_option->sub_menu->parent = this;
    options.push(new_option);
    return new_option;
}