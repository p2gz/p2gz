#include <p2gz/p2gz.h>
#include <p2gz/Test.h>
#include <p2gz/Utility.h>
#include <p2gz/BoundDelegate.h>
#include <Dolphin/pad.h>
#include <Game/gamePlayData.h>

using namespace gz;
using namespace gz::test;

#define WARP_WITH_PRESET(name, cat) (new FreeBoundDelegate2<const char*, PresetCategory>(&warp_with_preset, "CoS", PoD))
void warp_with_preset(const char* preset_name, PresetCategory category)
{
	Preset* preset = p2gz->preset_mgr->find(preset_name, category);
	GZASSERTLINE(preset);
	p2gz->warp->set_preset(preset, PS_Chosen);
	p2gz->warp->do_warp();
}

void assert_has_cos_upgrades()
{
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_SphericalAtlas));
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_PrototypeDetector));
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_FiveManNapsack));
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_JusticeAlloy));
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_GeographicProjection));
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_ForgedCourage));
	GZASSERTLINE(Game::playData->mOlimarData->hasItem(Game::OlimarData::ODII_BruteKnuckles));
}

void TestRunner::create_all_tests()
{
	// clang-format off
	tests.push(TEST("warp from menu",
        WAIT(300)
        DBL_DPAD_L
        PRESS(PAD_BUTTON_A)
        PRESS(PAD_BUTTON_UP)
        PRESS(PAD_BUTTON_A)
        WAIT_FOR(in_above_ground_play)
        DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
    ));
	tests.push(TEST("warp to ww day 31",
        DBL_DPAD_L
        PRESS(PAD_BUTTON_A)
        PRESS(PAD_BUTTON_LEFT)
        DO_N(3, PRESS(PAD_BUTTON_DOWN))
        DO_N(2, PRESS(PAD_BUTTON_A)) // select "no preset"
        PRESS(PAD_BUTTON_UP)
	    DO_N(27, PRESS(PAD_BUTTON_RIGHT))
        DO_N(2, PRESS(PAD_BUTTON_DOWN))
        PRESS(PAD_BUTTON_A)
	    WAIT_FOR(in_above_ground_play)
        DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
    ));
    tests.push(TEST("upgrades apply correctly",
        DO_ACTION(WARP_WITH_PRESET("CoS", PoD))
        WAIT_FOR(in_above_ground_play)
        DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
        DO_ACTION(new FreeDelegate(&assert_has_cos_upgrades))
    ));
    tests.push(TEST("warp 20 times",
        DO_N(20, DO_ACTION(WARP_WITH_PRESET("CoS", PoD))
                 WAIT_FOR(in_above_ground_play)
                 DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
        )
    ));
	// clang-format on
}
