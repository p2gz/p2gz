#ifndef _GZ_SETTINGS_H
#define _GZ_SETTINGS_H

#include "types.h"

// p2gz persists its own data (preferences, custom presets etc) to a dedicated memory
// card file, kept separate from the vanilla "Pikmin2_SaveData" for more flexibility.
// the Settings struct gives the format of that data: a magic + version header followed by
// saved fields. The card framing (the file itself, checksum, async I/O) lives in the
// memory card manager - Settings only lays out/parses the bytes.
struct Stream;

namespace gz {

// Persisted format version. Fields are append-only and each block is read behind a check for
// the version it first appeared in, so saves stay compatible across p2gz releases:
//   - older save + newer p2gz: blocks the save predates keep their setDefault() values
//   - newer save + older p2gz: trailing blocks the build doesn't know are simply not read
// Only ever append; bump + branch (never reorder/resize an existing field) on layout change.
enum SettingsVersion {
	SETTINGS_VERSION_V1 = 1, // bool-settings array (count-prefixed) + treasure region

	// ---- add new SETTINGS_VERSION_<name> entries above this line ----
	SETTINGS_VERSION_CURRENT = SETTINGS_VERSION_V1, // point this at the newest entry
};

// Every on/off preference. Each maps to one live manager toggle in Settings::apply_one().
// APPEND ONLY (the enum order is the on-card order): inserting/reordering corrupts old saves.
enum SettingId {
	SETTING_cutscenes_skippable,
	SETTING_skip_save_prompts,
	SETTING_timer_enabled,
	SETTING_timer_subtimer,

	SETTING_BOOL_COUNT, // keep last
};

struct Settings {
public:
	Settings() { setDefault(); }

	// reset every field to its default - also the fallback when no/invalid save is present
	void setDefault();

	// serialise to / parse from stream: [magic][version][fields...]
	void write(Stream& output);
	void read(Stream& input);

	// push every loaded value into the live managers
	void apply();
	// push one bool setting into its manager
	void apply_one(SettingId id);

	// save current settings to the mem card
	void save();

	// menu hooks: update a setting, apply it live, and save
	void set_bool(SettingId id, bool value); // generic, drives every ToggleMenuOption
	void set_treasure_region(size_t region);

	/* ===========================================================================
	 * HOW TO ADD A NEW SAVED SETTING
	 *  - A new on/off toggle: add a SETTING_* before SETTING_BOOL_COUNT, give it a
	 *    default in setDefault(), route it to its manager in apply_one(), and bind its
	 *    ToggleMenuOption with CurriedDelegate1<Settings, SettingId, bool>(.., set_bool, ID).
	 *    The count-prefixed array handles old saves automatically, so no need for a new version.
	 *  - A new typed field (int/enum/etc.): add the field, a SETTINGS_VERSION_<name> (aim
	 *    CURRENT at it), default it, write it after the existing fields, and read it behind
	 *    `if (version >= SETTINGS_VERSION_<name>)`; apply it in apply().
	 * ONLY EVER APPEND, never reorder or resize existing fields or old versions will corrupt/break.
	 * =========================================================================== */

	// --- saved settings ---
	bool bool_settings[SETTING_BOOL_COUNT]; // since SETTINGS_VERSION_V1 (count-prefixed)
	int treasure_region;                    // since SETTINGS_VERSION_V1 (gz::TreasureRegion)
};

}; // namespace gz

#endif
