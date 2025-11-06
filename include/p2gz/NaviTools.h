#ifndef _GZ_NAVITOOLS_H
#define _GZ_NAVITOOLS_H

#include <Game/Navi.h>

namespace gz {

struct NaviTools {
public:
	NaviTools();
	~NaviTools() { }

	void update();
	void sync();

	inline Game::Navi* active_navi() { return Game::naviMgr->getActiveNavi(); }

	void set_boing_mode(bool enabled);
	void set_active_navi_hp(f32 hp);
	void kill();

	bool boing_mode_enabled;
};

}; // namespace gz

#endif
