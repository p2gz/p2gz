#include <p2gz/p2gz.h>
#include <p2gz/Localization.h>
#include <System.h>
#include <P2JME/P2JME.h>

// include for ripped functions from pelletMgr.cpp
#include "Game/GameConfig.h"
#include "Game/gamePlayData.h"
#include "Game/GameSystem.h"
#include "Game/pelletMgr.h"
#include "Game/Stickers.h"
#include "Game/GameMessage.h"
#include "Game/gameStat.h"
#include "Game/DynParticle.h"
#include "Game/PlatInstance.h"
#include "Game/MoviePlayer.h"
#include "Game/AIConstants.h"
#include "Game/BaseHIO.h"
#include "Dolphin/rand.h"
#include "efx/TOtakara.h"
#include "PSM/Otakara.h"
#include "ObjectTypes.h"
#include "CollInfo.h"
#include "VsOtakaraName.h"
#include "JSystem/J3D/J3DModelLoader.h"
#include "nans.h"
#include <Game/SingleGameSection.h>

using namespace gz;

void LanguageSwap::init_menu()
{
	JUT_ASSERTLINE(10, p2gz->menu, "no p2gz menu!\n");

	// Setup treasure region option and 3 regions below (order of how we add them to the menu matters!!!)
	RadioMenuOption* region_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("localization/treasure region"));
	region_opt->options.push("English");
	region_opt->options.push("Japanese");
	region_opt->options.push("PAL");
}

void LanguageSwap::set_treasure_region(size_t new_treasure_region_id)
{
	// ID in the menu is not 1-to-1 with internal ID's, fix that here
	switch (new_treasure_region_id) {
	case 0:
	default:
		treasure_region_id = System::LANG_English;
		break;

	case 1:
		treasure_region_id = System::LANG_Japanese;
		break;

	// Internally, we'll have have all PAL treasures classified as french
	case 2:
		treasure_region_id = System::LANG_French;
		break;
	}
}

// GK2 is dumb for many reasons, but one of them is the internal name for treasues is not the
// Same between regions (unlike every other treasure), so we check for that here
// Replace the US/JP internal name with the PAL version, and vice versa depending on the current version
char* gk2TreasureNameSwap(char* oldTreasureName)
{
	// If not either of the 2 trouble treasures tied to GK, don't bother with anything below
	if (!(!strcmp(oldTreasureName, "g_futa_kyusyu") || !strcmp(oldTreasureName, "g_futa_sikoku"))) {
		return oldTreasureName;
	}
	// Check if we are in GK
	Game::BaseGameSection* section = Game::gameSystem->getSection();
	if (section && section->getCaveID() == 'y_02') { // glutton's kitchen
		Game::RoomMapMgr* roomMgr = static_cast<Game::RoomMapMgr*>(Game::mapMgr);
		if (roomMgr && roomMgr->mCaveInfo) {
			// If GK2
			if (section->getCurrFloor() + 1 == 2) {
				if (p2gz->language_menu_wrapper->get_treasure_region() == System::LANG_French) {
					// If the current treasure name is the US/JP internal name, return PAL name
					if (!strcmp(oldTreasureName, "g_futa_kyusyu")) {
						return "g_futa_sikoku";
					}
				} else {
					// If the current treasure name is the PAL internal name, return US/JP name
					if (!strcmp(oldTreasureName, "g_futa_sikoku")) {
						return "g_futa_kyusyu";
					}
				}
			}
			// If GK4
			else if (section->getCurrFloor() + 1 == 4) {
				if (p2gz->language_menu_wrapper->get_treasure_region() == System::LANG_French) {
					// If the current treasure name is the US/JP internal name, return PAL name
					if (!strcmp(oldTreasureName, "g_futa_sikoku")) {
						return "g_futa_kyusyu";
					}
				}
				// Otherwise (US/JP version)
				else {
					// If the current treasure name is the PAL internal name, return US/JP name
					if (!strcmp(oldTreasureName, "g_futa_kyusyu")) {
						return "g_futa_sikoku";
					}
				}
			}
		}
	}
	// Not relevant, don't change name
	return oldTreasureName;
}

