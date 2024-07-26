#include "GlobalData.h"
#include "GZMacros.h"
#include "Game/Piki.h"
#include "Game/PikiMgr.h"
#include "Game/PikiState.h"
#include "Game/MapMgr.h"
#include "JSystem/J2D/J2DPrint.h"

using namespace Game;

P2GZ* p2gz;

P2GZ::P2GZ()
{
    mIsScrollingCamera = false;
    mIsSaveLoadPosition = false;
    mScrollCameraZoom = 768;

    mSelectedPositionSlot = 0;
    for (int i = 0; i < 4; i++) {
        mSavedPositions[i] = Vector3f::zero;
    }
    mControlStickPicture = nullptr;
    mCStickPicture = nullptr;
    mAButtonPicture = nullptr;
    mBButtonPicture = nullptr;
    mXButtonPicture = nullptr;
    mYButtonPicture = nullptr;
    mLButtonPicture = nullptr;
    mRButtonPicture = nullptr;
    mZButtonPicture = nullptr;

    mAnimationCoefficient = 0.0f;
    mDirection = 1.0f;

    setCustomNextSeed = false;
    nextSeed = 0;
    history = new gzCollections::RingBuffer<64, SegmentRecord>;
    bugPokosCollectedSinceLoad = 0;
    treasurePokosCollectedSinceLoad = 0;

    mSelectedArea = 0;
    mSelectedDestination = 0;
    mSublevelNumber = 1;
    mSelectedPresetIndex = 0;
    
    showTimer = true;

    #include "DefaultPresets.h" // God is dead and it's all our fault
    size_t numPresets = sizeof(DEFAULT_PRESETS) / sizeof(DEFAULT_PRESETS[0]);
    mPresets = gzCollections::Vec<Preset>(numPresets);
    for (size_t i = 0; i < numPresets; i++) {
        Preset preset = DEFAULT_PRESETS[i];
        mPresets.push(preset);
    }
}

void P2GZ::init()
{
    JKRArchive* arc = JKRMountArchive("/new_screen/eng/res_s_menu_controller.szs", JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);

    ResTIMG* controlStickTimg = JKRGetImageResource("c_anim_otah_32.9.bti", arc);
    ResTIMG* cStickTimg = JKRGetImageResource("c_stick_anim_otah_32.5.bti", arc);
    ResTIMG* aButtonTimg = JKRGetImageResource("ia4_abtn.bti", arc);
    ResTIMG* bButtonTimg = JKRGetImageResource("ia4_bbtn.bti", arc);
    ResTIMG* xButtonTimg = JKRGetImageResource("x_btn.bti", arc);
    ResTIMG* yButtonTimg = JKRGetImageResource("y_btn.bti", arc);
    ResTIMG* lButtonTimg = JKRGetImageResource("l_btn.bti", arc);
    ResTIMG* rButtonTimg = JKRGetImageResource("r_btn.bti", arc);
    ResTIMG* zButtonTimg = JKRGetImageResource("z_btn.bti", arc);

    mControlStickPicture = new J2DPicture(new JUTTexture(controlStickTimg));
    mCStickPicture = new J2DPicture(new JUTTexture(cStickTimg));
    mAButtonPicture = new J2DPicture(new JUTTexture(aButtonTimg));
    mBButtonPicture = new J2DPicture(new JUTTexture(bButtonTimg));
    mXButtonPicture = new J2DPicture(new JUTTexture(xButtonTimg));
    mYButtonPicture = new J2DPicture(new JUTTexture(yButtonTimg));
    mLButtonPicture = new J2DPicture(new JUTTexture(lButtonTimg));
    mRButtonPicture = new J2DPicture(new JUTTexture(rButtonTimg));
    mZButtonPicture = new J2DPicture(new JUTTexture(zButtonTimg));
}

void P2GZ::update()
{
    if (mAnimationCoefficient >= 1.0f) {
        mDirection = -1.0f;
    } else if (mAnimationCoefficient <= 0.0f) {
        mDirection = 1.0f;
    }
    mAnimationCoefficient += mDirection / 30;
}

f32 P2GZ::getAnimationCoefficient()
{
    return mAnimationCoefficient;
}

Preset& P2GZ::getPresetByMsgId(s64 msgId) {
    for (size_t i = 0; i < mPresets.len(); i++) {
        Preset& preset = mPresets[i];
        if (preset.mMsgId == msgId) return preset;
    }
    GZASSERTLINE(0);
}

s64 P2GZ::getDefaultPresetId(int area, int destination, int sublevel) {
    switch (area) {
    case 0: // VoR
        switch (destination) {
        case 0: // AG
            if (sublevel < 7) return '3215_00';
            else return '3216_00';
        case 1: // EC
            return '3200_00';
        case 2: // SCx
            if (sublevel < 5) return '3217_00';
            else return '3219_00';
        case 3: // FC
            return '3221_00';
        }
    case 1: // AW
        switch (destination) {
        case 0: // AG
            return '3209_00'; // hardcoded for Enter BK
        case 1: // HoB
            if (sublevel < 3) return '3202_00';
            else if (sublevel < 5) return '3203_00';
            else return '3204_00';
        case 2: // WFG
            if (sublevel < 4) return '3204_00';
            else return '3205_00';
        case 3: // BK
            return '3213_00';
        case 4: // SH
            if (sublevel < 3) return '3206_00';
            else return '3207_00';
        }
    case 2: // PP
        switch (destination) {
        case 0: // AG
            return '3223_00';
        case 1: // CoS
        case 2: // GK
            return '3224_00';
        }
    }
    
    return '3299_00'; // testing preset
}

void P2GZ::applyPreset(Preset& preset) {
    playData->mCaveSaveData.mCavePikis = preset.mSquad;
    playData->mPikiContainer = preset.mSquad;
    playData->mSprayCount[0] = preset.mNumSpicies;
    playData->mSprayCount[1] = preset.mNumBitters;
    // TODO: onion pikmin
}

Preset& Preset::setMsgId(s64 msgId) {
    mMsgId = msgId;
    return *this;
}

Preset& Preset::setPikmin(int happa, int color, int amount) {
    mSquad.getCount(color, happa) += amount;
    return *this;
}

Preset& Preset::setOnionPikmin(int happa, int color, int amount) {
    mOnionPikis.getCount(color, happa) += amount;
    return *this;
}

Preset& Preset::setSprays(int spicies, int bitters) {
    mNumSpicies = spicies;
    mNumBitters = bitters;
    return *this;
}
