#ifndef _GZMENU_H
#define _GZMENU_H

#include <types.h>
#include <p2gz/gzCollections.h>
#include <p2gz/DoublePress.h>
#include <JSystem/JUtility/TColor.h>
#include <Graphics.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include <JSystem/J2D/J2DPrint.h>
#include <Dolphin/os.h>
#include <IDelegate.h>

namespace gz {

// predeclarations
struct MenuLayer;

/// Base class for selectable menu options. Override `on_selected` in subclasses
/// to give them unique behavior.
struct MenuOption {
	virtual MenuLayer* get_sub_menu() { return nullptr; }

public:
	MenuOption()
	    : title(nullptr)
	    , visible(true)
	{
	}
	MenuOption(const char* title_)
	    : title(title_)
	    , visible(true)
	{
	}

	virtual f32 draw(J2DPrint& j2d, f32 x, f32 z)
	{
		if (title)
			return j2d.print(x, z, title);
		return 0.0f;
	}
	virtual void select() = 0;

	const char* title;
	bool visible;
};

struct OpenSubMenuOption : public MenuOption {
	virtual MenuLayer* get_sub_menu() { return sub_menu; }

	MenuLayer* sub_menu;

public:
	OpenSubMenuOption()
	    : MenuOption()
	    , sub_menu(nullptr)
	{
	}
	OpenSubMenuOption(const char* title_, MenuLayer* sub_menu_);

	virtual void select();
};

struct PerformActionMenuOption : public MenuOption {
public:
	PerformActionMenuOption()
	    : MenuOption()
	    , on_selected(nullptr)
	{
	}
	PerformActionMenuOption(const char* title_, IDelegate* on_selected_)
	    : MenuOption(title_)
	    , on_selected(on_selected_)
	{
	}

	virtual void select()
	{
		if (on_selected)
			on_selected->invoke();
	}

private:
	IDelegate* on_selected;
};

struct ToggleMenuOption : public MenuOption {
public:
	ToggleMenuOption()
	    : MenuOption()
	    , on(false)
	    , on_selected(nullptr)
	{
	}
	ToggleMenuOption(const char* title_, bool on_, IDelegate1<bool>* on_selected_)
	    : MenuOption(title_)
	    , on(on_)
	    , on_selected(on_selected_)
	{
	}

	f32 draw(J2DPrint& j2d, f32 x, f32 z) { return j2d.print(x, z, "%s: %s", title, on ? "true" : "false"); }

	void select()
	{
		on = !on;
		if (on_selected) {
			on_selected->invoke(on);
		}
	}

private:
	bool on;
	IDelegate1<bool>* on_selected;
};

/// Base class for different types of menus
struct MenuLayer {
public:
	MenuLayer() { }

	virtual void update(JUTGamePad* controller)    = 0;
	virtual void draw(J2DPrint& j2d, f32 x, f32 z) = 0;
	virtual MenuOption* get_option(const char* path) { return nullptr; }
	virtual void navigate_to(const char* path) { }

	const char* title;
	MenuLayer* parent;
};

struct ListMenu : public MenuLayer {
public:
	ListMenu() { }

	virtual void update(JUTGamePad* controller);
	virtual void draw(J2DPrint& j2d, f32 x, f32 z);
	virtual MenuOption* get_option(const char* path);
	virtual void navigate_to(const char* path);

	MenuOption* cur_option() { return options[selected]; }

	ListMenu* push(MenuOption* option)
	{
		options.push(option);
		MenuLayer* sub_menu = option->get_sub_menu();
		if (sub_menu) {
			sub_menu->parent = this;
		}
		return this;
	}

	/// Menu to return to when backing out of this menu.
	/// Root menu should leave as null.
	MenuLayer* parent;
	Vec<MenuOption*> options;
	size_t selected;
};

struct GZMenu {
public:
	GZMenu();

	void init_menu();

	void update();
	void draw(Graphics* gfx);

	void open();
	void close();
	bool is_open() { return enabled; }

	/// Returns a reference to the option, or creates it if
	/// it doesn't exist. Use slashes to indicate heirarchy,
	/// e.g. `layer_a/layer_b/option_name`
	MenuOption* get_option(const char* path);

	void push_layer(MenuLayer* layer_);
	void pop_layer();

	/// Opens the menu to the specified absolute path.
	void navigate_to(const char* path);

	f32 glyph_width;
	f32 glyph_height;
	f32 start_offset_x;
	f32 breadcrumb_start_offset_x;
	f32 start_offset_z;
	f32 line_height;

	JUtility::TColor color_std;
	JUtility::TColor color_highlight;
	JUtility::TColor color_breadcrumbs;

private:
	void increase_text_size();
	void decrease_text_size();

	DoublePress openCloseAction;
	ListMenu* root_layer;

	// menu state
	bool enabled;
	MenuLayer* layer;
	Vec<const char*> breadcrumbs;
	bool eat_inputs;
	bool lock;
};

} // namespace gz

#endif
