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
public:
	MenuOption(const char* title_, const char* image_name_ = nullptr, bool image_only_ = false)
	    : title(title_)
	    , image_name(image_name_)
	    , image_only(image_only_)
	    , visible(true)
	{
		on_focus = nullptr;
	}
	virtual ~MenuOption()
	{
		if (on_focus) {
			delete on_focus;
		}
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update() { }
	virtual bool select() = 0;
	virtual MenuLayer* get_sub_menu() { return nullptr; }

	const char* title;
	bool visible;
	const char* image_name;
	bool image_only;
	IDelegate* on_focus;
};

struct OpenSubMenuOption : public MenuOption {
public:
	OpenSubMenuOption(const char* title_, MenuLayer* sub_menu_);
	~OpenSubMenuOption();

	virtual bool select();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual MenuLayer* get_sub_menu() { return sub_menu; }

private:
	MenuLayer* sub_menu;
};

struct PerformActionMenuOption : public MenuOption {
public:
	PerformActionMenuOption(const char* title_, IDelegate* on_selected_, const char* image_name_ = nullptr, bool image_only_ = false)
	    : MenuOption(title_, image_name_, image_only_)
	    , on_selected(on_selected_)
	{
	}
	~PerformActionMenuOption();

	virtual bool select()
	{
		if (on_selected) {
			on_selected->invoke();
		}
		return false;
	}

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);

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
	~ToggleMenuOption();

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);

	virtual bool select()
	{
		on = !on;
		if (on_selected) {
			on_selected->invoke(on);
		}
		return false;
	}

	bool get_selection() { return on; }
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
	~RadioMenuOption();

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update();
	virtual bool select();

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
		GZASSERTLINE(min <= max);
	}
	~RangeMenuOption();

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update();
	virtual bool select();
	virtual bool is_range_option() { return true; }

	s32 get_selection() { return selected_val; }
	void set_selection(s32 val) { selected_val = val; }
	void set_bounds(s32 min_, s32 max_);

private:
	void update_selection(s32 delta);

	IDelegate1<s32>* on_selected;
	s32 selected_val;
	s32 min;
	s32 max;
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
	~FloatRangeMenuOption();

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update();
	virtual bool select();
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
	~HexInputOption();

	virtual MenuLayer* get_sub_menu();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update() { }
	virtual bool select();

	bool is_selected();
	u32 get_selected_val();
	void set_selected_val(u32);
	void set_unselected();

private:
	HexKeypad* keypad;
	const char* value_if_unselected;
};

struct DecimalInputOption : public MenuOption {
public:
	DecimalInputOption(const char* title_, IDelegate1<u32>* on_selected, IDelegate* on_opened = nullptr, const char* image_name_ = nullptr,
	                   bool image_only_ = false);
	~DecimalInputOption();

	virtual MenuLayer* get_sub_menu();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void update() { }
	virtual bool select();

	bool is_selected();
	u32 get_selected_val();
	void set_selected_val(u32);

private:
	DecimalKeypad* keypad;
	IDelegate* sync_value;
};

/// Base class for different types of menus
struct MenuLayer {
public:
	MenuLayer(IDelegate* on_opened_ = nullptr)
	    : title(nullptr)
	    , parent(nullptr)
	    , on_opened(on_opened_)
	{
	}
	virtual ~MenuLayer();

	virtual void update()                            = 0;
	virtual void draw(J2DPrint& j2d, f32& x, f32& z) = 0;
	virtual void reset_selection()                   = 0;
	virtual MenuOption* get_option(const char* path) { return nullptr; }
	virtual MenuOption* cur_option() { return nullptr; }
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

	~ListMenu();

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
	virtual MenuOption* get_option(const char* path);
	virtual void navigate_to(const char* path);
	virtual void reset_selection() { selected = 0; }

