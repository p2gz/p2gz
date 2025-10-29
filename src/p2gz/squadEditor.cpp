#include <p2gz/SquadEditor.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemPikihead.h>
#include <Game/Navi.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <Game/PikiState.h>
#include <Game/gamePlayData.h>
#include <PikiAI.h>
#include <og/Sound.h>

using namespace gz;

void SquadEditor::init()
{
	squad_menu = static_cast<GridMenu*>(p2gz->menu->get_option("pikmin/squad")->get_sub_menu());
}

// Add a Pikmin to the active captain's squad if there are fewer than 100 Pikmin in the field.
void SquadEditor::birth_piki(Game::EPikiKind color, Game::EPikiHappa stage, int count)
{
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (!navi) {
		return;
	}

	if (!Game::playData) {
		return;
	}

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

// Remove a Pikmin from the active captain's squad.
void SquadEditor::kill_piki(Game::EPikiKind color, Game::EPikiHappa stage, int count)
{
	int killed = 0;
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mPikiKind == color && piki->mHappaKind == stage && !piki->isZikatu()) {
			piki->endStick();
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			piki->kill(&arg);
			killed++;
			if (killed == count) {
				break;
			}
		}
	}
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
}

// Get the current Pikmin counts from the active captain's squad.
// Returns a vector of { blue_leaves, blue_buds, blue_flowers, red_*, yellow_*, purple_*, white_* }.
gz::Vec<s32> SquadEditor::get_squad()
{
	gz::Vec<s32> squad(18);
	for (int i = 0; i < 18; i++) {
		squad.push(0);
	}
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mNavi) {
			squad[piki->mPikiKind * 3 + piki->mHappaKind]++;
		}
	}
	return squad;
}

// Set the active captain's squad to the Pikmin counts from the squad menu.
void SquadEditor::set_squad(s32 _)
{
	gz::Vec<s32> squad = get_squad();

	int total = Game::ItemPikihead::mgr->mMonoObjectMgr.mActiveCount;
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mNavi == nullptr) {
			total++;
		}
	}

	// If the player increments an option to exceed 100 total Pikmin, we need to identify the offending option
	// and clamp it so the total Pikmin count is 100.
	RangeMenuOption* changed = nullptr;
	int previous             = 0;
	for (int color = 0; color < 6; color++) {
		Vec<MenuOption*>* row = squad_menu->options[color];
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt = static_cast<RangeMenuOption*>((*row)[stage]);
			s32 target           = opt->get_selection();
			s32 current          = squad[color * 3 + stage];
			if (target != current) {
				changed  = opt;
				previous = current;
			}
			total += target;
		}
	}
	if (total > MAX_PIKI_COUNT) {
		int clamp = changed->get_selection() - total + MAX_PIKI_COUNT;
		if (clamp <= previous) {
			clamp = 0;
		}
		changed->set_selection(clamp);
	}

	for (int color = 0; color < 6; color++) {
		Vec<MenuOption*>* row = squad_menu->options[color];
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt = static_cast<RangeMenuOption*>((*row)[stage]);
			s32 target           = opt->get_selection();
			s32 current          = squad[color * 3 + stage];

			if (target < current) {
				kill_piki(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage), current - target);
			} else if (target > current) {
				birth_piki(static_cast<Game::EPikiKind>(color), static_cast<Game::EPikiHappa>(stage), target - current);
			}
		}
	}
}

// Update the squad menu with the Pikmin counts from the active captain's squad.
void SquadEditor::update()
{
	// Don't update the live squad count while the squad editor is open.
	if (p2gz->menu->is_open() && p2gz->menu->get_active_layer() && p2gz->menu->get_active_layer()->title
	    && strcmp(p2gz->menu->get_active_layer()->title, "squad") == 0) {
		open = true;
		return;
	}

	open = false;

	if (!Game::pikiMgr) {
		return;
	}

	if (Game::pikiMgr->mActiveCount == 0) {
		return;
	}

	gz::Vec<s32> squad = get_squad();
	for (int color = 0; color < 6; color++) {
		Vec<MenuOption*>* row = squad_menu->options[color];
		for (int stage = 0; stage < 3; stage++) {
			static_cast<RangeMenuOption*>((*row)[stage])->set_selection(squad[color * 3 + stage]);
		}
	}
}
