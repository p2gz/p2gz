#ifndef _GZMENU_H
#define _GZMENU_H

#include <types.h>
#include <p2gz/gzCollections.h>
#include <p2gz/InputHelpers.h>
#include <p2gz/images.h>
#include <JSystem/JUtility/TColor.h>
#include <JSystem/J2D/J2DPrint.h>
#include <JSystem/J2D/J2DPicture.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <Dolphin/os.h>
#include <IDelegate.h>
#include <Game/Piki.h>

namespace gz {

// predeclarations
struct MenuLayer;
struct HexKeypad;
struct DecimalKeypad;

/// Base class for selectable menu options. Override `on_selected` in subclasses
/// to give them unique behavior.
struct MenuOption {
	virtual MenuLayer* get_sub_menu() { return nullptr; }

public:
	MenuOption(const char* title_, const char* image_name_ = nullptr, bool image_only_ = false)
	    : title(title_)
	    , image_name(image_name_)
	    , image_only(image_only_)
	    , visible(true)
	{
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update() { }
	virtual void select() = 0;
	virtual bool is_range_option() { return false; }
	void set_editing_in_grid(bool editing) { editing_in_grid = editing; }

	const char* title;
	bool visible;
	const char* image_name;
	bool image_only;
	bool editing_in_grid;
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
	PerformActionMenuOption(const char* title_, IDelegate* on_selected_, const char* image_name_ = nullptr, bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
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
	ToggleMenuOption(const char* title_, bool on_, IDelegate1<bool>* on_selected_, const char* image_name_ = nullptr,
	                 bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
	    , on(on_)
	    , on_selected(on_selected_)
	{
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);

	virtual void select()
	{
		on = !on;
		if (on_selected) {
			on_selected->invoke(on);
		}
	}

	void set_selection(bool selected) { on = selected; }

private:
	bool on;
	IDelegate1<bool>* on_selected;
};

struct RadioMenuOption : public MenuOption {
public:
	RadioMenuOption(const char* title_, IDelegate1<size_t>* on_selected_, const char* image_name_ = nullptr, bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
	    , on_selected(on_selected_)
	    , selected_idx(0)
	{
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
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

	RangeMenuOption(const char* title_, s32 min_, s32 max_, s32 initial, OverflowBehavior overflow_behavior_, IDelegate1<s32>* on_selected_,
	                const char* image_name_ = nullptr, bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
	    , on_selected(on_selected_)
	    , selected_val(initial)
	    , min(min_)
	    , max(max_)
	    , overflow_behavior(overflow_behavior_)
	{
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update();
	virtual void select();
	virtual bool is_range_option() { return true; }

	s32 get_selection() { return selected_val; }
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
	FloatRangeMenuOption(const char* title_, f32 min_, f32 max_, f32 initial, IDelegate1<f32>* on_selected_,
	                     const char* image_name_ = nullptr, bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
	    , on_selected(on_selected_)
	    , selected_val(initial)
	    , min(min_)
	    , max(max_)
	{
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update();
	virtual void select();
	virtual bool is_range_option() { return true; }

	void set_selection(f32 val) { selected_val = val; }

	f32 min;
	f32 max;

private:
	void check_overflow();

	IDelegate1<f32>* on_selected;
	f32 selected_val;
	bool in_grid_menu;
};

struct HexInputOption : public MenuOption {
public:
	HexInputOption(const char* title_, const char* value_if_unselected_, IDelegate1<u32>* on_selected, IDelegate* on_unselected,
	               const char* image_name_ = nullptr, bool image_only_ = false);

	virtual MenuLayer* get_sub_menu();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update() { }
	virtual void select();

	bool is_selected();
	u32 get_selected_val();
	void set_selected_val(u32);

private:
	HexKeypad* keypad;
	const char* value_if_unselected;
};

struct DecimalInputOption : public MenuOption {
public:
	DecimalInputOption(const char* title_, const char* image_name_ = nullptr, bool image_only_ = false);

	virtual MenuLayer* get_sub_menu();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update() { }
	virtual void select();

	bool is_selected();
	u32 get_selected_val();
	void set_selected_val(u32);

private:
	DecimalKeypad* keypad;
};

/// Base class for different types of menus
struct MenuLayer {
public:
	MenuLayer(IDelegate* on_opened_ = nullptr) { on_opened = on_opened_; }

	virtual void update()                            = 0;
	virtual void draw(J2DPrint& j2d, f32& x, f32& z) = 0;
	virtual void reset_selection()                   = 0;
	virtual MenuOption* get_option(const char* path) { return nullptr; }
	virtual void navigate_to(const char* path) { }

	const char* title;
	/// Menu to return to when backing out of this menu.
	/// Root menu should leave as null.
	MenuLayer* parent;

	/// Optional callback to be run when this menu is opened
	IDelegate* on_opened;
};

struct ListMenu : public MenuLayer {
public:
	ListMenu(IDelegate* on_opened_ = nullptr)
	    : MenuLayer(on_opened_)
	    , pah_up(Controller::PRESS_DPAD_UP)
	    , pah_down(Controller::PRESS_DPAD_DOWN)
	{
		selected = 0;
		scroll   = 0;
	}

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
	virtual MenuOption* get_option(const char* path);
	virtual void navigate_to(const char* path);
	virtual void reset_selection() { selected = 0; }

	MenuOption* cur_option()
	{
		{
			if (options.len() > 0)
				return options[selected];
			else {
				return nullptr;
			}
		}
	}

	ListMenu* push(MenuOption* option);
	void clear();

	Vec<MenuOption*> options;
	size_t selected;

private:
	size_t scroll;
	PressAndHold pah_up;
	PressAndHold pah_down;
};

struct GridMenu : public MenuLayer {
public:
	GridMenu(f32 column_width_, IDelegate* on_opened_ = nullptr)
	    : MenuLayer(on_opened_)
	    , column_width(column_width_)
	    , selected_row(0)
	    , selected_col(0)
	    , pah_up(Controller::PRESS_DPAD_UP)
	    , pah_down(Controller::PRESS_DPAD_DOWN)
	    , pah_left(Controller::PRESS_DPAD_LEFT)
	    , pah_right(Controller::PRESS_DPAD_RIGHT)
	{
		options.push(new Vec<MenuOption*>);
		editing_range = false;
	}

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
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
	bool editing_range;

private:
	PressAndHold pah_up;
	PressAndHold pah_down;
	PressAndHold pah_left;
	PressAndHold pah_right;
};

struct HexKeypad : public MenuLayer {
public:
	HexKeypad(const char* title_, const char* cancel_text_, IDelegate1<u32>* on_selected_, IDelegate* on_unselected_);

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
	virtual void reset_selection()
	{
		keypad->reset_selection();
		cur_digit = 0;
	}

	u32 get_value() { return value; }
	u32 set_value(u32 value_)
	{
		value         = value_;
		unselected    = false;
	}
	bool is_unselected() { return unselected; }

private:
	void select_digit(u32);
	void submit();
	void set_unselected();

	GridMenu* keypad;
	u32 value;
	u8 cur_digit;
	const char* cancel_text;
	bool unselected;

	IDelegate1<u32>* on_selected;
	IDelegate* on_unselected;
};

struct DecimalKeypad : public MenuLayer {
public:
	DecimalKeypad(const char* title_);

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
	virtual void reset_selection()
	{
		keypad->reset_selection();
		cur_digit = 0;
	}

	u32 get_value() { return value; }
	void set_value(u32 value_) { value = value_; }

	bool is_unselected;

private:
	void select_digit(u32);
	void submit();
	void set_unselected();

	GridMenu* keypad;
	u32 value;
	u8 cur_digit;
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
	bool is_lock() { return lock; }

	// Called when navigating to a new page to disable inputs for 1 frame to prevent accidentally activating other stuff in submenus
	// immediately
	void set_lock() { lock = true; }

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

	MenuLayer* get_active_layer()
	{
		// Safety checks
		if (enabled && layer) {
			return layer;
		}
		// Safety check failed, return nothing
		return nullptr;
	}

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
