#include <p2gz/SquadEditor.h>
#include <p2gz/p2gz.h>
#include <p2gz/gzCollections.h>
#include <Game/Entities/ItemPikihead.h>
#include <Game/Navi.h>
#include <Game/Piki.h>
#include <Game/PikiMgr.h>

using namespace gz;

// Add a Pikmin to the active captain's squad if there are fewer than 100 Pikmin in the field.
void SquadEditor::birth_piki(Game::EPikiKind color, Game::EPikiHappa stage)
{
	if (Game::pikiMgr->mActiveCount + Game::ItemPikihead::mgr->mMonoObjectMgr.mActiveCount + 1 > MAX_PIKI_COUNT) {
		return;
	}

	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	if (!navi) {
		return;
	}

	Game::Piki* piki = Game::pikiMgr->birth();

	Game::PikiInitArg arg(-1);
	piki->init(&arg);
	piki->changeShape(color);
	piki->changeHappa(stage);

	Vector3f pos = navi->getPosition();
	piki->setPosition(pos, false);
	piki->mNavi = navi;
}

// Remove a Pikmin from the active captain's squad.
void SquadEditor::kill_piki(Game::EPikiKind color, Game::EPikiHappa stage)
{
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		if (piki->mNavi != nullptr && piki->mPikiKind == color && piki->mHappaKind == stage) {
			Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
			piki->kill(&arg);
			break;
		}
	}
}

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
	for (int i = 0; i < 5; i++) {
		Vec<MenuOption*>* row = editor->options[i];
		for (int j = 0; j < 3; j++) {
			static_cast<RangeMenuOption*>((*row)[j])->set_selection(squad[i * 3 + j]);
		}
	}
}

void SquadEditor::set_squad()
{
	gz::Vec<s32> squad = get_squad();
	GridMenu* editor   = static_cast<GridMenu*>(p2gz->menu->get_option("pikmin/squad")->get_sub_menu());

	for (int i = 0; i < 5; i++) {
		Vec<MenuOption*>* row = editor->options[i];
		for (int j = 0; j < 3; j++) {
			s32 target  = static_cast<RangeMenuOption*>((*row)[j])->get_selection();
			s32 current = squad[i * 3 + j];
			if (target < current) {
				for (int deaths = 0; deaths < current - target; deaths++) {
					kill_piki(static_cast<Game::EPikiKind>(i), static_cast<Game::EPikiHappa>(j));
				}
			} else if (target > current) {
				for (int births = 0; births < target - current; births++) {
					birth_piki(static_cast<Game::EPikiKind>(i), static_cast<Game::EPikiHappa>(j));
				}
			}
		}
	}
}
