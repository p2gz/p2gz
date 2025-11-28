#include <p2gz/OnionEditor.h>
#include <Game/Entities/ItemOnyon.h>
#include <Game/PikiMgr.h>
#include <Game/SingleGameSection.h>
#include <p2gz/p2gz.h>
#include <efx/Container.h>

using namespace gz;

#define SET_PIK(color, stage) \
	(new CurriedDelegate2<OnionEditor, Game::EPikiKind, Game::EPikiHappa, s32>(this, &set_piki_count, color, stage))
#define PIK_OPT(opt_name, color, stage, image_name) \
	(new RangeMenuOption(opt_name, 0, 100, 0, RangeMenuOption::WRAP, SET_PIK(color, stage), image_name, true))

#define SET_ONION(color)                       (new CurriedDelegate1<OnionEditor, Game::EPikiKind, bool>(this, &set_onion_unlocked, color))
#define ONION_OPT(opt_name, color, image_name) (new ToggleMenuOption(opt_name, false, SET_ONION(color), image_name, true))

#define LOCKED   0
#define UNLOCKED 1

// clang-format off
// from root/files/user/Abe/map/${area}/defaultgen.txt
const OnionConfig ONION_CONFIG[4][3] = {
	// Valley of Repose
	{
		OnionConfig(Vector3f(-452.735992f, 0.000000f, 3036.528076f), 145.000000f),
		OnionConfig(Vector3f(-225.018997f, 0.000000f, 2784.604980f), -48.880001f, Vector3f(-341.791565f, 60.000000f, 3933.437256f), 0.000000f),
		OnionConfig(Vector3f(-585.956970f, 0.000000f, 2782.989990f), -62.910000f),
	},
	// Awakening Wood
	{
		OnionConfig(Vector3f(367.233002f, -70.879997f, 2800.476074f), 175.000000f, Vector3f(-36.443707f, 15.000000f, -104.841454f), -45.000000f),
		OnionConfig(Vector3f(438.480988f, -70.879997f, 2468.446045f), -18.879999f),
		OnionConfig(Vector3f(125.092003f, -70.879997f, 2647.516113f), 92.910004f)
	},
	// Perplexing Pool
	{
		OnionConfig(Vector3f(-535.408997f, 80.000000f, 988.117981f), 94.695000f),
		OnionConfig(Vector3f(-196.123001f, 80.000000f, 1002.434021f ), -99.184998f),
		OnionConfig(Vector3f(-425.411011f, 80.000000f, 723.676025f), 12.605000f, Vector3f(-1100.000000f, 89.019012f, -950.000000f), 145.000000f)
	},
	// Wistful Wild
	{
		OnionConfig(Vector3f(-3182.139893f, 0.000000f, -732.187012f), -135.000000f),
		OnionConfig(Vector3f(-3390.689941f, 0.000000f, -1000.190002f), 31.120001f),
		OnionConfig(Vector3f(-3454.949951f, 0.000000f, -645.017029f), 142.910004f)
	}
};
// clang-format on

void OnionEditor::init()
{
	onion_menu = static_cast<GridMenu*>(p2gz->menu->get_option("pikmin/onions")->get_sub_menu());

	// clang-format off
	onion_menu
		->push_to_row(PIK_OPT("rf", Game::Red,     Game::Flower, "red_flower"))
		->push_to_row(PIK_OPT("rb", Game::Red,     Game::Bud,    "red_bud"))
		->push_to_row(PIK_OPT("rl", Game::Red,     Game::Leaf,   "red_leaf"))
		->push_to_row(ONION_OPT("ro", Game::Red, "onion_red"))
		->end_row()
		->push_to_row(PIK_OPT("yf", Game::Yellow,  Game::Flower, "yellow_flower"))
		->push_to_row(PIK_OPT("yb", Game::Yellow,  Game::Bud,    "yellow_bud"))
		->push_to_row(PIK_OPT("yl", Game::Yellow,  Game::Leaf,   "yellow_leaf"))
		->push_to_row(ONION_OPT("yo", Game::Yellow, "onion_yellow"))
		->end_row()
		->push_to_row(PIK_OPT("bf", Game::Blue,    Game::Flower, "blue_flower"))
		->push_to_row(PIK_OPT("bb", Game::Blue,    Game::Bud,    "blue_bud"))
		->push_to_row(PIK_OPT("bl", Game::Blue,    Game::Leaf,   "blue_leaf"))
		->push_to_row(ONION_OPT("bo", Game::Blue, "onion_blue"))
		->end_row()
		->push_to_row(PIK_OPT("pf", Game::Purple,  Game::Flower, "purple_flower"))
		->push_to_row(PIK_OPT("pb", Game::Purple,  Game::Bud,    "purple_bud"))
		->push_to_row(PIK_OPT("pl", Game::Purple,  Game::Leaf,   "purple_leaf"))
		->push_to_row(ONION_OPT("po", Game::Purple, "ship_purple"))
		->end_row()
		->push_to_row(PIK_OPT("wf", Game::White,   Game::Flower, "white_flower"))
		->push_to_row(PIK_OPT("wb", Game::White,   Game::Bud,    "white_bud"))
		->push_to_row(PIK_OPT("wl", Game::White,   Game::Leaf,   "white_leaf"))
		->push_to_row(ONION_OPT("wo", Game::White, "ship_white"));
	// clang-format on
}

