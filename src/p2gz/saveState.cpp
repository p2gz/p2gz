#include <p2gz/SaveState.h>
#include <Game/generalEnemyMgr.h>
#include <Game/EnemyBase.h>

using namespace gz;

SaveStateMgr::SaveStateMgr()
{
	saved_state = nullptr;
}

EnemyState::EnemyState(Game::EnemyBase* enemy_)
{
	enemy    = enemy_;
	state_id = enemy->mLifecycleFSM->getCurrState(enemy)->mStateID;
	pos      = enemy->mPosition;
	health   = enemy->mHealth;
}

void EnemyState::load()
{
	GZASSERTLINE(enemy);
	enemy->mPosition = pos;
	enemy->mHealth   = health;
	OSReport("setting state %d on %s\n", state_id, enemy->getCreatureName());
	enemy->mLifecycleFSM->transit(enemy, state_id, nullptr);
	enemy->mLifecycleFSM->mState->entry(enemy);
}

void SaveStateMgr::save()
{
	if (saved_state) {
		delete saved_state;
	}
	saved_state = new SaveState;

	GeneralMgrIterator<Game::EnemyBase> iEnemyMgr(Game::generalEnemyMgr);
	CI_LOOP(iEnemyMgr)
	{
		Game::EnemyBase* enemy = iEnemyMgr.getObject();
		if (enemy) {
			saved_state->enemies.push(EnemyState(enemy));
		}
	}
}

void SaveStateMgr::load()
{
	if (!saved_state) {
		return;
	}

	for (size_t i = 0; i < saved_state->enemies.len(); i++) {
		saved_state->enemies[i].load();
	}
}
