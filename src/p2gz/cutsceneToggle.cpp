#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/CutsceneToggle.h>
#include <p2gz/gzConstants.h>

using namespace gz;

void CutsceneToggle::init()
{
}

void CutsceneToggle::update()
{
}

void CutsceneToggle::set_cutscene_flag(bool setFlag, P2GZ_CUTSCENE_TOGGLE_ID cutscene_id)
{
	// Depending on which cutscene this option belongs to, trick the game into thinking it has either already played or not
	if (setFlag) {
		// Set the flag, trick the game that the cutscene played so it won't activate
		switch (cutscene_id) {
		case P2GZ_CT_MET_RED_PIKI: {
			Game::playData->setDemoFlag(Game::DEMO_Meet_Red_Pikmin);
		} break;
		}
	} else {
		// Clear the flag, trick the game that the cutscene never played so it may be activated again
		switch (cutscene_id) {
		case P2GZ_CT_MET_RED_PIKI: {
			// Playdata doesn't have a "resetDemoFlag", so we have to manually adjust the demoflag variable ourselves
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_Meet_Red_Pikmin);
			// This is normally handled in 'setDemoFlag', but as mentioned we have todo this ourselves
			Game::playData->mDemoFlags.dump();
		} break;
		}
	}
}