void OnionEditor::sync()
{
	for (int color = 0; color < 5; color++) {
		get_onion_option(static_cast<Game::EPikiKind>(color))->set_selection(Game::playData->hasContainer(color));
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt = get_pik_option(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage));
			opt->set_selection(Game::playData->mPikiContainer.getCount(color, stage));
		}
	}
}

// Returns whether we are currently in the area in which this onion is discovered.
bool OnionEditor::is_in_unlock_course(Game::Onyon* onion)
{
	const int courses[3]          = { 1, 0, 2 };
	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	if (game->mInCave) {
		return false;
	}
	return courses[onion->mOnyonType] == game->mCurrentCourseInfo->mCourseIndex;
}

// onKill is not implemented for onions. This just disables it and moves it underground.
void OnionEditor::kill_onion(int color)
{
	Game::Onyon* onion = Game::ItemOnyon::mgr->getOnyon(color);
	if (!onion) {
		return;
	}

	for (int stage = 0; stage < 3; stage++) {
		Game::playData->mPikiContainer.getCount(color, stage) = 0;
		p2gz->squad_editor->kill_piki(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage), 100);
	}

	onion->mPosition.y -= 300.0f;
	onion->mGoalWayPoint->setFlag(Game::WPF_Closed);

	onion->mContainer->startDemoDrawOff();
	delete onion->mSpotbeamModel;
	onion->mSpotbeamModel = nullptr;
	onion->setSpotState(Game::Onyon::SPOTSTATE_Closed);
	onion->startWaitMotion();

	sync();
}

// Moves the onion to the given position and rotation.
void OnionEditor::move_onion(Game::Onyon* onion, Vector3f position, f32 rotation)
{
	onion->setPosition(position, false);
	onion->mFaceDir = rotation;
	onion->onSetPosition();
	onion->mGoalWayPoint->mFlags &= ~Game::WPF_Closed;

	onion->setSpotState(Game::Onyon::SPOTSTATE_Opened);
	onion->startWaitMotion();
}

static const char COLOR_LETTERS[7] = "brypwc";
static const char STAGE_LETTERS[4] = "lbf";
RangeMenuOption* OnionEditor::get_pik_option(Game::EPikiKind color, Game::EPikiHappa stage)
{
	char opt_name[3];
	opt_name[0] = COLOR_LETTERS[color];
	opt_name[1] = STAGE_LETTERS[stage];
	opt_name[2] = '\0';

	return static_cast<RangeMenuOption*>(onion_menu->get_option(opt_name));
}

ToggleMenuOption* OnionEditor::get_onion_option(Game::EPikiKind color)
{
	char opt_name[3];
	opt_name[0] = COLOR_LETTERS[color];
	opt_name[1] = 'o';
	opt_name[2] = '\0';

	return static_cast<ToggleMenuOption*>(onion_menu->get_option(opt_name));
}

// Toggles whether the given onion is unlocked. Spawns it if it is not unlocked and is not discovered in the current area.
// Kills it if it is unlocked and is not discovered in the current area.
void OnionEditor::set_onion_unlocked(Game::EPikiKind color, bool unlocked)
{
	Game::SingleGameSection* section = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	if (section->mInCave) {
		return;
	}

	if (unlocked) {
		p2gz->squad_editor->set_demo_flags_for_color(static_cast<Game::EPikiKind>(color));
	} else {
		if (color != Game::Purple && color != Game::White) {
			Game::playData->mHasBootContainerFlags &= ~(1 << color);
		}
		Game::playData->mHasContainerFlags &= ~(1 << color);
		Game::playData->mMeetPikminFlags &= ~(1 << color);
	}

	if (color == Game::Purple || color == Game::White) {
		return;
	}

	int area           = section->mCurrentCourseInfo->mCourseIndex;
	Game::Onyon* onion = Game::ItemOnyon::mgr->getOnyon(color);

	if (unlocked) {
		if (onion == nullptr) {
			onion = Game::ItemOnyon::mgr->birth(ONYON_OBJECT_ONYON, color);
		}
		move_onion(onion, ONION_CONFIG[area][color].unlocked_position, ONION_CONFIG[area][color].unlocked_rotation);
	} else if (is_in_unlock_course(onion)) {
		// Remove old spot effect before moving
		onion->mContainer->startDemoDrawOff();
		delete onion->mSpotbeamModel;
		onion->mSpotbeamModel = nullptr;
		onion->setSpotState(Game::Onyon::SPOTSTATE_Closed);
		onion->startWaitMotion();

		move_onion(onion, ONION_CONFIG[area][color].locked_position, ONION_CONFIG[area][color].locked_rotation);
	} else {
		kill_onion(color);
	}
}

void OnionEditor::set_piki_count(Game::EPikiKind color, Game::EPikiHappa stage, s32 selection)
{
	Game::playData->mPikiContainer.getCount(color, stage) = selection;
}
