#include <p2gz/p2gz.h>
#include <p2gz/Localization.h>
#include <System.h>
#include <P2JME/P2JME.h>
#include <Game/pelletMgr.h>
#include <Game/GameConfig.h>
#include <Game/SingleGameSection.h>
#include <Game/Entities/PelletOtakara.h>

using namespace gz;

void Localization::init_menu()
{
	GZASSERTLINE(p2gz->menu);

	// Setup treasure region option and 3 regions below
	RadioMenuOption* region_opt = get_funni_text() ? static_cast<RadioMenuOption*>(p2gz->menu->get_option("localization/treasure region"))
	                                               : static_cast<RadioMenuOption*>(p2gz->menu->get_option("localisation/treasure region"));

	// NB: make sure this order matches the order in the TreasureRegion enum
	region_opt->options.push("English");
	region_opt->options.push("Japanese");
	region_opt->options.push("PAL");
}

void Localization::set_treasure_region(size_t new_treasure_region_id)
{
	next_treasure_region = (TreasureRegion)new_treasure_region_id;
	if (next_treasure_region != active_treasure_region) {
		// update on next load
		pending_update = true;
	} else {
		// no update needed
		pending_update = false;
	}
}

void Localization::update_region()
{
	if (!pending_update) {
		return;
	}

	// update treasures (i.e. not upgrades)
	Game::PelletOtakara::Mgr* ota_mgr = Game::PelletOtakara::mgr;
	if (ota_mgr) {
		// update all the configs
		int max = ota_mgr->mConfigList->mConfigCnt;
		for (int i = 0; i < max; i++) {
			Game::PelletConfig* config = ota_mgr->mConfigList->getPelletConfig(i);

			// hot-swap GK2 and GK4 treasure names when going to or from PAL
			// NB: this will result in entries being mismatched a bit, so need to handle that when loading the arcs
			const char* name = config->mParams.mName.mData;
			if ((next_treasure_region == Treasure_PAL) || (active_treasure_region == Treasure_PAL)) {
				if (IS_SAME_STRING(name, "g_futa_kyusyu")) {
					name = "g_futa_sikoku";

				} else if (IS_SAME_STRING(name, "g_futa_sikoku")) {
					name = "g_futa_kyusyu";
				}
			}

			// check each config against our swap table to see if we have matches
			for (int j = 0; j < TREASURE_MAP_COUNT; j++) {
				const LocalizationTreasureSwap* treasure = &treasureMap[j];
				// match regions
				if (treasure->region != next_treasure_region) {
					continue;
				}

				// match treasure internal name
				if (!IS_SAME_STRING(name, treasure->internal_name)) {
					continue;
				}

				config->mParams.mArchive.mData        = treasure->archive_name;
				config->mParams.mBmd.mData            = treasure->bmd_name;
				config->mParams.mAnimMgr.mData        = treasure->animMgr_name;
				config->mParams.mColltree.mData       = treasure->collTree_name;
				config->mParams.mRadius.mData         = treasure->bottom_radius;
				config->mParams.mPRadius.mData        = treasure->pick_radius;
				config->mParams.mHeight.mData         = treasure->cyl_height;
				config->mParams.mInertiaScaling.mData = treasure->inertia_scaling;
				config->mParams.mParticleType.mData   = treasure->ptcl_type;
				config->mParams.mNumParticles.mData   = treasure->num_ptcls;
				config->mParams.mParticleSize.mData   = treasure->ptcl_size;
				config->mParams.mFriction.mData       = treasure->friction;
				config->mParams.mMin.mData            = treasure->min_carriers;
				config->mParams.mMax.mData            = treasure->max_carriers;
				config->mParams.mDynamics.mData       = treasure->dyn_type;
				config->mParams.mMoney.mData          = treasure->pokos;
				config->mParams.mUnique.mData         = treasure->unique;
				config->mParams.mIndirect.mData       = treasure->indirect_mats;
				config->mParams.mNumPMotions.mData    = treasure->num_pmotions;
				config->mParams.mDepth.mData          = treasure->bury_depth;
				config->mParams.mDepthMax.mData       = treasure->bury_depth_max;
				config->mParams.mDepthA.mData         = treasure->bury_radius_A;
				config->mParams.mDepthB.mData         = treasure->bury_radius_B;
				config->mParams.mDepthC.mData         = treasure->bury_radius_C;
				config->mParams.mDepthD.mData         = treasure->bury_radius_D;
				config->mParams.mCode.mData           = treasure->render_code;
				config->mParams.mDictionary.mData     = treasure->piklopedia_num;
				config->mParams.mIndirectState        = treasure->indirect_state;
				break;
			}
		}
	}

	// do the same for upgrades
	// (the only two differences are the globes, but it's a short list. it's fine.)
	Game::PelletItem::Mgr* item_mgr = Game::PelletItem::mgr;
	if (item_mgr) {
		// update all the configs
		int max = item_mgr->mConfigList->mConfigCnt;
		for (int i = 0; i < max; i++) {
			Game::PelletConfig* config = item_mgr->mConfigList->getPelletConfig(i);

			// check each config against our swap table to see if we have matches
			for (int j = 0; j < TREASURE_MAP_COUNT; j++) {
				const LocalizationTreasureSwap* treasure = &treasureMap[j];
				// match regions
				if (treasure->region != next_treasure_region) {
					continue;
				}

				// match treasure internal name
				if (!IS_SAME_STRING(config->mParams.mName.mData, treasure->internal_name)) {
					continue;
				}

				config->mParams.mArchive.mData        = treasure->archive_name;
				config->mParams.mBmd.mData            = treasure->bmd_name;
				config->mParams.mAnimMgr.mData        = treasure->animMgr_name;
				config->mParams.mColltree.mData       = treasure->collTree_name;
				config->mParams.mRadius.mData         = treasure->bottom_radius;
				config->mParams.mPRadius.mData        = treasure->pick_radius;
				config->mParams.mHeight.mData         = treasure->cyl_height;
				config->mParams.mInertiaScaling.mData = treasure->inertia_scaling;
				config->mParams.mParticleType.mData   = treasure->ptcl_type;
				config->mParams.mNumParticles.mData   = treasure->num_ptcls;
				config->mParams.mParticleSize.mData   = treasure->ptcl_size;
				config->mParams.mFriction.mData       = treasure->friction;
				config->mParams.mMin.mData            = treasure->min_carriers;
				config->mParams.mMax.mData            = treasure->max_carriers;
				config->mParams.mDynamics.mData       = treasure->dyn_type;
				config->mParams.mMoney.mData          = treasure->pokos;
				config->mParams.mUnique.mData         = treasure->unique;
				config->mParams.mIndirect.mData       = treasure->indirect_mats;
				config->mParams.mNumPMotions.mData    = treasure->num_pmotions;
				config->mParams.mDepth.mData          = treasure->bury_depth;
				config->mParams.mDepthMax.mData       = treasure->bury_depth_max;
				config->mParams.mDepthA.mData         = treasure->bury_radius_A;
				config->mParams.mDepthB.mData         = treasure->bury_radius_B;
				config->mParams.mDepthC.mData         = treasure->bury_radius_C;
				config->mParams.mDepthD.mData         = treasure->bury_radius_D;
				config->mParams.mCode.mData           = treasure->render_code;
				config->mParams.mDictionary.mData     = treasure->piklopedia_num;
				config->mParams.mIndirectState        = treasure->indirect_state;
				break;
			}
		}
	}

	// update previous treasure region to avoid repeated updates
	active_treasure_region = next_treasure_region;
	pending_update         = false;
}

