#include <p2gz/SquadEditor.h>
#include <p2gz/p2gz.h>
#include <p2gz/BoundDelegate.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemPikihead.h>
#include <Game/Navi.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <Game/PikiState.h>
#include <Game/gamePlayData.h>
#include <Game/gameStat.h>
#include <PikiAI.h>
#include <og/Sound.h>

using namespace gz;

#define SET_PIK(color, stage) \
	(new CurriedDelegate2<SquadEditor, Game::EPikiKind, Game::EPikiHappa, s32>(this, &set_piki_count, color, stage))
#define PIK_OPT(opt_name, color, stage, image_name) \
	(new RangeMenuOption(opt_name, 0, 100, 0, RangeMenuOption::WRAP, SET_PIK(color, stage), image_name, true))

void SquadEditor::init()
{
	squad_menu = static_cast<GridMenu*>(p2gz->menu->get_option("pikmin/squad")->get_sub_menu());
	GZASSERTLINE(squad_menu);

	// clang-format off
	squad_menu
		->push_to_row(PIK_OPT("rf", Game::Red,     Game::Flower, "red_flower"))
		->push_to_row(PIK_OPT("rb", Game::Red,     Game::Bud,    "red_bud"))
		->push_to_row(PIK_OPT("rl", Game::Red,     Game::Leaf,   "red_leaf"))
		->end_row()
		->push_to_row(PIK_OPT("yf", Game::Yellow,  Game::Flower, "yellow_flower"))
		->push_to_row(PIK_OPT("yb", Game::Yellow,  Game::Bud,    "yellow_bud"))
		->push_to_row(PIK_OPT("yl", Game::Yellow,  Game::Leaf,   "yellow_leaf"))
		->end_row()
		->push_to_row(PIK_OPT("bf", Game::Blue,    Game::Flower, "blue_flower"))
		->push_to_row(PIK_OPT("bb", Game::Blue,    Game::Bud,    "blue_bud"))
		->push_to_row(PIK_OPT("bl", Game::Blue,    Game::Leaf,   "blue_leaf"))
		->end_row()
		->push_to_row(PIK_OPT("pf", Game::Purple,  Game::Flower, "purple_flower"))
		->push_to_row(PIK_OPT("pb", Game::Purple,  Game::Bud,    "purple_bud"))
		->push_to_row(PIK_OPT("pl", Game::Purple,  Game::Leaf,   "purple_leaf"))
		->end_row()
		->push_to_row(PIK_OPT("wf", Game::White,   Game::Flower, "white_flower"))
		->push_to_row(PIK_OPT("wb", Game::White,   Game::Bud,    "white_bud"))
		->push_to_row(PIK_OPT("wl", Game::White,   Game::Leaf,   "white_leaf"))
		->end_row()
		->push_to_row(PIK_OPT("cf", Game::Bulbmin, Game::Flower, "bulbmin_flower"))
		->push_to_row(PIK_OPT("cb", Game::Bulbmin, Game::Bud,    "bulbmin_bud"))
		->push_to_row(PIK_OPT("cl", Game::Bulbmin, Game::Leaf,   "bulbmin_leaf"));
	// clang-format on
}

// Add a Pikmin to the active captain's squad if there are fewer than 100 Pikmin in the field.
void SquadEditor::birth_piki(Game::EPikiKind color, Game::EPikiHappa stage, int count)
{
	if (!Game::naviMgr || !Game::naviMgr->mArray || !Game::playData || !Game::pikiMgr) {
		return;
	}

	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (!navi) {
		return;
	}

	set_demo_flags_for_color(color);

	Game::pikiMgr->mBirthMode = Game::PikiMgr::PSM_Replace;
	for (int i = 0; i < count; i++) {
		Game::Piki* piki = Game::pikiMgr->birth();
		GZASSERTLINE(piki);

		piki->init(nullptr);
		piki->changeShape(color);
		piki->changeHappa(stage);

		Vector3f pos = navi->getPosition();
		piki->setPosition(pos, false);
		piki->mNavi = navi;
		PikiAI::ActFormationInitArg arg(navi);
		piki->mBrain->start(PikiAI::ACT_Formation, &arg);
	}
	Game::pikiMgr->mBirthMode = Game::PikiMgr::PSM_Normal;
}

void SquadEditor::set_demo_flags_for_color(Game::EPikiKind color)
{
	if (color != Game::Bulbmin) {
		if (color != Game::Purple && color != Game::White) {
			Game::playData->setBootContainer(color);
		}
		Game::playData->setContainer(color);
		Game::playData->setMeetPikmin(color);
	}

	switch (color) {
	case Game::Blue:
		Game::playData->setDemoFlag(Game::DEMO_Find_Blue_Onion);
		break;
	case Game::Red:
		Game::playData->setDemoFlag(Game::DEMO_Pluck_First_Pikmin);
		Game::playData->setDemoFlag(Game::DEMO_Meet_Red_Pikmin);
		Game::playData->setDemoFlag(Game::DEMO_Louie_Finds_Red_Onion);
		break;
	case Game::Yellow:
		Game::playData->setDemoFlag(Game::DEMO_Find_Yellow_Onion);
		break;
	case Game::Purple:
		Game::playData->setDemoFlag(Game::DEMO_Purple_Candypop);
		Game::playData->setDemoFlag(Game::DEMO_Purples_In_Ship);
		Game::playData->setDemoFlag(Game::DEMO_Reds_Purples_Tutorial);
		break;
	case Game::White:
		Game::playData->setDemoFlag(Game::DEMO_White_Candypop);
		Game::playData->setDemoFlag(Game::DEMO_Whites_In_Ship);
		break;
	case Game::Bulbmin:
		Game::playData->setDemoFlag(Game::DEMO_Discover_Bulbmin);
		break;
	}
}

