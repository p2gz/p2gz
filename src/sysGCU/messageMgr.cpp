#include "P2JME/P2JME.h"
#include "System.h"
#include "nans.h"
#include "P2JME/TReference.h"
#include "JSystem/JMessage/TResource.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JMessage/TParse.h"
#include "og/ogLib2D.h"

static const u32 padding[3] = { 0, 0, 0 };

P2JME::Mgr* gP2JMEMgr;

namespace P2JME {

static const char* sFontResName[]
    = { "/message/font_foreign.szs", "/message/font_foreign.szs", "/message/font_foreign.szs", "/message/font_foreign.szs",
	    "/message/font_foreign.szs", "/message/font_jpn.szs",     "/message/font_foreign.szs" };

static const char* sMesResName[]
    = { "/message/mesRes_eng.szs", "/message/mesRes_fra.szs", "/message/mesRes_ger.szs", "/message/mesRes_hol.szs",
	    "/message/mesRes_ita.szs", "/message/mesRes_jpn.szs", "/message/mesRes_spa.szs" };

static const char* cBtnTexName[] = { "a_btn.bti", "b_btn.bti", "c_btn.bti",  "x_btn.bti",  "y_btn.bti", "z_btn.bti",
	                                 "l_btn.bti", "r_btn.bti", "3d_btn.bti", "st_btn.bti", "t_btn.bti" };

/**
 * @note Address: N/A
 * @note Size: 0x1C
 */
const char* getCurrentFontResName()
{
	return sFontResName[sys->mRegion];
}

/**
 * @note Address: N/A
 * @note Size: 0x1C
 */
const char* getCurrentMesResName()
{
	return sMesResName[sys->mRegion];
}

/**
 * @note Address: 0x80437EAC
 * @note Size: 0x40
 */
Mgr* Mgr::create(JKRExpHeap* heap)
{
	new Mgr(heap);
}

/**
 * @note Address: 0x80437EEC
 * @note Size: 0x170
 */
Mgr::Mgr(JKRExpHeap* heap)
    : mRubyFont(nullptr)
    , mImageLists(nullptr)
    , mMaxTextures(nullptr)
    , mIsLoaded(false)
    , _2C(0)
    , mResContainer(nullptr)
    , mMsgRef(nullptr)
// , mLanguageSwapHeap(nullptr)
{
	P2ASSERTLINE(194, !gP2JMEMgr);
	gP2JMEMgr = this;

	sys->heapStatusStart("MessageMgr", nullptr);

	mMaxTextures = new int[1];
	mImageLists  = new JUTTexture**[1];

	// dumb.
	for (int i = 0; i < 1; i++) {
		mMaxTextures[i] = 0;
		mImageLists[i]  = new JUTTexture*;
		mImageLists[i]  = nullptr;
	}

	sys->heapStatusStart("font res", nullptr);
	setupFont("pikmin2main.bfn", heap);
	sys->heapStatusEnd("font res");

	sys->heapStatusStart("tex res", nullptr);
	setupTex();
	sys->heapStatusEnd("tex res");

	sys->heapStatusEnd("MessageMgr");

	JKRHeap* currHeap = JKRGetCurrentHeap();
	// mLanguageSwapHeap = makeExpHeap(0x60000, currHeap, false);
	_2C = 0;
	// mLanguageSwapHeap->becomeCurrentHeap();
	setupMessage();
	currHeap->becomeCurrentHeap();
	mIsLoaded = true;
}

// 	JKRHeap* heap2 = JKRGetCurrentHeap();
// 	_2C            = 0;
// 	setupMessage(); // <--- this is the original code; setup the actual message stuff
// 	// // @P2GZ localization-swap: copy 1^2 code to add support for swapping languages
// 	// // Notably, initialize our new heap that gets used for language swaps here
// 	// {
// 	// 	mLanguageSwapHeap = JKRExpHeap::create(0x20000, sys->mSysHeap, true);
// 	// 	mLanguageSwapHeap->becomeCurrentHeap();
// 	// 	setupMessage(); // <--- this is the original code; setup the actual message stuff
// 	// 	og::gLib2D = nullptr;
// 	// 	og::Lib2D::create();
// 	// }
// 	// End P2GZ edit
// 	heap2->becomeCurrentHeap();
// 	mIsLoaded = true;
// }

/**
 * @note Address: 0x8043805C
 * @note Size: 0x68
 */
Mgr::~Mgr()
{
	gP2JMEMgr = nullptr;
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void Mgr::reloadMessageResource()
{
	// This is most likely used in the PAL region after changing languages
	// Entirely removed for US though

	// @P2GZ localization-swap: we now use this funciton to update text

	JKRHeap* currHeap = JKRGetCurrentHeap();
	// mLanguageSwapHeap->becomeCurrentHeap();
	// mLanguageSwapHeap->freeAll();
	setupMessage();
	// og::gLib2D = nullptr;
	// og::Lib2D::create();
	currHeap->becomeCurrentHeap();

	// // Copy 1^2 code to swap languages in-game
	// JKRHeap* mesgheap = mLanguageSwapHeap; // new p2jme heap
	// mesgheap->freeAll();
	// JKRHeap* old_heap = mesgheap->becomeCurrentHeap();
	// setupMessage();
	// og::gLib2D = nullptr;
	// og::Lib2D::create();
	// old_heap->becomeCurrentHeap();
}

/**
 * @note Address: 0x804380C4
 * @note Size: 0x100
 */
void Mgr::setupMessage()
{
	JKRArchive* msgarc = JKRMountArchive(getCurrentMesResName(), JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
	P2ASSERTLINE(304, msgarc);

	mResContainer      = new JMessage::TResourceContainer;
	mMsgRef            = new TReference;
	mMsgRef->mResource = mResContainer;

	setupMessageResource(msgarc, "pikmin2.bmg");
	setupColor(msgarc, "pikmin2.bmc");
}

/**
 * @note Address: 0x804381C4
 * @note Size: 0xF8
 */
void Mgr::setupTex()
{
	JKRArchive* imgarc = JKRMountArchive("/user/Yamashita/arc/gameTex.szs", JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
	createImage(ImageGroup::ID0, 11);

	ResTIMG* timg;
	for (int i = 0; i < mMaxTextures[0]; i++) {
		// dont even ask.
		timg = (ResTIMG*)JKRGetArchiveImageResource(imgarc, cBtnTexName[i]);
		P2ASSERTLINE(344, timg);
		setImage(ImageGroup::ID0, i, new JUTTexture(timg));
	}
}

/**
 * @note Address: 0x804382BC
 * @note Size: 0x248
 */
void Mgr::setupFont(char const* path, JKRExpHeap* heap)
{
	// Japanese uses a separate font loading system since it's so big
	if (sys->mRegion == System::LANG_Japanese) {
		JKRAram::sAramObject->mAramHeap->getFreeSize();

		JKRArchive* fontarc = JKRMountArchive(getCurrentFontResName(), JKRArchive::EMM_Mem, heap, JKRArchive::EMD_Tail);
		P2ASSERTLINE(368, fontarc);
		sys->heapStatusStart("cacheFont", nullptr);

		ResFONT* file = static_cast<ResFONT*>(fontarc->getResource(path));
		P2ASSERTLINE(371, file);

		mFont = new JUTCacheFont(file, 0x4c2c0, JKRGetCurrentHeap());
		P2ASSERTLINE(374, mFont);

		sys->heapStatusEnd("cacheFont");

		P2ASSERTLINE(378, mFont->mIsValid);
		static_cast<JUTCacheFont*>(mFont)->mPagingType = JUTCacheFont::CFPAGETYPE_Unk1;
		static_cast<JUTCacheFont*>(mFont)->loadCache_string("\tあいうえおかきくけこさしすせそたちすてとなにぬねのはひすへほまみむめもやゆよ"
		                                                    "わん\tアイウエオカキクケコサシスセソ\タチツテトナ"
		                                                    "ニヌネノハヒフヘホマミムメモヤユヨワン\t",
		                                                    true); // this is just all the characters in the japanese font sheet lol
		delete file;
		fontarc->unmount();
	} else {
		JKRArchive* fontarc = JKRMountArchive(getCurrentFontResName(), JKRArchive::EMM_Mem, JKRGetCurrentHeap(), JKRArchive::EMD_Head);
		P2ASSERTLINE(407, fontarc);

		sys->heapStatusStart("resFont", nullptr);

		ResFONT* file = static_cast<ResFONT*>(fontarc->getResource(path));

		mFont = new JUTResFont(file, JKRGetCurrentHeap());

		sys->heapStatusEnd("resFont");
	}
}

/**
 * @note Address: 0x80438504
 * @note Size: 0x1A8
 */
void Mgr::createImage(ImageGroup::EID group, int max)
{
	P2ASSERTBOUNDSLINE(423, 0, group, 1);
	P2ASSERTLINE(426, !mImageLists[group]);
	mMaxTextures[group] = max;
	mImageLists[group]  = new JUTTexture*[max];

	for (int i = 0; i < max; i++) {
		mImageLists[group][i] = nullptr;
	}
}

/**
 * @note Address: 0x804386AC
 * @note Size: 0xC4
 */
void Mgr::setImage(ImageGroup::EID group, int id, JUTTexture* tex)
{
	P2ASSERTBOUNDSLINE(442, 0, group, 1);
	P2ASSERTLINE(444, mMaxTextures[group] > id);

	mImageLists[group][id] = tex;
}

/**
 * @note Address: 0x80438770
 * @note Size: 0xB8
 */
JUTTexture* Mgr::getImage(ImageGroup::EID group, int id)
{
	P2ASSERTBOUNDSLINE(456, 0, group, 1);
	P2ASSERTLINE(458, mMaxTextures[group] > id);

	return mImageLists[group][id];
}

/**
 * @note Address: 0x80438828
 * @note Size: 0x104
 */
void Mgr::setupMessageResource(JKRArchive* arc, char const* path)
{
	sys->heapStatusStart("bmg", nullptr);
	const void* file = arc->getResource(path);
	P2ASSERTLINE(474, file);
	sys->heapStatusEnd("bmg");

	sys->heapStatusStart("メッセージのパース", nullptr); // "Message Parsing"
	JMessage::TParse parse(mResContainer);
	P2ASSERTLINE(484, parse.parse(file, 0));
	sys->heapStatusEnd("メッセージのパース"); // "Message Parsing"
}

/**
 * @note Address: 0x8043892C
 * @note Size: 0xE0
 */
bool Mgr::setupColor(JKRArchive* arc, char const* path)
{
	const void* file = arc->getResource(path);
	P2ASSERTLINE(501, file);

	sys->heapStatusStart("メッセージカラーのパース", nullptr); // "Message Parsing"
	JMessage::TParse_color parse(mResContainer);
	bool success = parse.parse(file, 0x20);
	P2ASSERTLINE(510, success);
	sys->heapStatusEnd("メッセージカラーのパース"); // "Message Parsing"
	return success;
}

} // namespace P2JME
