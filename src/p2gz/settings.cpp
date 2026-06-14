#include <p2gz/Settings.h>
#include <p2gz/p2gz.h>
#include <Game/MemoryCard/Mgr.h>
#include "System.h"
#include "stream.h"

using namespace gz;

// Identifies our memory card payload. If the leading word doesn't match, the buffer isn't
// ours (uninitialised/foreign/corrupt) and we fall back to defaults rather than read garbage
#define SETTINGS_MAGIC 'p2gz'

void Settings::setDefault()
{
	// bool defaults mirror each manager's own default / the menu's initial toggle state
	bool_settings[SETTING_cutscenes_skippable] = true;
	bool_settings[SETTING_skip_save_prompts]   = true;
	bool_settings[SETTING_timer_enabled]       = true;
	bool_settings[SETTING_timer_subtimer]      = false;

	treasure_region = Treasure_US;
}

void Settings::write(Stream& output)
{
	output.setMode(STREAM_MODE_BINARY, 1);
	output.writeInt(SETTINGS_MAGIC);
	output.writeInt(SETTINGS_VERSION_CURRENT);

	// --- SETTINGS_VERSION_V1 ---
	// Count-prefixed so adding a bool later stays readable by older builds (and vice-versa)
	output.writeByte(SETTING_BOOL_COUNT);
	for (int i = 0; i < SETTING_BOOL_COUNT; i++) {
		output.writeByte(bool_settings[i]);
	}
	output.writeInt(treasure_region);
}

void Settings::read(Stream& input)
{
	// Start from defaults so any field absent in an older save retains a sane value.
	setDefault();

	input.setMode(STREAM_MODE_BINARY, 1);
	if (input.readInt() != SETTINGS_MAGIC) {
		return; // not our data - keep defaults
	}

	const u32 version = input.readInt();

	if (version >= SETTINGS_VERSION_V1) {
		// Only assign bools we still know about; ignore any extras a newer build wrote.
		const u8 count = input.readByte();
		for (u8 i = 0; i < count; i++) {
			const bool value = input.readByte();
			if (i < SETTING_BOOL_COUNT) {
				bool_settings[i] = value;
			}
		}
		treasure_region = input.readInt();
	}
	// (A legacy version-1 file fails the gate above and stays on defaults - fine this early.)
}

void Settings::apply()
{
	for (int i = 0; i < SETTING_BOOL_COUNT; i++) {
		apply_one((SettingId)i);
	}
	// sets the pending region; the existing require_update()/update_region() flow applies it
	// on the next level load (see singleGS_Load.cpp)
	p2gz->localization_op->set_treasure_region(treasure_region);
}

void Settings::apply_one(SettingId id)
{
	const bool v = bool_settings[id];
	switch (id) {
	case SETTING_cutscenes_skippable:
		p2gz->skippable_cutscenes->toggle_skippable(v);
		break;
	case SETTING_skip_save_prompts:
		p2gz->skip_save->toggle_save_skip(v);
		break;
	case SETTING_timer_enabled:
		p2gz->timer->set_enabled(v);
		break;
	case SETTING_timer_subtimer:
		p2gz->timer->set_sub_timer_enabled(v);
		break;
	}
}

void Settings::save()
{
	sys->mCardMgr->saveP2GZData();
}

void Settings::set_bool(SettingId id, bool value)
{
	bool_settings[id] = value;
	apply_one(id); // take effect immediately
	save();        // and remember it for next boot
}

void Settings::set_treasure_region(size_t region)
{
	treasure_region = region;
	p2gz->localization_op->set_treasure_region(region);
	save();
}
