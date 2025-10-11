#ifndef _GZMENU_H
#define _GZMENU_H

#include <types.h>
#include <p2gz/gzCollections.h>
#include <p2gz/DoublePress.h>
#include <JSystem/JUtility/TColor.h>
#include <Game/BaseGameSection.h>
#include<Graphics.h>

namespace gz {

// predeclarations
struct GZMenuOption;

struct GZMenuLayer {
public:
    GZMenuLayer() : selected(0) {}

    GZMenuOption* cur_option() {
        return options[selected];
    }

    /// Returns a reference to the option, or creates it if
    /// it doesn't exist. Use slashes to indicate heirarchy,
    /// e.g. `layer_a/layer_b/option_name`
    GZMenuOption* get(const char* name);

    /// Deletes an option from this layer
    void remove(const char* name);

    /// Menu to return to when backing out of this menu.
    /// Root menu should leave as null.
    GZMenuLayer* parent;
    Vec<GZMenuOption*> options;
    size_t selected;
};


struct GZMenuOption {
public:
    GZMenuOption() : sub_menu(new GZMenuLayer()) { init(); }
    GZMenuOption(const char* title_)
      : title(title_),
        sub_menu(new GZMenuLayer()) { init(); }

    /// Returns a reference to the option, or creates it if
    /// it doesn't exist. Use slashes to indicate heirarchy,
    /// e.g. `layer_a/layer_b/option_name`
    GZMenuOption* get(const char* name) {
        return sub_menu->get(name);
    }

    bool selected() {
        if (just_selected) {
            just_selected = false;
            return true;
        }
        return false;
    }

    void select() {
        just_selected = true;
    }

    const char* title;
    bool visible;
    GZMenuLayer* sub_menu;

private:
    void init() {
        visible = true;
        just_selected = false;
    }

    bool just_selected;
};

struct GZMenu {
public:
    GZMenu();

    void update();
    void draw(Graphics* gfx);

    void open();
    void close();

    /// Returns a reference to the option, or creates it if
    /// it doesn't exist. Use slashes to indicate heirarchy,
    /// e.g. `layer_a/layer_b/option_name`
    GZMenuOption* get(const char* name) {
        return root_layer->get(name);
    }

private:
    void check_menu_settings();

    DoublePress openCloseAction;
    GZMenuLayer* root_layer;

    f32 glyph_width;
    f32 glyph_height;
    f32 start_offset_x;
    f32 breadcrumb_start_offset_x;
    f32 start_offset_z;
    f32 line_height;

    JUtility::TColor color_std;
    JUtility::TColor color_highlight;
    JUtility::TColor color_breadcrumbs;

    // menu state
    bool enabled;
    GZMenuLayer* layer;
    Vec<const char*> breadcrumbs;
};

} // namespace gz


#endif