void updatePelletConfig(Game::PelletConfig* thisConfig)
{
	OSReport("Cur treasure: %s\n\n", thisConfig->mParams.mName.mData);
	for (int i = 0; i < TREASURE_MAP_COUNT; i++) {
		OSReport("Sys region: %d P2GZ region: %d Cur treasure map region: %d\n", sys->mRegion,
		         p2gz->language_menu_wrapper->get_treasure_region(), treasureMap[i].region);
		OSReport("gk treasure swap: %s\n", gk2TreasureNameSwap(thisConfig->mParams.mName.mData));
		// Loop through all treasures in the map to find the one we need to edit
		if (!strcmp(gk2TreasureNameSwap(thisConfig->mParams.mName.mData), treasureMap[i].mName)) {
			// This is the treasure we need; now check if it's for the right version
			if (p2gz->language_menu_wrapper->get_treasure_region() == treasureMap[i].region) {
				OSReport("Region difference detected! Converting...\n");
				// Update config info here
				thisConfig->mParams.mArchive.mData        = treasureMap[i].mArchive;
				thisConfig->mParams.mBmd.mData            = treasureMap[i].mBmd;
				thisConfig->mParams.mAnimMgr.mData        = treasureMap[i].mAnimMgr;
				thisConfig->mParams.mColltree.mData       = treasureMap[i].mCollTree;
				thisConfig->mParams.mRadius.mData         = treasureMap[i].mRadius;
				thisConfig->mParams.mPRadius.mData        = treasureMap[i].mPRadius;
				thisConfig->mParams.mHeight.mData         = treasureMap[i].mHeight;
				thisConfig->mParams.mInertiaScaling.mData = treasureMap[i].mIntertialScaling;
				thisConfig->mParams.mParticleType.mData   = treasureMap[i].mParticleType;
				thisConfig->mParams.mNumParticles.mData   = treasureMap[i].mNumParticles;
				thisConfig->mParams.mParticleSize.mData   = treasureMap[i].mParticleSize;
				thisConfig->mParams.mFriction.mData       = treasureMap[i].mFriction;
				thisConfig->mParams.mMin.mData            = treasureMap[i].mMin;
				thisConfig->mParams.mMax.mData            = treasureMap[i].mMax;
				thisConfig->mParams.mDynamics.mData       = treasureMap[i].mDynamics;
				thisConfig->mParams.mMoney.mData          = treasureMap[i].mMoney;
				thisConfig->mParams.mUnique.mData         = treasureMap[i].mUnique;
				thisConfig->mParams.mIndirect.mData       = treasureMap[i].mIndirect;
				thisConfig->mParams.mNumPMotions.mData    = treasureMap[i].mNumPMotions;
				thisConfig->mParams.mDepth.mData          = treasureMap[i].mDepth;
				thisConfig->mParams.mDepthMax.mData       = treasureMap[i].mDepthMax;
				thisConfig->mParams.mDepthA.mData         = treasureMap[i].mDepthA;
				thisConfig->mParams.mDepthB.mData         = treasureMap[i].mDepthB;
				thisConfig->mParams.mDepthC.mData         = treasureMap[i].mDepthC;
				thisConfig->mParams.mDepthD.mData         = treasureMap[i].mDepthD;
				thisConfig->mParams.mCode.mData           = treasureMap[i].mCode;
				thisConfig->mParams.mDictionary.mData     = treasureMap[i].mDictionary;
				thisConfig->mParams.mIndirectState        = treasureMap[i].mIndirectState;
				// We did what we need to, just end the function
				return;
			}
		}
	}
	// If we make it here, treasure doesn't have a valid map (is same in all versions)
	OSReport("No region difference\n");
}

// Add functions from pelletMgr.cpp below that we need to change for treasure swapping; these functions are equivalent but pelletMgr.cpp is
// not-matching, so yehaw time it is

