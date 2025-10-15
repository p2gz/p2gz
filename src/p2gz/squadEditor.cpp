#include <p2gz/SquadEditor.h>
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
