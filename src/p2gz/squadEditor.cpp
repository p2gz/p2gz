#include <p2gz/SquadEditor.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemPikihead.h>
#include <Game/Navi.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>
#include <og/Sound.h>

using namespace gz;

// Add a Pikmin to the active captain's squad if there are fewer than 100 Pikmin in the field.
void SquadEditor::birth_piki(Game::EPikiKind color, Game::EPikiHappa stage, int count)
{
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (!navi) {
		return;
	}

	for (int i = 0; i < count; i++) {
		Game::Piki* piki = Game::pikiMgr->birth();

		Game::PikiInitArg arg(-1);
		piki->init(&arg);
		piki->changeShape(color);
		piki->changeHappa(stage);

		Vector3f pos = navi->getPosition();
		piki->setPosition(pos, false);
		piki->mNavi = navi;
	}
}

// Remove a Pikmin from the active captain's squad.
void SquadEditor::kill_piki(Game::EPikiKind color, Game::EPikiHappa stage, int count)
{
	int killed = 0;
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mNavi != nullptr && piki->mPikiKind == color && piki->mHappaKind == stage) {
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			piki->kill(&arg);
			killed++;
			if (killed == count) {
				break;
			}
		}
	}
}

// Get the current Pikmin counts from the active captain's squad.
// Returns a vector of { blue_leaves, blue_buds, blue_flowers, red_*, yellow_*, purple_*, white_* }.
gz::Vec<s32> SquadEditor::get_squad()
{
	gz::Vec<s32> squad(15);
	for (int i = 0; i < 15; i++) {
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
	GridMenu* editor   = static_cast<GridMenu*>(p2gz->menu->get_option("pikmin/squad")->get_sub_menu());

	for (int color = 0; color < 5; color++) {
		Vec<MenuOption*>* row = editor->options[color];
		for (int stage = 0; stage < 3; stage++) {
			RangeMenuOption* opt = static_cast<RangeMenuOption*>((*row)[stage]);
			s32 target           = opt->get_selection();
			s32 current          = squad[color * 3 + stage];
			if (target == MAX_PIKI_COUNT) {
				int clamp = MAX_PIKI_COUNT - Game::pikiMgr->mActiveCount - Game::ItemPikihead::mgr->mMonoObjectMgr.mActiveCount;
				opt->set_selection(clamp);
				target = clamp;
			}
			if (target > current
			    && Game::pikiMgr->mActiveCount + Game::ItemPikihead::mgr->mMonoObjectMgr.mActiveCount + 1 > MAX_PIKI_COUNT) {
				opt->set_selection(0);
				target = 0;
			}

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
		return;
	}

	if (!Game::pikiMgr) {
		return;
	}

	if (Game::pikiMgr->mActiveCount == 0) {
		return;
	}

	gz::Vec<s32> squad = get_squad();
	GridMenu* editor   = static_cast<GridMenu*>(p2gz->menu->get_option("pikmin/squad")->get_sub_menu());
	for (int color = 0; color < 5; color++) {
		Vec<MenuOption*>* row = editor->options[color];
		for (int stage = 0; stage < 3; stage++) {
			static_cast<RangeMenuOption*>((*row)[stage])->set_selection(squad[color * 3 + stage]);
		}
	}
}