// Remove a Pikmin from the active captain's squad.
void SquadEditor::kill_piki(Game::EPikiKind color, Game::EPikiHappa stage, int count)
{
	if (count <= 0) {
		return;
	}

	int killed = 0;
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mPikiKind == color && piki->mHappaKind == stage && !piki->isZikatu() && !piki->isWildBulbmin()) {
			piki->endStick();
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			piki->kill(&arg);
			killed++;
			if (killed == count) {
				return;
			}
		}
	}

	Iterator<Game::ItemPikihead::Item> iPikihead = Game::ItemPikihead::mgr;
	CI_LOOP(iPikihead)
	{
		Game::ItemPikihead::Item* item = *iPikihead;
		if (item->isAlive() && item->mColor == color && item->mHeadType == stage) {
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			item->kill(&arg);
			Game::GameStat::mePikis.dec(item->mColor);
			killed++;
			if (killed == count) {
				return;
			}
		}
	}
}

void kill_all_seeds()
{
	if (Game::ItemPikihead::mgr) {
		Iterator<Game::ItemPikihead::Item> iPikihead = Game::ItemPikihead::mgr;
		CI_LOOP(iPikihead)
		{
			Game::ItemPikihead::Item* item = *iPikihead;
			if (item->isAlive()) {
				Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
				item->kill(&arg);
				Game::GameStat::mePikis.dec(item->mColor);
			}
		}
	}
	// Clear seed count. This isn't automatically decremented by killing the seeds for some reason
	Game::GameStat::mePikis.clear();
}

void SquadEditor::clear_all_pikmin()
{
	Game::playData->mPikiContainer.clear();
	Game::playData->mCaveSaveData.mCavePikis.clear(); // clear saved cave pikmin
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
		piki->kill(&arg);
	}

	kill_all_seeds();
}

void SquadEditor::clear_field_pikmin()
{
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (!piki->isZikatu() && !piki->isWildBulbmin()) {
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			piki->kill(&arg);
		}
	}

	kill_all_seeds();
}

/// Get the counts for all living Pikmin on the field
Game::PikiContainer SquadEditor::get_squad()
{
	Game::PikiContainer squad;

	// managers don't exist until a level loads so when the menu is opened
	// from the file-select screen, there's no squad to count yet. return the default.
	if (!Game::pikiMgr) {
		return squad;
	}

	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		squad.getCount(piki->mPikiKind, piki->mHappaKind)++;
	}

	if (Game::ItemPikihead::mgr) {
		Iterator<Game::ItemPikihead::Item> iPikihead = Game::ItemPikihead::mgr;
		CI_LOOP(iPikihead)
		{
			Game::ItemPikihead::Item* item = *iPikihead;
			if (item->isAlive()) {
				squad.getCount(item->mColor, item->mHeadType)++;
			}
		}
	}

	return squad;
}

static const char COLOR_LETTERS[7] = "brypwc";
static const char STAGE_LETTERS[4] = "lbf";
RangeMenuOption* SquadEditor::get_option(Game::EPikiKind color, Game::EPikiHappa stage)
{
	char opt_name[3];
	opt_name[0] = COLOR_LETTERS[color];
	opt_name[1] = STAGE_LETTERS[stage];
	opt_name[2] = '\0';

	return static_cast<RangeMenuOption*>(squad_menu->get_option(opt_name));
}

void SquadEditor::set_piki_count(Game::EPikiKind color, Game::EPikiHappa stage, s32 selection)
{
	Game::PikiContainer squad     = get_squad();
	const int num_field_pikmin    = squad.getTotalSum();
	const int selected_kind_count = squad.getCount(color, stage);
	int num_to_change             = selection - selected_kind_count;

	if (num_field_pikmin + num_to_change > MAX_PIKI_COUNT) {
		num_to_change = MAX_PIKI_COUNT - num_field_pikmin;
	} else if (num_field_pikmin + num_to_change < 0) {
		num_to_change = -num_field_pikmin;
	}

	if (num_to_change > 0) {
		birth_piki(color, stage, num_to_change);
	} else {
		kill_piki(color, stage, -num_to_change);
	}

	sync();
}

// Aligns the menu options' selections with actual living pikmin
// counts and updates their upper bounds to prevent going over 100
void SquadEditor::sync()
{
	Game::PikiContainer squad  = get_squad();
	const int num_field_pikmin = squad.getTotalSum();

	for (int color = 0; color < 6; color++) {
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt  = get_option(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage));
			const int num_of_kind = squad.getCount(color, stage);
			opt->set_selection(num_of_kind);
			opt->set_bounds(0, num_of_kind + MAX_PIKI_COUNT - num_field_pikmin);
		}
	}
}
