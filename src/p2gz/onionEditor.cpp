#include <p2gz/OnionEditor.h>
#include <Game/Entities/ItemOnyon.h>
#include <Game/PikiMgr.h>
#include <Game/SingleGameSection.h>
#include <p2gz/p2gz.h>

using namespace gz;

const int LOCKED   = 0;
const int UNLOCKED = 1;

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
}

void OnionEditor::update()
{
	if (p2gz->menu->is_active_menu("onions")) {
		return;
	}

	if (!Game::playData) {
		return;
	}

	Game::SingleGameSection* game = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	if (!game) {
		return;
	}

	if (game->mInCave) {
		return;
	}

	if (!Game::ItemOnyon::mgr) {
		return;
	}

	for (int color = 0; color < 5; color++) {
		Vec<MenuOption*>* row = onion_menu->options[color];
		static_cast<ToggleMenuOption*>((*row)[3])->set_selection(Game::playData->hasContainer(color));
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt = static_cast<RangeMenuOption*>((*row)[stage]);
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

	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mPikiKind == color && !piki->isZikatu()) {
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			piki->kill(&arg);
		}
	}

	for (int stage = 0; stage < 3; stage++) {
		Game::playData->mPikiContainer.getCount(color, stage) = 0;
		static_cast<RangeMenuOption*>((*onion_menu->options[color])[stage])->set_selection(0);
	}

	onion->mPosition.y -= 300.0f;
	onion->setSpotState(Game::Onyon::SPOTSTATE_Closed);
	onion->startWaitMotion();
	onion->mGoalWayPoint->setFlag(Game::WPF_Closed);
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

// Toggles whether the given onion is unlocked. Spawns it if it is not unlocked and is not discovered in the current area.
// Kills it if it is unlocked and is not discovered in the current area.
void OnionEditor::set_unlocked(bool _)
{
	Game::SingleGameSection* section = static_cast<Game::SingleGameSection*>(Game::gameSystem->mSection);
	if (section->mInCave) {
		return;
	}

	for (int color = 0; color < 5; color++) {
		Vec<MenuOption*>* row = onion_menu->options[color];
		bool unlocked         = static_cast<ToggleMenuOption*>((*row)[3])->get_selection();

		// Didn't update this onion.
		if (unlocked == Game::playData->hasContainer(color)) {
			continue;
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
			continue;
		}

		int area           = section->mCurrentCourseInfo->mCourseIndex;
		Game::Onyon* onion = Game::ItemOnyon::mgr->getOnyon(color);

		if (unlocked) {
			if (onion == nullptr) {
				onion = Game::ItemOnyon::mgr->birth(ONYON_OBJECT_ONYON, color);
			}
			move_onion(onion, ONION_CONFIG[area][color].unlocked_position, ONION_CONFIG[area][color].unlocked_rotation);
		} else if (is_in_unlock_course(onion)) {
			move_onion(onion, ONION_CONFIG[area][color].locked_position, ONION_CONFIG[area][color].locked_rotation);
		} else {
			kill_onion(color);
		}
	}

	for (int color = 3; color < 5; color++) {
		Vec<MenuOption*>* row = onion_menu->options[color];
		bool unlocked         = static_cast<ToggleMenuOption*>((*row)[3])->get_selection();
		if (unlocked == Game::playData->hasContainer(color)) {
			continue;
		}
	}
}

void OnionEditor::set_count(s32 _)
{
	for (int color = 0; color < 5; color++) {
		Vec<MenuOption*>* row = onion_menu->options[color];
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt = static_cast<RangeMenuOption*>((*row)[stage]);
			if (opt->get_selection() != Game::playData->mPikiContainer.getCount(color, stage)) {
				Game::playData->mPikiContainer.getCount(color, stage) = opt->get_selection();
			}
		}
	}
}