// Add functions from pelletMgr.cpp below that we need to change for treasure swapping; these functions are equivalent but pelletMgr.cpp is
// not-matching, so yehaw time it is

namespace Game {

// @Extracted: pelletMgr.s load__Q24Game13BasePelletMgrFv
void BasePelletMgr::load()
{
	char buffer[512];
	char* file = nullptr;

	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		// @P2GZ localization-swap
		// adjust to use p2gz region instead of system language region
		P2ASSERTBOUNDSINCLUSIVELINE(158, Treasure_US, p2gz->localization_op->get_treasure_region(), Treasure_PAL);
		switch (p2gz->localization_op->get_treasure_region()) {
		case Treasure_JP:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "jpn");
			file = buffer;
			break;
		case Treasure_US:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "us");
			file = buffer;
			break;
		case Treasure_PAL:
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

// @Extracted: pelletMgr.s load_texArc__Q24Game13BasePelletMgrFPc
void BasePelletMgr::load_texArc(char* filename)
{
	char buffer[512];
	char* directory = nullptr;

	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		// @P2GZ localization-swap
		// adjust to use p2gz region instead of system language region
		P2ASSERTBOUNDSINCLUSIVELINE(244, Treasure_US, p2gz->localization_op->get_treasure_region(), Treasure_PAL);
		switch (p2gz->localization_op->get_treasure_region()) {
		case Treasure_JP:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "jpn");
			directory = buffer;
			break;
		case Treasure_US:
			sprintf(buffer, "/user/Abe/Pellet/%s/", "us");
			directory = buffer;
			break;
		case Treasure_PAL:
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
				// @P2GZ localization-swap
				// PAL GK2/4 treasures have incorrect names, so swap them
				// sprintf(path, "%s/%s", config->mParams.mName.mData, config->mParams.mAnimMgr.mData);
				const char* name = config->mParams.mName.mData;
				if (p2gz->localization_op->get_treasure_region() == Treasure_PAL) {
					if (IS_SAME_STRING(name, "g_futa_kyusyu")) {
						name = "g_futa_sikoku";
					} else if (IS_SAME_STRING(name, "g_futa_sikoku")) {
						name = "g_futa_kyusyu";
					}
				}
				sprintf(path, "%s/%s", name, config->mParams.mAnimMgr.mData);
				mAnimMgr[i] = SysShape::AnimMgr::load(textArc, path, data, archive, nullptr);
				if (mAnimMgr[i] == nullptr) {
					mAnimMgr[i] = SysShape::AnimMgr::load(textArc, path, data, archive, nullptr);
				}
			}

