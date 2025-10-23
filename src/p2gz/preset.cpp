#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/Preset.h>
#include <Game/Piki.h>
#include <JSystem/J2D/J2DPrint.h>
#include <System.h>

using namespace gz;

Preset* Preset::set_pikmin(int stage, int color, int amount)
{
	squad.getCount(color, stage) = amount;
	return this;
}

Preset* Preset::set_onion_pikmin(int stage, int color, int amount)
{
	onion_pikis.getCount(color, stage) = amount;
	return this;
}

Preset* Preset::set_sprays(bool spicies_unlocked_, int spicies, bool bitters_unlocked_, int bitters)
{
	spicies_unlocked = spicies_unlocked_;
	bitters_unlocked = bitters_unlocked_;
	num_spicies      = spicies;
	num_bitters      = bitters;
	return this;
}

Preset* Preset::set_cutscene_flags(size_t num_flags, int flags[])
{
	cutscene_flags.expandCapacityTo(num_flags);
	for (size_t i = 0; i < num_flags; i++) {
		cutscene_flags.push(flags[i]);
	}
	return this;
}

void Preset::apply()
{
	// GameStat::mePikis.clear(); // clear sprouts

	// Clear squad
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			p2gz->squad_editor->kill_piki(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage), MAX_PIKI_COUNT);
		}
	}

	// Apply squad
	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			int amount = squad.getCount(color, stage);
			p2gz->squad_editor->birth_piki(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage), amount);
		}
	}

	// Apply onion pikmin
	Game::playData->mPikiContainer = onion_pikis;

	// Apply sprays
	p2gz->spray_editor->set_bitters(num_bitters);
	p2gz->spray_editor->set_spicies(num_spicies);
	p2gz->spray_editor->toggle_bitters(bitters_unlocked);
	p2gz->spray_editor->toggle_spicies(spicies_unlocked);

	// Set cutscene flags
	// TODO: use cutscene flag editor for this
	// for (size_t i = 0; i < preset.cutscene_flags.len(); i++) {
	// 	Game::playData->mDemoFlags.setFlag(preset.cutscene_flags[i]);
	// }
}

PresetMenuOption::PresetMenuOption()
    : MenuOption("preset")
{
	// preset = (new Preset("test preset", General))
	//              ->set_pikmin(Game::Flower, Game::Red, 20)
	//              ->set_pikmin(Game::Flower, Game::Yellow, 20)
	//              ->set_pikmin(Game::Flower, Game::Blue, 20)
	//              ->set_pikmin(Game::Flower, Game::Purple, 20)
	//              ->set_pikmin(Game::Flower, Game::White, 20)
	//              ->set_sprays(false, 0, true, 16);
	preset = (new Preset("BK", PoD))
	             ->set_pikmin(Game::Flower, Game::White, 20)
	             ->set_pikmin(Game::Flower, Game::Purple, 20)
	             ->set_pikmin(Game::Flower, Game::Red, 28)
	             ->set_pikmin(Game::Leaf, Game::Blue, 32);
}

const char* color_and_stage_to_name(int color, int stage)
{
	const char* color_name = nullptr;
	const char* stage_name = nullptr;

	switch (color) {
	case 0:
		color_name = "blue";
		break;
	case 1:
		color_name = "red";
		break;
	case 2:
		color_name = "yellow";
		break;
	case 3:
		color_name = "purple";
		break;
	case 4:
		color_name = "white";
		break;
	default:
		return nullptr; // TODO: bulbmin
	}

	switch (stage) {
	case 0:
		stage_name = "leaf";
		break;
	case 1:
		stage_name = "bud";
		break;
	case 2:
		stage_name = "flower";
		break;
	default:
		GZASSERTLINE(false);
	}

	char* img_name = new char[16];
	sprintf(img_name, "%s_%s", color_name, stage_name);
	return const_cast<const char*>(img_name);
}

f32 PresetMenuOption::draw(J2DPrint& j2d, f32 x, f32 z, bool selected)
{
	x = MenuOption::draw(j2d, x, z, selected);
	x += j2d.print(x, z, ": ");
	if (preset) {
		for (int color = 0; color < 6; color++) {
			for (int stage = 0; stage < 3; stage++) {
				int amount = preset->squad.getCount(color, stage);
				if (amount == 0) {
					continue;
				}

				const char* img_name = color_and_stage_to_name(color, stage);
				if (!img_name) {
					continue;
				}

				x += p2gz->images->draw(img_name, x, z - p2gz->images->height() + (p2gz->menu->line_height / 2.0));
				x += 2.0; // space between the image and the number

				j2d.initiate();
				x += j2d.print(x, z, "%d", amount);
				x += 5.0; // some padding between pikmin types
			}
		}
	}
	return x;
}

void PresetMenuOption::update()
{
}

void PresetMenuOption::select()
{
}