namespace Game {

void BasePelletMgr::load()
{
	char buffer[512];
	char* file = nullptr;

	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		P2ASSERTBOUNDSINCLUSIVELINE(158, 0, p2gz->language_menu_wrapper->get_treasure_region(), System::LANG_Spanish);
		switch (p2gz->language_menu_wrapper->get_treasure_region()) {
		case System::LANG_Japanese:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "jpn");
			file = buffer;
			break;
		case System::LANG_English:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "us");
			file = buffer;
			break;
		case System::LANG_French:
		case System::LANG_German:
		// case System::LANG_HOL_UNUSED:
		case System::LANG_Italian:
		case System::LANG_Spanish:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "pal");
			file = buffer;
			break;
		}
	} else {
		file = "user/Kando/pellet/";
	}

	char buffer2[512];

	for (int i = 0; i < mConfigList->mConfigCnt; i++) {

		PelletConfig* config   = &mConfigList->mConfigs[i];
		JKRArchive* archive    = nullptr;
		config->mParams.mIndex = i;
		char* archiveName      = config->mParams.mArchive.mData;
		if (strcmp("null", archiveName)) {
			sprintf(buffer2, "%s%s", file, config->mParams.mArchive.mData);
			archive = JKRMountArchive(buffer2, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
		}

		J3DModelData* data = nullptr;

		if (strcmp("null", config->mParams.mBmd.mData) == 0) {
			mModelData[i] = nullptr;
		} else {
			sprintf(buffer2, "%s", config->mParams.mBmd.mData);

			void* resource = JKRFileLoader::getGlbResource(buffer2, nullptr);
			if (resource == nullptr) {
				JUT_PANICLINE(4560, "meck ** %s : is not foun !\n", buffer2);
			}

			u32 flags = J3DMLF_Material_PE_FogOff | J3DMLF_Material_UseIndirect | J3DMLF_18 | J3DMLF_UseImmediateMtx;
			if (config->mParams.mCode.mData & 2) {
				flags |= J3DMLF_UsePostTexMtx;
			}
			data = J3DModelLoaderDataBase::load(resource, flags);

			if (config->mParams.mCode.mData & 2) {
				for (u16 i = 0; i < data->getShapeNum(); i++) {
					data->getShapeNodePointer(i)->setTexMtxLoadType(0x2000);
				}
			}

			mModelData[i] = data;
		}

		if (config->mParams.mAnimMgr.mData) {
			sprintf(buffer2, "%s%s", file, config->mParams.mAnimMgr.mData);
			mAnimMgr[i] = SysShape::AnimMgr::load(buffer2, data, archive);
		}

		if (config->mParams.mColltree.mData) {
			sprintf(buffer2, "%s%s", file, config->mParams.mColltree.mData);
			mCollParts[i] = CollPartFactory::load(buffer2);
		}
	}
}

/**
 * @note Address: 0x8016C0CC
 * @note Size: 0x3E4
 */
