#include "GlobalData.h"

P2GZ* p2gz;

SeedHistoryTracker::SeedHistoryTracker()
{
    seedBufHead = 0;
}

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
    seedHistory = SeedHistoryTracker();
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

SeedHistoryTracker::SeedRecord::SeedRecord(u32 seed, int floorIndex) {
    this->seed = seed;
    this->floorIndex = floorIndex;
}

void SeedHistoryTracker::push(u32 seed, int floorIndex) {
    seedRingBuf[seedBufHead] = SeedRecord(seed, floorIndex);
    seedBufHead = (seedBufHead + 1) % SEED_HISTORY_SIZE;
}

SeedHistoryTracker::SeedRecord SeedHistoryTracker::pop() {
    seedBufHead = (seedBufHead - 1) % SEED_HISTORY_SIZE;
    SeedRecord r = seedRingBuf[seedBufHead];
    return r;
}

SeedHistoryTracker::SeedRecord SeedHistoryTracker::peek() {
    return seedRingBuf[(seedBufHead - 1) % SEED_HISTORY_SIZE];
}