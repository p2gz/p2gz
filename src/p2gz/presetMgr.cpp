#include <p2gz/Preset.h>
#include <Game/Piki.h>
#include <System.h>

using namespace gz;
using namespace Game;

PresetMgr::PresetMgr()
{
	presets.push((new Preset("everything", General))
	                 ->set_pikmin(Flower, Red, 20)
	                 ->set_pikmin(Flower, Yellow, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_sprays(false, 0, true, 16));
	presets.push((new Preset("EC", PoD))->set_pikmin(Flower, Red, 46)->set_pikmin(Leaf, Red, 6));
	presets.push((new Preset("enter HoB", PoD))
	                 ->set_onion_pikmin(Flower, Red, 36)
	                 ->set_onion_pikmin(Leaf, Red, 6)
	                 ->set_onion_pikmin(Leaf, Purple, 10));
	presets.push((new Preset("HoB1-2", PoD))->set_pikmin(Flower, Red, 62)->set_pikmin(Flower, Purple, 10));
	presets.push((new Preset("HoB3-4", PoD))->set_pikmin(Flower, Red, 52)->set_pikmin(Flower, Purple, 10)->set_pikmin(Leaf, Purple, 10));
	presets.push((new Preset("HoB5-WFG3", PoD))->set_pikmin(Flower, Red, 52)->set_pikmin(Flower, Purple, 20));
	presets.push(
	    (new Preset("WFG4-enter SH", PoD))->set_pikmin(Flower, Red, 37)->set_pikmin(Flower, Purple, 20)->set_pikmin(Flower, White, 15));
	presets.push((new Preset("SH1-2", PoD))->set_pikmin(Flower, Red, 34)->set_pikmin(Flower, Purple, 20)->set_pikmin(Flower, White, 15));
	presets.push((new Preset("SH3-7", PoD))
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5));
	presets.push((new Preset("enter BK (15/5)", PoD))
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Leaf, Blue, 16));
	presets.push((new Preset("enter BK (20)", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Leaf, Blue, 16));
	presets.push((new Preset("enter BK (25)", PoD))
	                 ->set_pikmin(Flower, White, 25)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 24)
	                 ->set_onion_pikmin(Leaf, Blue, 16));
	presets.push((new Preset("BK", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Red, 28)
	                 ->set_pikmin(Leaf, Blue, 32));
	presets.push((new Preset("day 6 CR", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Leaf, Blue, 60));
	presets.push((new Preset("enter SCx", PoD))
	                 ->set_onion_pikmin(Flower, White, 20)
	                 ->set_onion_pikmin(Flower, Purple, 20)
	                 ->set_onion_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 29));
	presets.push((new Preset("SCx", PoD))
	                 ->set_pikmin(Flower, White, 20)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 60)
	                 ->set_onion_pikmin(Flower, Red, 29));
	presets.push((new Preset("FC", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_onion_pikmin(Flower, Red, 29)
	                 ->set_onion_pikmin(Flower, Blue, 25));
	presets.push((new Preset("CoS", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 45)
	                 ->set_onion_pikmin(Flower, Red, 29));
	presets.push((new Preset("GK", PoD))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 39)
	                 ->set_onion_pikmin(Flower, Red, 29));
}

Preset* PresetMgr::find(const char* name, PresetCategory category)
{
	for (size_t i = 0; i < presets.len(); i++) {
		Preset* preset = presets[i];
		GZASSERTLINE(preset);
		GZASSERTLINE(preset->name);
		if (category == preset->category && strcmp(preset->name, name) == 0) {
			return preset;
		}
	}
	return nullptr;
}
