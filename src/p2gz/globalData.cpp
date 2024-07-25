#include "GlobalData.h"
#include "GZMacros.h"
#include "DefaultPresets.h"
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
    mSelectedPreset = 0;
    mSublevelNumber = 1;
    
    showTimer = true;

    size_t numPresets = sizeof(DEFAULT_PRESETS) / sizeof(DEFAULT_PRESETS[0]);
    mPresets = gzCollections::Vec<Preset>(numPresets);
    for (size_t i = 0; i < numPresets; i++) {
        const char* presetString = DEFAULT_PRESETS[i];
        mPresets.push(Preset(presetString));
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

u32 P2GZ::getDefaultPresetId(int area, int destination, int sublevel) {
    switch (area) {
    case 0: // VoR
        switch (destination) {
        case 0: // AG
            if (sublevel < 7) return 15;
            else return 16;
        case 1: // EC
            return 0;
        case 2: // SCx
            if (sublevel < 5) return 17;
            else return 19;
        case 3: // FC
            return 21;
        }
    case 1: // AW
        switch (destination) {
        case 0: // AG
            return 9; // hardcoded for Enter BK
        case 1: // HoB
            if (sublevel < 3) return 2;
            else if (sublevel < 5) return 3;
            else return 4;
        case 2: // WFG
            if (sublevel < 4) return 4;
            else return 5;
        case 3: // BK
            return 13;
        case 4: // SH
            if (sublevel < 3) return 6;
            else return 7;
        }
    case 2: // PP
        switch (destination) {
        case 0: // AG
            return 23;
        case 1: // CoS
        case 2: // GK
            return 24;
        }
    }
    
    return 26; // testing preset. replace when we have presets for AT and stuff
}

void P2GZ::applyPreset(Preset& preset) {
    playData->mCaveSaveData.mCavePikis = preset.mSquad;
    playData->mPikiContainer = preset.mSquad;
    playData->mSprayCount[0] = preset.mNumSpicies;
    playData->mSprayCount[1] = preset.mNumBitters;
    // TODO: onion pikmin
}

/// @brief Creates a Preset from a short-form preset string.
/// @param presetString A "preset string" like "name:5R10b15p1s2n".
/// Each Pikmin type in the squad is represented by a number followed by a single
/// letter for their color. Lower-case is leaf, and upper-case is flower. Sprays
/// are specified with the letter 's' for spicies and 'n' for bitters (from the
/// Japanese "nigai" since B is already taken for blue Pikmin.)
Preset::Preset(const char* presetString) {
    // just to be sure
    mNumBitters = 0;
	mNumSpicies = 0;

    u32 count = 0;
    u32 i = 0;
    bool msgIdDone = false;
    bool inOnion = false;
    while (presetString[i] != '\0') {
        char c = presetString[i];
        if (msgIdDone) {
            // if c is a digit character
            if (c >= '0' && c <= '9') {
                count *= 10; // shift the existing count to the left one (base-10) place
                count += c - '0'; // convert digit character to integer
            }
            // if c is a letter
            else if (c >= 'A' && c <= 'z') {
                if (c == 'O') {
                    inOnion = true;
                }
                else {
                    _addByCharCode(c, count, inOnion);
                    count = 0;
                    inOnion = false;
                }
            }
            else {
                // no other characters are valid in these strings
                GZASSERTLINE(0);
            }
        }
        else {
            if (c == ':') {
                mMsgId <<= 24;
                mMsgId += '_00';
                msgIdDone = true;
            }
            else {
                mMsgId <<= 8;
                mMsgId += c;
            }
        }
        
        i++;
    }
}

void Preset::_addByCharCode(char code, u32 count, bool inOnion) {
    PikiContainer* container;
    if (inOnion) container = &mOnionPikis;
    else container = &mSquad;
    switch (code) {
    case 'r':
        container->getCount(Red, Leaf) += count;
        break;
    case 'R':
        container->getCount(Red, Flower) += count;
        break;
    case 'y':
        container->getCount(Yellow, Leaf) += count;
        break;
    case 'Y':
        container->getCount(Yellow, Flower) += count;
        break;
    case 'b':
        container->getCount(Blue, Leaf) += count;
        break;
    case 'B':
        container->getCount(Blue, Flower) += count;
        break;
    case 'p':
        container->getCount(Purple, Leaf) += count;
        break;
    case 'P':
        container->getCount(Purple, Flower) += count;
        break;
    case 'w':
        container->getCount(White, Leaf) += count;
        break;
    case 'W':
        container->getCount(White, Flower) += count;
        break;
    case 's':
        mNumSpicies += count;
        break;
    case 'n':
        mNumBitters += count;
        break;
    default:
        GZASSERTLINE(0);
    }
}
