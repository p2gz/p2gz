#include <p2gz/TreasureEditor.h>
#include <p2gz/FreeCam.h>
#include <Game/Entities/PelletItem.h>
#include <Game/Entities/PelletOtakara.h>
#include <Game/MapMgr.h>
#include <Game/pelletMgr.h>
#include <VsOtakaraName.h>
#include <p2gz/p2gz.h>

namespace gz {
void TreasureEditor::init()
{
	treasures = static_cast<ListMenu*>(p2gz->menu->get_option("map/treasures")->get_sub_menu());
}

void TreasureEditor::enable()
{
	enabled = true;

	Game::Pellet* target = nullptr;

	Iterator<Game::PelletOtakara::Object> treasureIterator(Game::PelletOtakara::mgr);
	CI_LOOP(treasureIterator)
	{
		Game::PelletOtakara::Object* treasure = *treasureIterator;
		if (strcmp(treasure->getConfigName(), treasures->cur_option()->title) == 0) {
			target = treasure;
		}
		treasure->mLod.setFlag(AILOD_IsVisibleBoth);
	}

	Iterator<Game::PelletItem::Object> upgradeIterator(Game::PelletItem::mgr);
	CI_LOOP(upgradeIterator)
	{
		Game::PelletItem::Object* treasure = *upgradeIterator;
		if (strcmp(treasure->getConfigName(), treasures->cur_option()->title) == 0) {
			target = treasure;
		}
		treasure->mLod.setFlag(AILOD_IsVisibleBoth);
	}

	GZASSERTLINE(target);
	active_treasure  = target;
	initial_position = target->getPosition();

	p2gz->waypoint_viewer->toggle(true);
	p2gz->freecam->enable();

	// We could have a target Creature that freecam follows, or we could just do this,
	// which feels like a hack but took zero effort.
	p2gz->freecam->set_position(target->getPosition());
}

void TreasureEditor::snap_to_nearest_waypoint()
{
	Game::WayPoint* nearest = nullptr;
	Iterator<Game::WayPoint> iterator(Game::mapMgr->mRouteMgr);
	CI_LOOP(iterator)
	{
		Game::WayPoint* wp = *iterator;
		if (!nearest) {
			nearest = wp;
			continue;
		}

		Vector3f treasurePos = active_treasure->getPosition();
		Vector3f currentPos  = wp->getPosition();
		Vector3f nearestPos  = nearest->getPosition();
		if (sqrDistanceXZ(treasurePos, currentPos) < sqrDistanceXZ(treasurePos, nearestPos)) {
			nearest = wp;
		}
	}

	GZASSERTLINE(nearest);
	Vector3f pos = Vector3f(nearest->getPosition().x, p2gz->freecam->get_position().y, nearest->getPosition().z);
	p2gz->freecam->set_position(pos);
}

void TreasureEditor::add(Game::Pellet* pellet)
{
	Game::PelletItem::Object* treasure = static_cast<Game::PelletItem::Object*>(pellet);

	// clang-format off
	treasures->push(new OpenSubMenuOption(treasure->getConfigName(), (new ListMenu())
		->push(new PerformActionMenuOption("move", new Delegate<TreasureEditor>(p2gz->treasure_editor, &TreasureEditor::enable)))
		->push(new ToggleMenuOption("collected", false, new Delegate1<TreasureEditor, bool>(p2gz->treasure_editor, &TreasureEditor::toggle_collected)))
	));
	// clang-format on
}

void TreasureEditor::clear_treasures()
{
	if (treasures) {
		treasures->clear();
	}
}
} // namespace gz

