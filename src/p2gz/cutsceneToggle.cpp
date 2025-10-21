#include <JSystem/J2D/J2DPrint.h>
#include <P2JME/P2JME.h>
#include <p2gz/CutsceneToggle.h>
#include <p2gz/gzConstants.h>
#include <p2gz/p2gz.h>

using namespace gz;

void CutsceneToggle::init()
{

	// Add options for each cutscene below: organized by the type of cutscene they are

	// Meet pikmin cutscenes //
	ListMenu* cutscene_toggle_base_list = static_cast<ListMenu*>(p2gz->menu->get_option("cutscenes/Meet Pikmin")->get_sub_menu());
	CutsceneWrapper* redPiki            = new CutsceneWrapper(Game::DEMO_Meet_Red_Pikmin);
	cutscene_wrapper_list.push(redPiki);

	cutscene_toggle_base_list->push(
	    new ToggleMenuOption("\"Meet Red Pikmin\" already played", true,
	                         new Delegate1<CutsceneWrapper, bool>(redPiki, &CutsceneToggle::CutsceneWrapper::set_cutscene_flag)));
}

void CutsceneToggle::update()
{
	// Update each cutscene menu option
	// Debug
	if (p2gz->menu->is_within_menu("cutscenes")) {
		// OSReport("Cur menu: %s\n", p2gz->menu->get_active_layer_title());
	}
}

void CutsceneToggle::CutsceneWrapper::set_cutscene_flag(bool setFlag)
{
	// Depending on which cutscene this option belongs to, trick the game into thinking it has either already played or not
	if (setFlag) {
		// Set the flag, trick the game that the cutscene played so it won't activate
		Game::playData->setDemoFlag(cutscene_id);
	} else {
		// Clear the flag, trick the game that the cutscene never played so it may be activated again
		// Playdata doesn't have a "resetDemoFlag", so we have to manually adjust the demoflag variable ourselves
		Game::playData->mDemoFlags.resetFlag(cutscene_id);
		// This is normally handled in 'setDemoFlag', but as mentioned we have todo this ourselves
		Game::playData->mDemoFlags.dump();
	}
}

void CutsceneToggle::playdata_update_setting(Game::DemoFlags id)
{
	for (int i = 0; i < cutscene_wrapper_list.len() - 1; i++) {
		if (cutscene_wrapper_list[i]->get_cutscene_id() == id) {
			cutscene_wrapper_list[i]->set_cutscene_flag(true);
		}
	}
}
