#include <p2gz/p2gz.h>
#include <p2gz/warp.h>
#include <p2gz/gzMacros.h>
#include <p2gz/gzmenu.h>

using namespace gz;

static const char* AREA_NAMES[4] = {
	"Valley of Repose\0",
	"Awakening Wood\0",
	"Perplexing Pool\0",
	"Wistful Wild\0",
};
static const char* CAVE_NAMES[16] = {
	"Emergence Cave\0",     "Subterranean Complex\0", "Frontier Cavern\0",  "\0",
	"Hole of Beasts\0",     "White Flower Garden\0",  "Bulblax Kingdom\0",  "Snagret Hole\0",
	"Citadel of Spiders\0", "Glutton's Kitchen\0",    "Submerged Castle\0", "Shower Room\0",
	"Cavern of Chaos\0",    "Hole of Heroes\0",       "Dream Den\0",        "\0",
};
static const size_t NUM_FLOORS[16] = { 2, 9, 8, 0, 5, 5, 7, 7, 5, 6, 5, 7, 10, 15, 14, 0 };

Warp::Warp()
    : warp_area(0)
{
}

void Warp::init()
{
	RadioMenuOption* area_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/area"));
	for (size_t i = 0; i < 4; i++) {
		OSReport("%s\n", AREA_NAMES[i]);
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
	OSReport("update_cave_opt warp_area: %d warp_cave: %d warp_sublevel: %d\n", warp_area, warp_cave, warp_sublevel);
	GZASSERTLINE(warp_area < 4);
	RadioMenuOption* cave_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("warp/cave"));
	cave_opt->set_selection(0);
	warp_cave = 0;

	cave_opt->options.clear();
	cave_opt->options.push("Above Ground");
	for (size_t i = 0; i < 4; i++) {
		const char* cave_name = CAVE_NAMES[warp_area * 4 + i];
		if (cave_name && strlen(cave_name) > 0) {
			OSReport("%s\n", cave_name);
			cave_opt->options.push(cave_name);
		}
	}
}

void Warp::update_sublevel_opt()
{
	OSReport("update_sublevel_opt warp_area: %d warp_cave: %d warp_sublevel: %d\n", warp_area, warp_cave, warp_sublevel);
	GZASSERTLINE(warp_area < 4);
	GZASSERTLINE(warp_cave < 4);
	RangeMenuOption* sublevel_opt = static_cast<RangeMenuOption*>(p2gz->menu->get_option("warp/sublevel"));
	sublevel_opt->set_selection(1);
	sublevel_opt->max = NUM_FLOORS[warp_area * 4 + warp_cave];
}
