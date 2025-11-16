#ifndef _GZ_SAVESTATE_H
#define _GZ_SAVESTATE_H

#include <p2gz/gzCollections.h>
#include <types.h>
#include <Vector3.h>
#include <Game/EnemyBase.h>
#include <Game/Navi.h>
#include <Game/Piki.h>
#include <Game/StateMachine.h>

namespace gz {

struct EnemyState {
public:
	EnemyState() { enemy = nullptr; }
	EnemyState(Game::EnemyBase*);

	void load();

	Game::EnemyBase* enemy;
	int state_id;
	f32 health;
	f32 face_dir;
	Vector3f pos;
	Vector3f vel;
};

struct NaviState {
public:
	NaviState() { navi = nullptr; }
	NaviState(Game::Navi* navi_);

	void load();

	Game::Navi* navi;
	int state_id;
	f32 face_dir;
	f32 health;
	Vector3f pos;
	Vector3f vel;
};

struct SinglePikiState {
public:
	SinglePikiState() { }
	SinglePikiState(Game::Piki* piki);

	Vector3f pos;
	Vector3f vel;
	u8 fsm_state_id;
	u8 brain_action_id;

	inline u8 get_stage() { return (stage_and_kind & 0xF0) >> 4; }
	inline u8 get_kind() { return (stage_and_kind & 0x0F); }

private:
	u8 stage_and_kind; // Upper 4 bits for stage and lower 4 bits for kind
};

struct SquadState {
public:
	SquadState();

	void load();

	Vec<SinglePikiState> piki_states;
};

struct SaveState {
public:
	SaveState() { }
	~SaveState() { }

	Vec<EnemyState> enemies;
	NaviState navis[2];
	SquadState squad;
};

struct SaveStateMgr {
public:
	SaveStateMgr();
	~SaveStateMgr() { }

	void save();
	void load();

	void register_fsm(Game::EnemyBase* enemy, Game::EnemyStateMachine* fsm);
	Game::EnemyStateMachine* get_fsm(Game::EnemyBase* enemy);

private:
	// Currently we can't edit EnemyBase at all without getting strange crashes,
	// probably due to the asm in an unlinked file expecting the struct to be a
	// specific size. We get around this by adding each enemy's FSM to this map
	// in its start function and looking them up later.
	struct EnemyFSMMapEntry {
		EnemyFSMMapEntry(Game::EnemyBase* enemy_, Game::EnemyStateMachine* fsm_)
		{
			enemy = enemy_;
			fsm   = fsm_;
		}

		Game::EnemyBase* enemy;
		Game::EnemyStateMachine* fsm;
	};

	SaveState* saved_state;
	Vec<EnemyFSMMapEntry*> fsm_map;
};

}; // namespace gz

#endif
