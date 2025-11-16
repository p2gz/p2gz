#include <p2gz/SaveState.h>
#include <p2gz/p2gz.h>
#include <Game/generalEnemyMgr.h>
#include <Game/EnemyBase.h>
#include <Game/PikiMgr.h>

using namespace gz;

SaveStateMgr::SaveStateMgr()
{
	saved_state = nullptr;
}

EnemyState::EnemyState(Game::EnemyBase* enemy_)
{
	enemy    = enemy_;
	pos       = enemy->getPosition();
	vel       = enemy->getVelocity();
	health   = enemy->mHealth;
	face_dir  = enemy->getFaceDir();

	Game::EnemyStateMachine* fsm = p2gz->savestate_mgr->get_fsm(enemy);
	if (fsm) {
		state_id = fsm->getCurrState(enemy)->mStateID;
	} else {
		state_id = -1;
	}
}

void EnemyState::load()
{
	GZASSERTLINE(enemy);

	if (enemy->isDead() && health > 0.0f) {
		if (enemy->mPellet) {
			enemy->mPellet->kill(nullptr);
		}
		Game::EnemyBirthArg birth_arg;
		birth_arg.mPosition = pos;
		birth_arg.mFaceDir  = face_dir;
		enemy               = Game::generalEnemyMgr->birth(enemy->getEnemyTypeID(), birth_arg);
	} else if (enemy->isAlive() && health <= 0.0f) {
		Game::EnemyKillArg kill_arg(Game::CKILL_DisableDeathEffects | Game::CKILL_NotKilledByPlayer);
		enemy->kill(&kill_arg);
		enemy->mPellet->setPosition(pos, false);
	}

	enemy->mPosition = pos;
	enemy->setVelocity(vel);
	enemy->updateFaceDir(face_dir);
	enemy->mHealth   = health;
	enemy->setAlive(health > 0.0f);

	Game::EnemyStateMachine* fsm = p2gz->savestate_mgr->get_fsm(enemy);
	if (fsm) {
		fsm->transit(enemy, state_id, nullptr);
	}
}

NaviState::NaviState(Game::Navi* navi_)
{
	navi     = navi_;
	pos      = navi->getPosition();
	vel      = navi->getVelocity();
	face_dir = navi->getFaceDir();
	health   = navi->mHealth;
	state_id = navi->mFsm->mCurrentID;
}

void NaviState::load()
{
	GZASSERTLINE(navi);

	navi->setPosition(pos, false);
	navi->setVelocity(vel);
	navi->mFaceDir = face_dir;
	navi->mHealth  = health;
	navi->mFsm->transit(navi, state_id, nullptr);
}

SinglePikiState::SinglePikiState(Game::Piki* piki)
{
	pos = piki->getPosition();
	vel = piki->getVelocity();
	// fsm_state_id    = piki->mFsm->getCurrID(piki);
	brain_action_id = piki->mBrain->mActionId;
	u8 stage        = piki->getHappa();
	u8 kind         = piki->getKind();
	stage_and_kind  = (stage << 4) | (kind & 0x0F);
}

SquadState::SquadState()
    : piki_states(100)
{
	Iterator<Game::Piki> iterator(Game::pikiMgr);
	CI_LOOP(iterator)
	{
		Game::Piki* piki = *iterator;
		piki_states.push(SinglePikiState(piki));
	}
}

void SquadState::load()
{
	p2gz->squad_editor->clear_all_pikmin();
	for (size_t i = 0; i < piki_states.len(); i++) {
		SinglePikiState& piki_state = piki_states[i];
		Game::Piki* piki            = Game::pikiMgr->birth();
		piki->init(nullptr);
		piki->changeShape(piki_state.get_kind());
		piki->changeHappa(piki_state.get_stage());
		piki->setPosition(piki_state.pos, false);
		piki->setVelocity(piki_state.vel);
		piki->mBrain->start(piki_state.brain_action_id, nullptr);
		// piki->mFsm->start(piki, piki_state.fsm_state_id, nullptr);
		// TODO navi?
	}
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

	// for (size_t i = 0; i < 2; i++) {
	// 	saved_state->navis[i] = NaviState(Game::naviMgr->getAt(i));
	// }
}

void SaveStateMgr::load()
{
	if (!saved_state) {
		return;
	}

	for (size_t i = 0; i < saved_state->enemies.len(); i++) {
		saved_state->enemies[i].load();
	}

	// for (size_t i = 0; i < 2; i++) {
	// 	saved_state->navis[i].load();
	// }

	saved_state->squad.load();
}

void SaveStateMgr::register_fsm(Game::EnemyBase* enemy, Game::EnemyStateMachine* fsm)
{
	for (size_t i = 0; i < fsm_map.len(); i++) {
		if (fsm_map[i]->enemy == enemy) {
			// The enemy may already have an FSM entry for its lifecycle FSM.
			// We don't need that one, so overwrite it with the more specific kind.
			fsm_map[i]->fsm = fsm;
			return;
		}
	}
	fsm_map.push(new EnemyFSMMapEntry(enemy, fsm));
}

Game::EnemyStateMachine* SaveStateMgr::get_fsm(Game::EnemyBase* enemy)
{
	for (size_t i = 0; i < fsm_map.len(); i++) {
		if (fsm_map[i]->enemy == enemy) {
			return fsm_map[i]->fsm;
		}
	}
	return nullptr;
}
