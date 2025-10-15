#ifndef _GZMENU_H
#define _GZMENU_H

#include <types.h>
#include <p2gz/gzCollections.h>
#include <p2gz/DoublePress.h>
#include <JSystem/JUtility/TColor.h>
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
	MenuOption(const char* title_)
	    : title(title_)
	    , visible(true)
	{
	}

	virtual f32 draw(J2DPrint& j2d, f32 x, f32 z, bool selected);
	virtual void update() { }
	virtual void select() = 0;

	const char* title;
	bool visible;
};

struct OpenSubMenuOption : public MenuOption {
	virtual MenuLayer* get_sub_menu() { return sub_menu; }

	MenuLayer* sub_menu;

public:
	OpenSubMenuOption(const char* title_, MenuLayer* sub_menu_);

	virtual void select();
};

struct PerformActionMenuOption : public MenuOption {
public:
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
	ToggleMenuOption(const char* title_, bool on_, IDelegate1<bool>* on_selected_)
	    : MenuOption(title_)
	    , on(on_)
	    , on_selected(on_selected_)
	{
	}

	virtual f32 draw(J2DPrint& j2d, f32 x, f32 z, bool selected);

	virtual void select()
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

struct RadioMenuOption : public MenuOption {
public:
	RadioMenuOption(const char* title_, IDelegate1<size_t>* on_selected_)
	    : MenuOption(title_)
	    , on_selected(on_selected_)
	    , selected_idx(0)
	{
	}

	virtual f32 draw(J2DPrint& j2d, f32 x, f32 z, bool selected);
	virtual void update();
	virtual void select();

	void set_selection(size_t idx) { selected_idx = idx; }

	Vec<const char*> options;

private:
	IDelegate1<size_t>* on_selected;
	size_t selected_idx;
};

struct RangeMenuOption : public MenuOption {
public:
	enum OverflowBehavior { CAP, WRAP };

	RangeMenuOption(const char* title_, s32 min_, s32 max_, s32 initial, OverflowBehavior overflow_behavior_, IDelegate1<s32>* on_selected_)
	    : MenuOption(title_)
	    , on_selected(on_selected_)
	    , selected_val(initial)
	    , min(min_)
	    , max(max_)
	    , overflow_behavior(overflow_behavior_)
	{
	}

	virtual f32 draw(J2DPrint& j2d, f32 x, f32 z, bool selected);
	virtual void update();
	virtual void select();

	void set_selection(s32 val) { selected_val = val; }

	s32 min;
	s32 max;

private:
	void check_overflow();

	IDelegate1<s32>* on_selected;
	s32 selected_val;
	OverflowBehavior overflow_behavior;
};

struct FloatRangeMenuOption : public MenuOption {
public:
	FloatRangeMenuOption(const char* title_, f32 min_, f32 max_, f32 initial, IDelegate1<f32>* on_selected_)
	    : MenuOption(title_)
	    , on_selected(on_selected_)
	    , selected_val(initial)
	    , min(min_)
	    , max(max_)
	{
	}

	virtual f32 draw(J2DPrint& j2d, f32 x, f32 z, bool selected);
	virtual void update();
	virtual void select();

	void set_selection(f32 val) { selected_val = val; }

	f32 min;
	f32 max;

private:
	void check_overflow();

	IDelegate1<f32>* on_selected;
	f32 selected_val;
};

/// Base class for different types of menus
struct MenuLayer {
public:
	virtual void update()                          = 0;
	virtual void draw(J2DPrint& j2d, f32 x, f32 z) = 0;
	virtual void reset_selection()                 = 0;
	virtual MenuOption* get_option(const char* path) { return nullptr; }
	virtual void navigate_to(const char* path) { }

	const char* title;
	/// Menu to return to when backing out of this menu.
	/// Root menu should leave as null.
	MenuLayer* parent;
};

struct ListMenu : public MenuLayer {
public:
	virtual void update();
	virtual void draw(J2DPrint& j2d, f32 x, f32 z);
	virtual MenuOption* get_option(const char* path);
	virtual void navigate_to(const char* path);
	virtual void reset_selection() { selected = 0; }

	ListMenu* push(MenuOption* option)
	{
		options.push(option);
		MenuLayer* sub_menu = option->get_sub_menu();
		if (sub_menu) {
			sub_menu->parent = this;
		}
		return this;
	}

	Vec<MenuOption*> options;
	size_t selected;
};

struct GridMenu : public MenuLayer {
public:
	GridMenu(f32 column_width_)
	    : column_width(column_width_)
	{
		options.push(new Vec<MenuOption*>);
	}

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32 x, f32 z);
	virtual MenuOption* get_option(const char* path);
	virtual void navigate_to(const char* path);
	virtual void reset_selection()
	{
		selected_row = 0;
		selected_col = 0;
	}

	MenuOption* cur_option() { return (*options[selected_row])[selected_col]; }

	GridMenu* push_to_row(MenuOption* option)
	{
		options[selected_row]->push(option);
		MenuLayer* sub_menu = option->get_sub_menu();
		if (sub_menu) {
			sub_menu->parent = this;
		}
		return this;
	}

	GridMenu* end_row()
	{
		selected_row += 1;
		options.push(new Vec<MenuOption*>);
		return this;
	}

	Vec<Vec<MenuOption*>*> options;
	size_t selected_row;
	size_t selected_col;
	f32 column_width;
};

struct GZMenu {
public:
	GZMenu();

	void init_menu();

	void update();
	void draw();

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

	/// Call in `update()` in menu options that use Dpad L to prevent
	/// accidentally closing the menu
	void block_open_close_action() { open_close_action.reset(); }

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

	DoublePress open_close_action;
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