	virtual MenuOption* cur_option()
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
	GridMenu(f32 opt_width_, f32 opt_height_, IDelegate* on_opened_ = nullptr)
	    : MenuLayer(on_opened_)
	    , opt_width(opt_width_)
	    , opt_height(opt_height_)
	    , selected_row(0)
	    , selected_col(0)
	    , pah_up(Controller::PRESS_DPAD_UP)
	    , pah_down(Controller::PRESS_DPAD_DOWN)
	    , pah_left(Controller::PRESS_DPAD_LEFT)
	    , pah_right(Controller::PRESS_DPAD_RIGHT)
	{
		options.push(new Vec<MenuOption*>);
		editing_option = false;
	}
	~GridMenu();

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
	virtual MenuOption* get_option(const char* path);
	virtual void navigate_to(const char* path);
	virtual void reset_selection()
	{
		selected_row = 0;
		selected_col = 0;
	}

	virtual MenuOption* cur_option() { return (*options[selected_row])[selected_col]; }

	GridMenu* push_to_row(MenuOption* option);
	GridMenu* end_row();

	Vec<Vec<MenuOption*>*> options;
	size_t selected_row;
	size_t selected_col;
	f32 opt_width;
	f32 opt_height;

private:
	PressAndHold pah_up;
	PressAndHold pah_down;
	PressAndHold pah_left;
	PressAndHold pah_right;

	bool editing_option;
};

struct HexKeypad : public MenuLayer {
public:
	HexKeypad(const char* title_, const char* cancel_text_, IDelegate1<u32>* on_selected_, IDelegate* on_unselected_);
	~HexKeypad();

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
		value      = value_;
		unselected = false;
	}
	bool is_unselected() { return unselected; }
	bool unselected;

private:
	void select_digit(u32);
	void submit();
	void set_unselected_and_pop();

	GridMenu* keypad;
	u32 value;
	u8 cur_digit;
	const char* cancel_text;

	IDelegate1<u32>* on_selected;
	IDelegate* on_unselected;
};

struct DecimalKeypad : public MenuLayer {
public:
	DecimalKeypad(const char* title_, IDelegate1<u32>* on_selected_, IDelegate* on_opened_ = nullptr);
	~DecimalKeypad();

	virtual void update();
	virtual void draw(J2DPrint& j2d, f32& x, f32& z);
	virtual void reset_selection()
	{
		keypad->reset_selection();
		cur_digit = 0;
	}

	u32 get_value() { return value; }
	void set_value(u32 value_)
	{
		value = value_;
		if (on_selected) {
			on_selected->invoke(value);
		}
	}

	bool is_unselected;

private:
	void select_digit(u32);
	void submit();
	void set_unselected();

	GridMenu* keypad;
	u32 value;
	u8 cur_digit;
	IDelegate1<u32>* on_selected;
};

struct BottomControlsDisplay {
public:
	BottomControlsDisplay()
	{
		x      = 16.0f;
		z      = 430.0f;
		margin = 8.0f;
	}

	void draw_ctrl(J2DPrint& j2d, Controller::EButton button, const char* action);
	void reset() { x = 16.0f; }

	f32 x;
	f32 z;
	f32 margin;
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
	bool is_root_open() { return enabled && layer == root_layer; }

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

	void draw_control(J2DPrint& j2d, Controller::EButton button, const char* action) { controls.draw_ctrl(j2d, button, action); }

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

	/// Check if current active menu layer has title `title`
	bool is_active_menu(const char* title)
	{
		// check if menu is open, and if active layer title matches
		if (!is_open()) {
			return false;
		}
		MenuLayer* active_layer = get_active_layer();
		if (!active_layer) {
			return false;
		}
		if (!active_layer->title) {
			return false;
		}
		if (strcmp(active_layer->title, title) != 0) {
			return false;
		}
		return true;
	}

	/// Check if current active menu layer is a submenu of layer titled `title`
	bool is_active_menu_parent(const char* title)
	{
		// check if menu is open, and if active layer title matches
		if (!is_open()) {
			return false;
		}
		MenuLayer* active_layer = get_active_layer();
		if (!active_layer) {
			return false;
		}
		MenuLayer* parent_layer = active_layer->parent;
		if (!parent_layer) {
			return false;
		}
		if (!parent_layer->title) {
			return false;
		}
		if (strcmp(parent_layer->title, title) != 0) {
			return false;
		}
		return true;
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
	BottomControlsDisplay controls;

	// menu state
	bool enabled;
	MenuLayer* layer;
	Vec<const char*> breadcrumbs;
	bool eat_inputs;
	bool lock;
};

} // namespace gz

#endif
