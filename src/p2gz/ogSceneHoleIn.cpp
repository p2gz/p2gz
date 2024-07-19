#include "og/newScreen/Cave.h"

namespace og {
namespace newScreen {

/**
 * @note Address: 0x803304F0
 * @note Size: 0x50
 */
HoleIn::HoleIn() { }

void HoleIn::doUserCallBackFunc(Resource::MgrCommand*) { }

void HoleIn::doCreateObj(JKRArchive* archive) { registObj(new ObjHoleIn("P2GZ HoleIn Controls Screen"), archive); }

} // namespace newScreen
} // namespace og
