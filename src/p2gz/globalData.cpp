#include "GlobalData.h"
#include "Game/Piki.h"
#include "Game/PikiMgr.h"
#include "Game/PikiState.h"
#include "Game/MapMgr.h"

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
    history = new RingBuffer<64, SegmentRecord>;
    bugPokosCollectedSinceLoad = 0;
    treasurePokosCollectedSinceLoad = 0;

    mSelectedArea = 0;
    mSelectedDestination = 0;
    mSublevelNumber = 1;
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