// WIP: https://decomp.me/scratch/Ltrad
void BasePelletMgr::load_texArc(char* filename)
{
	char buffer[512];
	char* directory = nullptr;

	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		P2ASSERTBOUNDSINCLUSIVELINE(244, 0, p2gz->language_menu_wrapper->get_treasure_region(), System::LANG_Spanish);
		switch (p2gz->language_menu_wrapper->get_treasure_region()) {
		case System::LANG_Japanese:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "jpn");
			directory = buffer;
			break;
		case System::LANG_English:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "us");
			directory = buffer;
			break;
		case System::LANG_French:
		case System::LANG_German:
		// case System::LANG_HOL_UNUSED:
		case System::LANG_Italian:
		case System::LANG_Spanish:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "pal");
			directory = buffer;
			break;
		}
	} else {
		directory = "user/Kando/pellet/";
	}

	char path[512];
	sprintf(path, "%s%s", directory, filename);

	JKRArchive* textArc = openTextArc(path);
	JUT_ASSERTLINE(4728, textArc, "%s: not found !\n", path);

	for (int i = 0; i < mConfigList->mConfigCnt; i++) {
		PelletConfig* config = &mConfigList->mConfigs[i];

		// @P2GZ localization-swap: non-matching inline, we're just gonna plop it in here and assume it works
		P2ASSERTBOUNDSLINE(4425, 0, i, mEntries);
		if (mIsUsedList[i] != false) {
			config->mParams.mIndex = i;

			JKRArchive* archive = nullptr;
			if (strcmp("null", config->mParams.mArchive.mData)) {
				sprintf(path, "%s%s", directory, config->mParams.mArchive.mData);
				archive = JKRMountArchive(path, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
			}

			J3DModelData* data = nullptr;

			if (strcmp("null", config->mParams.mBmd.mData) == 0) {
				mModelData[i] = nullptr;
			} else {
				JUT_ASSERTLINE(4776, archive, "archive not found\n");
				sprintf(path, "%s", config->mParams.mBmd.mData);
				void* resourceLoad = JKRFileLoader::getGlbResource(path, nullptr);
				void* resource     = resourceLoad;
				if (resourceLoad == nullptr) {
					resource = archive->getResource(path);
					JUT_PANICLINE(4786, "%s : is not foun !\n", path);
				}

				if (config->mParams.mIndirectState != PelletConfig::Indirect_No) {
					u32 flags = J3DMLF_Material_PE_FogOff | J3DMLF_Material_UseIndirect | J3DMLF_18 | J3DMLF_UseImmediateMtx;
					if (config->mParams.mCode.mData & 2) {
						flags |= J3DMLF_UsePostTexMtx;
					}
					data = J3DModelLoaderDataBase::load(resource, flags);
				} else {
					u32 flags = J3DMLF_Material_PE_FogOff | J3DMLF_18 | J3DMLF_UseImmediateMtx;
					if (config->mParams.mCode.mData & 2) {
						flags |= J3DMLF_UsePostTexMtx;
					}
					data = J3DModelLoaderDataBase::load(resource, flags);
				}

				mModelData[i] = data;

				if (config->mParams.mCode.mData & 2) {
					for (u16 i = 0; i < data->getShapeNum(); i++) {
						data->getShapeNodePointer(i)->setTexMtxLoadType(0x2000);
					}
				}
			}

			if (config->mParams.mAnimMgr.mData != 0) {
				sprintf(path, "%s/%s", gk2TreasureNameSwap(config->mParams.mName.mData), config->mParams.mAnimMgr.mData);
				mAnimMgr[i] = SysShape::AnimMgr::load(textArc, path, data, archive, nullptr);
				if (mAnimMgr[i] == nullptr) {
					mAnimMgr[i] = SysShape::AnimMgr::load(textArc, path, data, archive, nullptr);
				}
			}

			if (config->mParams.mColltree.mData != 0) {
				sprintf(path, "%s/%s", gk2TreasureNameSwap(config->mParams.mName.mData), config->mParams.mColltree.mData);
				mCollParts[i] = CollPartFactory::load(textArc, path);
			}
		}
	}
	closeTextArc(textArc);
}

/**
 * @note Address: 0x8016C4B0
 * @note Size: 0x10C
 */
JKRArchive* BasePelletMgr::openTextArc(char* arc)
{
	char directory[512];
	char* file = nullptr;
	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		P2ASSERTBOUNDSINCLUSIVELINE(350, 0, p2gz->language_menu_wrapper->get_treasure_region(), System::LANG_Spanish);
		switch (p2gz->language_menu_wrapper->get_treasure_region()) {
		case System::LANG_Japanese:
			sprintf(directory, "/user/Abe/Pellet/%s/", "jpn");
			file = directory;
			break;
		case System::LANG_English:
			sprintf(directory, "/user/Abe/Pellet/%s/", "us");
			file = directory;
			break;
		case System::LANG_French:
		case System::LANG_German:
		// case System::LANG_HOL_UNUSED:
		case System::LANG_Italian:
		case System::LANG_Spanish:
			sprintf(directory, "/user/Abe/Pellet/%s/", "pal");
			file = directory;
			break;
		}
	} else {
		file = "user/Kando/pellet/";
	}

	char filePath[512];

	sprintf(filePath, "%s/%s", file, arc);
	return JKRMountArchive(filePath, JKRArchive::EMM_Mem, JKRHeap::sCurrentHeap, JKRArchive::EMD_Tail);
}

} // namespace Game
