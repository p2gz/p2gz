#include "og/newScreen/SMenu.h"

namespace og {
namespace newScreen {

/**
 * @note Address: 0x803304F0
 * @note Size: 0x50
 */
SMenuWarp::SMenuWarp() { }

/**
 * @note Address: 0x80330540
 * @note Size: 0x68
 */
bool SMenuWarp::doConfirmSetScene(::Screen::SetSceneArg& sceneArg)
{
	bool result = false;

	switch (sceneArg.getSceneType()) {
	case SCENE_GROUND:
	case SCENE_CAVE:
	case SCENE_VS:
	case SCENE_PAUSE_MENU:
	case SCENE_PAUSE_MENU_DOUKUTU:
	case SCENE_PAUSE_MENU_ITEMS:
	case SCENE_PAUSE_MENU_MAP:
	case SCENE_CHALLENGE_2P:
	case SCENE_CHALLENGE_1P:
    case SCENE_PAUSE_MENU_CONTROLS:
	case SCENE_P2GZ_SQUAD:
	case SCENE_P2GZ_WARP:
		result = true;
	}

	return result;
}

/**
 * @note Address: 0x803305A8
 * @note Size: 0xC
 */
void SMenuWarp::doSetBackupScene(::Screen::SetSceneArg& sceneArg) { sceneArg.mDoCreateBackup = false; }

/**
 * @note Address: 0x803305B4
 * @note Size: 0x4
 */
void SMenuWarp::doUserCallBackFunc(Resource::MgrCommand*) { }

/**
 * @note Address: 0x803305B8
 * @note Size: 0x60
 */
void SMenuWarp::doCreateObj(JKRArchive* archive) { registObj(new ObjSMenuWarp("SMenuWarp screen"), archive); }

/**
 * @note Address: 0x80330618
 * @note Size: 0x4
 */
void SMenuWarp::doUpdateActive() { }

} // namespace newScreen
} // namespace og
