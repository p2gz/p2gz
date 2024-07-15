#include "og/newScreen/Cave.h"

namespace og {
namespace newScreen {

/**
 * @note Address: 0x803304F0
 * @note Size: 0x50
 */
HoleIn::HoleIn() { }

/**
 * @note Address: 0x80330540
 * @note Size: 0x68
 */
bool HoleIn::doConfirmSetScene(::Screen::SetSceneArg& sceneArg)
{
	bool result = false;

	switch (sceneArg.getSceneType()) {
	case SCENE_CAVE:
		result = true;
	}

	return result;
}

/**
 * @note Address: 0x803305A8
 * @note Size: 0xC
 */
void HoleIn::doSetBackupScene(::Screen::SetSceneArg& sceneArg) { sceneArg.mDoCreateBackup = false; }

/**
 * @note Address: 0x803305B4
 * @note Size: 0x4
 */
void HoleIn::doUserCallBackFunc(Resource::MgrCommand*) { }

/**
 * @note Address: 0x803305B8
 * @note Size: 0x60
 */
void HoleIn::doCreateObj(JKRArchive* archive) { registObj(new ObjHoleIn("P2GZ HoleIn Controls Screen"), archive); }

/**
 * @note Address: 0x80330618
 * @note Size: 0x4
 */
void HoleIn::doUpdateActive() { }

} // namespace newScreen
} // namespace og
