#ifndef _GZ_NAVITOOLS_H
#define _GZ_NAVITOOLS_H

#include <Game/Navi.h>

namespace gz {

struct NaviTools {
public:
	NaviTools();
	~NaviTools() { }

	Game::Navi* active_navi() { return Game::naviMgr->getActiveNavi(); }

	void jump();
	void kill();
};

}; // namespace gz

#endif
