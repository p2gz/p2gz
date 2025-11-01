#ifndef _GZ_SAVESTATE_H
#define _GZ_SAVESTATE_H

#include <p2gz/gzCollections.h>
#include <types.h>
#include <Vector3.h>
#include <Game/EnemyBase.h>

namespace gz {

struct EnemyState {
public:
	EnemyState() { enemy = nullptr; }
	EnemyState(Game::EnemyBase*);
	~EnemyState() { }

	void load();

	Game::EnemyBase* enemy;
	int state_id;
	f32 health;
	Vector3f pos;
};

struct SaveState {
public:
	SaveState() { }
	~SaveState() { }

	Vec<EnemyState> enemies;
};

struct SaveStateMgr {
public:
	SaveStateMgr();
	~SaveStateMgr() { }

	void save();
	void load();

private:
	SaveState* saved_state;
};

}; // namespace gz

#endif
