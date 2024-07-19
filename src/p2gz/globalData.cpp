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

void P2GZ::setSquad(Game::PikiContainer* squad, bool falling) {
    int oldBirthMode = PikiMgr::mBirthMode;
    PikiMgr::mBirthMode = PikiMgr::PSM_Replace;
    pikiMgr->killAllPikmins();

    OSReport("total sum : %d\n", squad->getTotalSum());
    
    // Spawn the new squad
    for (int color = 0; color < PikiColorCount; color++) {
        for (int happa = 0; happa < PikiGrowthStageCount; happa++) {
            for (int i = 0; i < squad->getCount(color, happa); i++) {
                Piki* piki = pikiMgr->birth();

                Game::PikiInitArg arg(PIKISTATE_Tane);
                piki->init(&arg);
                piki->changeHappa(happa);
                piki->changeShape(color);

                Vector3f pos = naviMgr->getActiveNavi()->getPosition();
                piki->setPosition(pos, false);
                piki->mNavi = naviMgr->getActiveNavi();
                if (falling) {
                    Vector3f temp = piki->getPosition();
                    temp.y        = mapMgr->getMinY(temp);
                    piki->setPosition(temp, false);
                    piki->mVelocity       = 0.0f;
                    piki->mTargetVelocity = 0.0f;
                }
            }
        }
    }
    PikiMgr::mBirthMode = oldBirthMode;
}