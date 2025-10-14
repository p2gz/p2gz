#include <p2gz/p2gz.h>
#include <p2gz/warp.h>
#include <p2gz/gzMacros.h>
#include <p2gz/gzmenu.h>

using namespace gz;

static const char* AREA_NAMES[4] = {
	"Valley of Repose",
	"Awakening Wood",
	"Perplexing Pool",
	"Wistful Wild",
};
static const char* CAVE_NAMES[4][4] = {
	{ "Emergence Cave", "Subterranean Complex", "Frontier Cavern", "" },
	{ "Hole of Beasts", "White Flower Garden", "Bulblax Kingdom", "Snagret Hole" },
	{ "Citadel of Spiders", "Glutton's Kitchen", "Submerged Castle", "Shower Room" },
	{ "Cavern of Chaos", "Hole of Heroes", "Dream Den", "" },
};
static const size_t NUM_FLOORS[4][4] = {
	{ 2, 9, 8, 0 },
	{ 5, 5, 7, 7 },
	{ 5, 6, 5, 7 },
	{ 10, 15, 14, 0 },
};

Warp::Warp()
    : warp_area(0)
    , warp_cave(0)
    , warp_sublevel(1)
{
}

void Warp::init()
{
	RadioMenuOption* area_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/area"));
	for (size_t i = 0; i < 4; i++) {
		area_opt->options.push(AREA_NAMES[i]);
	}

	update_cave_opt();
	update_sublevel_opt();
}

void Warp::set_warp_area(size_t area)
{
	warp_area = area;
	update_cave_opt();
	update_sublevel_opt();
}

void Warp::set_warp_cave(size_t cave)
{
	warp_cave = cave;
	update_sublevel_opt();
}

void Warp::set_warp_sublevel(s32 sublevel)
{
	GZASSERTLINE(sublevel >= 1);
	warp_sublevel = sublevel - 1; // Menu is 1-indexed for nicer visuals
}

void Warp::update_cave_opt()
{
	GZASSERTLINE(warp_area < 4);
	RadioMenuOption* cave_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/cave"));
	cave_opt->set_selection(0);
	warp_cave = 0;

	cave_opt->options.clear();
	cave_opt->options.push("Above Ground");
	for (size_t i = 0; i < 4; i++) {
		const char* cave_name = CAVE_NAMES[warp_area][i];
		if (cave_name && strlen(cave_name) > 0) {
			cave_opt->options.push(cave_name);
		}
	}
}

void Warp::update_sublevel_opt()
{
	GZASSERTLINE(warp_area < 4);
	GZASSERTLINE(warp_cave < 5); // 5th is AG
	RangeMenuOption* sublevel_opt = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/sublevel"));
	sublevel_opt->set_selection(1);
	if (warp_cave == 0) {
		// Sublevel field is used for day counter when selecting AG
		sublevel_opt->max = 30;
	} else {
		sublevel_opt->max = NUM_FLOORS[warp_area][warp_cave - 1];
	}
}