namespace Game {
void Pellet::onInit(CreatureInitArg* initArg)
{
	mMaxCarriers = -1;
	mMinCarriers = -1;
	mWallTimer   = 0;
	mIsBounced   = false;
	mIsInWater   = false;

	clearDiscoverDisable();

	mClaim         = 0;
	mAngleOffset   = 0.0f;
	mFloorTriangle = nullptr;
	mHasCollided   = 0;
	mFaceDir       = 0.0f;
	mAnimSpeed     = 0.0f;
	mIsCaptured    = 0;
	mPickFlags     = 0;
	mCarryInfoList = nullptr;

	clearCapture();

	mCarryPower = 0.0f;

	P2ASSERTLINE(1632, initArg);

	u16 stateType = static_cast<PelletInitArg*>(initArg)->mState;
	if (stateType == PelBirthType_Normal) {
		mPelletSM->start(this, PELSTATE_Normal, nullptr);
		mScale = Vector3f(1.0f);
	} else if (stateType == PelBirthType_Piklopedia) {
		mPelletSM->start(this, PELSTATE_Zukan, nullptr);
	} else if (stateType == PelBirthType_Appear) {
		mPelletSM->start(this, PELSTATE_Appear, nullptr);
		mScale = Vector3f(0.01f);
	} else {
		mPelletSM->start(this, PELSTATE_ScaleAppear, nullptr);
		mScale = Vector3f(0.01f);
	}

	mPikminCount[Blue]    = 0;
	mPikminCount[Red]     = 0;
	mPikminCount[Yellow]  = 0;
	mPikminCount[Purple]  = 0;
	mPikminCount[White]   = 0;
	mPikminCount[Bulbmin] = 0;
	mPikminCount[Carrot]  = 0;
	mTotalCarriers        = 0;
	mPelletSizeType       = (u16) static_cast<PelletInitArg*>(initArg)->mPelletIndex;

	mConfig = mMgr->mConfigList->getPelletConfig(static_cast<PelletInitArg*>(initArg)->mTextIdentifier);

	if (!(mConfig->mParams.mCode.mData & 1)) {
		shadowOn();
	}

	if (static_cast<PelletInitArg*>(initArg)->mAdjustWeightForSquad) {
		mMinCarriers = GameStat::getMapPikmins(AllPikminCalcs);
		int minPikis = mConfig->mParams.mMin.mData;
		if (mMinCarriers > minPikis) {
			mMinCarriers = minPikis;
		}
	} else {
		mMinCarriers = -1;
	}

	if ((static_cast<PelletInitArg*>(initArg)->mMinCarriers != -1) && (static_cast<PelletInitArg*>(initArg)->mMaxCarriers != -1)) {
		mMinCarriers = static_cast<PelletInitArg*>(initArg)->mMinCarriers;
		mMaxCarriers = static_cast<PelletInitArg*>(initArg)->mMaxCarriers;
	}

	mRigid.mFlags.unset(0x01);
	mRigid.mFlags.unset(0x80);
	mPelletColor = static_cast<PelletInitArg*>(initArg)->mPelletColor;

	if (!strcmp(getConfigName(), "loozy")) {
		mPelletFlag = FLAG_LOOZY;
	} else if (!strcmp(getConfigName(), "orima")) {
		mPelletFlag = FLAG_NAVI_NAPSACK;
	} else {
		mPelletFlag = FLAG_NONE;
	}

	if (!strcmp(getConfigName(), VsOtakaraName::cBedamaYellow)) {
		mPelletFlag = FLAG_VS_BEDAMA_YELLOW;
	} else if (!strcmp(getConfigName(), VsOtakaraName::cBedamaRed)) {
		mPelletFlag = FLAG_VS_BEDAMA_RED;
	} else if (!strcmp(getConfigName(), VsOtakaraName::cBedamaBlue)) {
		mPelletFlag = FLAG_VS_BEDAMA_BLUE;
	} else if (!strcmp(getConfigName(), VsOtakaraName::cCoin)) {
		mPelletFlag = FLAG_VS_CHERRY;
	}

	if (static_cast<PelletInitArg*>(initArg)->mDoSkipCreateModel == 0) {
		mModel = mMgr->createShape(mPelletSizeType, mSlotIndex);
		onCreateShape();
	}

	if (mModel) {
		mCarryAnim.mAnimMgr = mMgr->mAnimMgr[mPelletSizeType];
		mRadius             = 2.0f * mModel->getRoughBoundingRadius();
	} else {
		mRadius = mConfig->mParams.mRadius.mData;
	}

	if (mCarryAnim.mAnimMgr) {
		SysShape::MotionListener* listener = this;
		mCarryAnim.startAnim(0, listener);
		stop_carrymotion();
		init_pmotions();
		start_pmotions();
		if ((gameSystem->isZukanMode()) && (mPelletFlag == FLAG_LOOZY)) {
			mAnimSpeed = 30.0f;
		}
	}

	mMgr->setCollTree(this, mPelletSizeType);

	if (strcmp(mConfig->mParams.mDynamics.mData, "never") == 0) {
		mDynamicType = 2;
		mIsDynamic   = false;

		mConfig->mParams.mNumParticles.mData = 0;
	} else if (strcmp(mConfig->mParams.mDynamics.mData, "lod") == 0) {
		mDynamicType = 1;
		mIsDynamic   = true;
	} else {
		mDynamicType = 0;
		mIsDynamic   = true;
	}

	mSlots.mSlots[0]  = 0;
	mSlots.mSlots[1]  = 0;
	mSlots.mSlots[2]  = 0;
	mSlots.mSlots[3]  = 0;
	mSlots.mSlots[4]  = 0;
	mSlots.mSlots[5]  = 0;
	mSlots.mSlots[6]  = 0;
	mSlots.mSlots[7]  = 0;
	mSlots.mSlots[8]  = 0;
	mSlots.mSlots[9]  = 0;
	mSlots.mSlots[10] = 0;
	mSlots.mSlots[11] = 0;
	mSlots.mSlots[12] = 0;
	mSlots.mSlots[13] = 0;
	mSlots.mSlots[14] = 0;
	mSlots.mSlots[15] = 0;
	mIsAlwaysCarried  = 0;

	mSlotCount = getPelletConfigMax();

	if (mSlotCount > 128) {
		mSlotCount = 128;
	}

	// For Doomsday Apparatus (because it couldnt just have 1000 in the config... apparently)
	if (mConfig->mParams.mMin.mData == 128) {
		mMinCarriers = 1000;
		mMaxCarriers = 1000;
	}

	mDynParticle = nullptr;
	setupParticles();
	do_onInit(initArg);

	if (getKind() == PelletType::Treasure) {
		Radar::Mgr::entry(this, Radar::MAP_TREASURE, 0);
	} else if (getKind() == PelletType::Upgrade) {
		Radar::Mgr::entry(this, Radar::MAP_UPGRADE, 0);
	}

	setAlive(true);

	if (gameSystem->isVersusMode()) {
		GameMessagePelletBorn msg(this);
		gameSystem->mSection->sendMessage(msg);
	}

	if (getKind() == PelletType::Treasure || getKind() == PelletType::Upgrade) {
		OSReport("added %s\n", getConfigName());
		p2gz->treasure_editor->add(this);
	}
}
} // namespace Game
