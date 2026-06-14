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
	RadioMenuOption* region_opt = static_cast<RadioMenuOption*>(p2gz->menu->get_option("level/treasure region"));

	// NB: make sure this order matches the order in the TreasureRegion enum
	region_opt->options.push("US");
	region_opt->options.push("JP");
	region_opt->options.push("PAL");

	// reflect the saved-to-mem-card region pref in the menu
	region_opt->set_selection(p2gz->settings->treasure_region);
}

void Localization::set_treasure_region(size_t new_treasure_region_id)
{
	next_treasure_region = (TreasureRegion)new_treasure_region_id;
}

void Localization::update_region()
{
	if (!require_update()) {
		return;
	}

	// update regular treasures
	Game::PelletOtakara::Mgr* ota_mgr = Game::PelletOtakara::mgr;
	if (ota_mgr) {
		int max = ota_mgr->mConfigList->mConfigCnt;
		for (int i = 0; i < max; i++) {
			Game::PelletConfig* config = ota_mgr->mConfigList->getPelletConfig(i);

			// hot-swap GK2 and GK4 treasure names when going to or from PAL
			// NB: this will result in entries being mismatched a bit, so need to handle that when loading the arcs
			const char* name = config->mParams.mName.mData;
			if ((next_treasure_region == Treasure_PAL) || (active_treasure_region == Treasure_PAL)) {
				if (!strcmp(name, "g_futa_kyusyu")) {
					name = "g_futa_sikoku";

				} else if (!strcmp(name, "g_futa_sikoku")) {
					name = "g_futa_kyusyu";
				}
			}

			localize_config(name, config);
		}
	}

	// do the same for upgrades (EK kit treasures)
	Game::PelletItem::Mgr* item_mgr = Game::PelletItem::mgr;
	if (item_mgr) {
		int max = item_mgr->mConfigList->mConfigCnt;
		for (int i = 0; i < max; i++) {
			Game::PelletConfig* config = item_mgr->mConfigList->getPelletConfig(i);

			localize_config(config->mParams.mName.mData, config);
		}
	}

	// update previous treasure region to avoid repeated updates
	active_treasure_region = next_treasure_region;
}

void Localization::localize_config(const char* name, Game::PelletConfig* config)
{
	for (int j = 0; j < TREASURE_MAP_COUNT; j++) {
		const LocalizedTreasureConfig* treasure = &treasure_region_map[j];
		if (treasure->region != next_treasure_region) {
			continue;
		}

		if (strcmp(name, treasure->internal_name)) {
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
