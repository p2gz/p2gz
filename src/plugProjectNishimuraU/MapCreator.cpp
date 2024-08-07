#include "Game/GameSystem.h"
#include "Game/GameConfig.h"
#include "Game/Cave/RandMapMgr.h"
#include "Game/mapParts.h"
#include "types.h"
#include "GlobalData.h"   // @P2GZ
#include "Dolphin/rand.h" // @P2GZ
#include "Game/PikiContainer.h" // @P2GZ
#include "Game/PikiMgr.h" // @P2GZ
#include "Dolphin/os.h" // @P2GZ

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

	// @P2GZ Start - record sublevel starting conditions
	s64 currentTime = OSTicksToMilliseconds(OSGetTime());
	SegmentRecord* previousRecord = p2gz->mHistory->peek();
	if (previousRecord != nullptr) {
		previousRecord->mEndTime = currentTime;
	}

	if (p2gz->mSetCustomNextSeed) {
		srand(p2gz->mNextSeed);
		p2gz->mSetCustomNextSeed = false;
	}
	u32 seed = getSeed();

	SegmentRecord record;
	record.mSeed = seed;
	record.mFloorIndex = floorInfo->mParms.mFloorIndex1;
	record.mStartTime = currentTime;
	record.mSquad = playData->mCaveSaveData.mCavePikis;

	p2gz->mHistory->push(record);
	OSReport("Generating sublevel with seed %X\n", seed);
	// @P2GZ End

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
