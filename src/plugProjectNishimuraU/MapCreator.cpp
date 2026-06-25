#include "Game/GameSystem.h"
#include "Game/GameConfig.h"
#include "Game/Cave/RandMapMgr.h"
#include "Game/mapParts.h"
#include "types.h"
#include <p2gz/p2gz.h>
#include <PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h>

namespace Game {
namespace Cave {
extern RandMapMgr* randMapMgr;
} // namespace Cave

/**
 * @note Address: 0x8024C5E4
 * @note Size: 0xF0
 */
void RoomMapMgr::nishimuraCreateRandomMap(MapUnitInterface* muiArray, int p2, Cave::FloorInfo* floorInfo, bool lastFloor,
                                          Cave::EditMapUnit* unit)
{
	bool isVersusHiba = false;
	if (gameSystem && gameSystem->isVersusMode() && gGameConfig.mParms.mVsHiba.mData) {
		isVersusHiba = true;
	}

	// @P2GZ - record seed for this sublevel
	p2gz->segment_history->started_creating_map = true;
	gz::Segment* segment                        = p2gz->segment_history->cur_segment();
	GZASSERTLINE(segment);

	// @P2GZ race mode: force set seed for every floor of the run
	if (p2gz->race_mode->is_active() && p2gz->race_mode->using_set_seed()) {
		segment->use_set_seed = true;
		segment->seed         = p2gz->race_mode->get_seed();
	}

	if (segment->use_set_seed) {
		// seed was set via warp menu - apply it here
		srand(segment->seed);
	} else {
		// seed was not set - record it in case we want to retry
		segment->seed = get_rng_seed();
	}

	Cave::randMapMgr = new Cave::RandMapMgr(isVersusHiba);
	Cave::randMapMgr->loadResource(muiArray, p2, floorInfo, lastFloor, unit);
	Cave::randMapMgr->create();

	const int numRooms = Cave::randMapMgr->getNumRooms();
	for (int i = 0; i < numRooms; i++) {
		char* name = Cave::randMapMgr->getUseUnitName(i);
		useUnit(name);
	}
}

/**
 * @note Address: 0x8024C6D4
 * @note Size: 0x11C
 */
void RoomMapMgr::nishimuraPlaceRooms()
{
	const int numRooms = Cave::randMapMgr->getNumRooms();
	allocRooms(numRooms);

	for (int index = 0; index < numRooms; index++) {
		f32 centreX;
		f32 centreY;
		int direction;

		char* unitName         = Cave::randMapMgr->getRoomData(index, centreX, centreY, direction); // sets centreX, centreY, and direction
		RoomLink* pRoomLink    = Cave::randMapMgr->makeRoomLink(index);
		ObjectLayoutInfo* pOLI = Cave::randMapMgr->makeObjectLayoutInfo(index);

		makeRoom(unitName, centreX, centreY, direction, index, pRoomLink, pOLI);
	}

	for (int i = 0; i < 2; i++) {
		Vector3f startPos(0.0f, 0.0f, 0.0f);
		Cave::randMapMgr->getStartPosition(startPos, i);

		mStartPositions[i].x = startPos.x;
		mStartPositions[i].y = startPos.y;
		mStartPositions[i].z = startPos.z;
	}
}

/**
 * @note Address: 0x8024C7F0
 * @note Size: 0x88
 */
void RoomMapMgr::nishimuraSetTexture()
{
	const int numRooms = Cave::randMapMgr->getNumRooms();
	for (int i = 0; i < numRooms; i++) {
		char* unitName      = Cave::randMapMgr->getUseUnitName(i);
		JUTTexture* texture = getTexture(unitName);
		Cave::randMapMgr->setUnitTexture(i, texture);
	}
}
} // namespace Game
