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
    mAnimationCoefficient = 0.0f;
    mDirection = 1.0f;
    mWhistle = nullptr;
    mBButtonPicture = nullptr;
    mLButtonPicture = nullptr;
    mControlStickPicture = nullptr;
    mCStickPicture = nullptr;
    setCustomNextSeed = false;
    nextSeed = 0;
    history = new RingBuffer<64, SegmentRecord>;
    bugPokosCollectedSinceLoad = 0;
    treasurePokosCollectedSinceLoad = 0;
}

void P2GZ::init()
{
    JKRArchive* arc = JKRMountArchive("/new_screen/eng/res_s_menu_controller.szs", JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);

    ResTIMG* controlStickTimg = JKRGetImageResource("c_anim_otah_32.9.bti", arc);
    ResTIMG* cStickTimg = JKRGetImageResource("c_stick_anim_otah_32.5.bti", arc);
    ResTIMG* bButtonTimg = JKRGetImageResource("ia4_bbtn.bti", arc);
    ResTIMG* lButtonTimg = JKRGetImageResource("l_btn.bti", arc);

    mControlStickPicture = new J2DPicture(new JUTTexture(controlStickTimg));
    mCStickPicture = new J2DPicture(new JUTTexture(cStickTimg));
    mBButtonPicture = new J2DPicture(new JUTTexture(bButtonTimg));
    mLButtonPicture = new J2DPicture(new JUTTexture(lButtonTimg));
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