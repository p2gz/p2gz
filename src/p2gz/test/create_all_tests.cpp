#include <p2gz/p2gz.h>
#include <p2gz/Test.h>
#include <p2gz/Utility.h>
#include <p2gz/BoundDelegate.h>
#include <Dolphin/pad.h>
#include <Game/gamePlayData.h>

using namespace gz;
using namespace gz::test;

#define WARP_WITH_PRESET(name, cat) (new FreeBoundDelegate2<const char*, PresetCategory>(&warp_with_preset, name, cat))
void warp_with_preset(const char* preset_name, PresetCategory category)
{
	Preset* preset = p2gz->preset_mgr->find(preset_name, category);
	GZASSERTLINE(preset);
	p2gz->warp->set_preset(preset, PS_Chosen);
	p2gz->warp->do_warp();
}

#define WARP_TO_DEST(area, cave, sublevel) (new FreeBoundDelegate3<int, int, int>(&warp_to_dest, area, cave, sublevel))
void warp_to_dest(int area, int cave, int sublevel)
{
	p2gz->warp->set_warp_area(area);
	p2gz->warp->set_warp_cave(cave);
	p2gz->warp->set_warp_sublevel(sublevel);
	p2gz->warp->do_warp();
}

#define EXPECT_WARP_PRESET(expected) (new FreeBoundDelegate1<Preset*>(&expect_warp_preset, expected))
void expect_warp_preset(Preset* expected)
{
	Preset* current_preset = p2gz->warp->get_preset();
	GZEXPECT(current_preset == expected, "Preset does not match");
}

#define EXPECT_CAPTAIN_HP(expected) (new FreeBoundDelegate1<f32>(&expect_captain_hp, expected))
void expect_captain_hp(f32 expected)
{
	s32 actual = p2gz->navi_tools->active_navi()->mHealth;
	GZEXPECT(absF(actual - expected) < 0.1f, "Captain health is incorrect");
}

#define ASSERT_LOCATION(area, cave, sublevel) (new FreeBoundDelegate3<int, int, int>(&assert_location, area, cave, sublevel))
void assert_location(int area, int cave, int sublevel)
{
	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	OSReport("current area = %d\n", game->mCurrentCourseInfo->mCourseIndex);
	GZASSERTLINE(game->mCurrentCourseInfo->mCourseIndex == area);

	Game::CourseInfo* dst_course_info = Game::stageList->getCourseInfo(area);
	ID32 caveID(dst_course_info->getCaveID_FromIndex(cave - 1));

	OSReport("current cave ID = %d\n", game->mCaveID.getID());
	GZASSERTLINE(game->mCaveID.getID() == caveID.getID());

	OSReport("current floor = %d\n", game->mCurrentFloor);
	GZASSERTLINE(game->mCurrentFloor == sublevel - 1);
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
        SKIP_LOAD_CUTSCENES
    ));
    tests.push(TEST("sublevel retry works as expected",
        DO_ACTION(WARP_TO_DEST(1, 1, 1))
        SKIP_LOAD_CUTSCENES
        DO_ACTION(WARP_TO_DEST(1, 1, 2))
        SKIP_LOAD_CUTSCENES
        RETRY_SEGMENT
        SKIP_LOAD_CUTSCENES
        DO_ACTION(ASSERT_LOCATION(1, 1, 2))
    ));
	tests.push(TEST("warp to ww",
        DBL_DPAD_L
        PRESS(PAD_BUTTON_A)
        PRESS(PAD_BUTTON_LEFT)
        DO_N(3, PRESS(PAD_BUTTON_DOWN))
        DO_N(2, PRESS(PAD_BUTTON_A)) // select "no preset"
        PRESS(PAD_BUTTON_UP)
	    DO_N(27, PRESS(PAD_BUTTON_RIGHT))
        DO_N(2, PRESS(PAD_BUTTON_DOWN))
        PRESS(PAD_BUTTON_A)
	    SKIP_LOAD_CUTSCENES
    ));
    tests.push(TEST("upgrades apply correctly",
        DO_ACTION(WARP_WITH_PRESET("CoS", PoD))
        WAIT_FOR(in_above_ground_play)
        DO_UNTIL(SKIP_CUTSCENE, in_above_ground_gameplay)
        DO_ACTION(new FreeDelegate(&assert_has_cos_upgrades))
    ));
    tests.push(TEST("warp 20 times AG",
        WAIT_FOR(in_gameplay)
        DO_N(20, DO_ACTION(WARP_WITH_PRESET("CoS", PoD))
                 SKIP_LOAD_CUTSCENES
                 WAIT(50)
        )
    ));
    // tests.push(TEST("warp 20 times in cave",
    //     WAIT_FOR(in_gameplay)
    //     DO_ACTION(WARP_TO_DEST(0, 2, 7))
    //     DO_N(20, DO_ACTION(WARP_WITH_PRESET("SCx5-8", PoD))
    //              SKIP_LOAD_CUTSCENES
    //              WAIT(50)
    //     )
    // ));
    tests.push(TEST("warp after editing squad",
        WAIT_FOR(in_gameplay)
        DO_ACTION(WARP_TO_DEST(0, 2, 1))
        SKIP_LOAD_CUTSCENES
        DO_ACTION((new BoundDelegate1<SquadEditor, s32>(p2gz->squad_editor, &SquadEditor::set_squad, 0)))
        DO_ACTION(WARP_TO_DEST(0, 2, 1))
        SKIP_LOAD_CUTSCENES
        DO_ACTION(EXPECT_WARP_PRESET(nullptr))
        DO_ACTION(EXPECT_CAPTAIN_HP(50.0f))
    ));
	// clang-format on
}
