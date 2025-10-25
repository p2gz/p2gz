#ifndef _GZ_PRESET_H
#define _GZ_PRESET_H

#include <p2gz/gzCollections.h>
#include <types.h>
#include <Game/PikiContainer.h>
#include <p2gz/gzmenu.h>
#include <JSystem/J2D/J2DPrint.h>

namespace gz {

typedef enum PresetCategory {
	PoD,
	AT,
	General,
} PresetCategory;

struct Preset {
public:
	Preset(const char* name_, PresetCategory category_)
	    : name(name_)
	{
		GZASSERTLINE(name_);

		category         = category_;
		bitters_unlocked = false;
		spicies_unlocked = false;
		num_bitters      = 0;
		num_spicies      = 0;

		squad.clear();
		onion_pikis.clear();
	}

	void apply();

	Preset* set_pikmin(int stage, int color, int amount);
	Preset* set_onion_pikmin(int stage, int color, int amount);
	Preset* set_sprays(bool spicies_unlocked_, int spicies, bool bitters_unlocked_, int bitters);
	Preset* set_cutscene_flags(size_t num_flags, int flags[]);

	PresetCategory category;
	const char* name;
	Game::PikiContainer squad;
	Game::PikiContainer onion_pikis;
	bool bitters_unlocked;
	bool spicies_unlocked;
	int num_bitters;
	int num_spicies;
	Vec<int> cutscene_flags;
};

struct PresetMgr {
	PresetMgr();
	Preset* find(const char* name, PresetCategory category);

	Vec<Preset*> presets;
};

struct PresetMenuOption : public MenuOption {
public:
	PresetMenuOption();

	virtual MenuLayer* get_sub_menu() { return preset_category_list; }
	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void select();

	Preset* current_preset;

private:
	ListMenu* preset_category_list;
};

struct PresetPreviewMenuOption : public MenuOption {
	PresetPreviewMenuOption(Preset* preset_, PresetMenuOption* parent_);

	virtual void draw(J2DPrint& j2d, f32& x, f32& z, bool selected);
	virtual void select();

	Preset* preset;
	PresetMenuOption* parent;
};

}; // namespace gz

#endif
