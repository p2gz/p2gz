#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H

#include "types.h"
#include "Game/Navi.h"

struct P2GZ {
	P2GZ();

	~P2GZ() { }

    void init();
	void update();
	f32 getAnimationCoefficient();

	bool isCameraScroll();
	void setCameraScroll(bool);

	bool mIsScrollingCamera; // controlling camera for warping
	f32 mAnimationCoefficient;
	f32 mDirection;
	Game::NaviWhistle* mWhistle;
	
	J2DPicture* mControlStickPicture;
	J2DPicture* mCStickPicture;
	J2DPicture* mBButtonPicture;
	J2DPicture* mLButtonPicture;
};

extern P2GZ* p2gz;

#endif