			if (config->mParams.mColltree.mData != 0) {
				// @P2GZ localization-swap
				// PAL GK2/4 treasures have incorrect names, so swap them
				// sprintf(path, "%s/%s", config->mParams.mName.mData, config->mParams.mAnimMgr.mData);
				const char* name = config->mParams.mName.mData;
				if (p2gz->localization_op->get_treasure_region() == Treasure_PAL) {
					if (IS_SAME_STRING(name, "g_futa_kyusyu")) {
						name = "g_futa_sikoku";
					} else if (IS_SAME_STRING(name, "g_futa_sikoku")) {
						name = "g_futa_kyusyu";
					}
				}
				sprintf(path, "%s/%s", name, config->mParams.mColltree.mData);
				mCollParts[i] = CollPartFactory::load(textArc, path);
			}
		}
	}
	closeTextArc(textArc);
}

// @Extracted: pelletMgr.s openTextArc__Q24Game13BasePelletMgrFPc
JKRArchive* BasePelletMgr::openTextArc(char* arc)
{
	char directory[512];
	char* file = nullptr;
	if (gGameConfig.mParms.mPelletMultiLang.mData != 0) {
		// @P2GZ localization-swap
		// adjust to use p2gz region instead of system language region
		P2ASSERTBOUNDSINCLUSIVELINE(350, Treasure_US, p2gz->localization_op->get_treasure_region(), Treasure_PAL);
		switch (p2gz->localization_op->get_treasure_region()) {
		case Treasure_JP:
			sprintf(directory, "/user/Abe/Pellet/%s/", "jpn");
			file = directory;
			break;
		case Treasure_US:
			sprintf(directory, "/user/Abe/Pellet/%s/", "us");
			file = directory;
			break;
		case Treasure_PAL:
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
