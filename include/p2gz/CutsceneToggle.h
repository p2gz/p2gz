#ifndef _GZ_CUTSCENE_TOGGLE_H
#define _GZ_CUTSCENE_TOGGLE_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <p2gz/gzCollections.h>
#include <Dolphin/os.h>
#include <Game/gamePlayData.h>
#include <p2gz/gzmenu.h>
#include <p2gz/gzConstants.h>

namespace gz {

struct CutsceneToggle {
public:
	void init();
	void update();

	// Enable or disable cutscene flags, allowing them to either play again or not
	// If setFlag is true, the cutscene will be disabled (Tell the game it already played); false means the cutscene can be triggered again
	void set_cutscene_flag(bool setFlag, P2GZ_CUTSCENE_TOGGLE_ID cutscene_id);

private:
};

}; // namespace gz

#endif
