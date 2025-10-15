#ifndef _GZ_ENEMY_DEBUG_INFO_H
#define _GZ_ENEMY_DEBUG_INFO_H

namespace gz {

struct EnemyDebugInfo {
	EnemyDebugInfo()
	    : enabled(false)
	{
	}

	void init();
	void update();

	bool enabled;
};

}; // namespace gz

#endif
