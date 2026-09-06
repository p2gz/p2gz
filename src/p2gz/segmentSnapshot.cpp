#include <p2gz/SegmentSnapshot.h>
#include <p2gz/p2gz.h>
#include <p2gz/Utility.h>
#include <Game/gameGeneratorCache.h>
#include <Game/gamePlayData.h>
#include <Game/Entities/ItemBridge.h>
#include <System.h>

using namespace gz;

namespace {
struct SnapshotTransfer {
	SnapshotTransfer(u8* data_, bool restoring_)
	    : data(data_)
	    , restoring(restoring_)
	    , size(0)
	{
	}

	void bytes(void* value, int count)
	{
		if (data && count) {
			if (restoring) {
				memcpy(value, data + size, count);
			} else {
				memcpy(data + size, value, count);
			}
		}
		size += count;
	}

	template <typename T>
	void value(T& item)
	{
		bytes(&item, sizeof(item));
	}

	void flags(BitFlags& flags) { bytes(flags.mFlagList, flags.mFlagListSize); }
	void counter(Game::KindCounter& counter) { bytes(counter.mKinds, counter.getNumKinds()); }
	void crop(Game::PelletCropMemory* crop)
	{
		counter(crop->mOtakara);
		counter(crop->mItem);
		counter(crop->mCarcass);
	}

	u8* data;
	bool restoring;
	int size;
};

int transfer_segment_progress(u8* data, bool restoring)
{
	SnapshotTransfer transfer(data, restoring);
	Game::PlayData* pd = Game::playData;
	transfer.value(pd->mHasContainerFlags);
	transfer.value(pd->mHasBootContainerFlags);
	transfer.value(pd->mMeetPikminFlags);
	transfer.value(pd->mStoryFlags);
	transfer.flags(pd->mDemoFlags);
	transfer.flags(pd->mFindItemFlags);
	transfer.value(pd->mOlimarData);
	transfer.crop(pd->mZukanStat);
	transfer.crop(pd->mMainCropMemory);
	transfer.crop(pd->mCaveCropMemory);
	transfer.value(pd->mTreasureCount);
	transfer.value(pd->mSprayCount);
	transfer.value(pd->mBerryCount);
	transfer.value(pd->mPokoCount);
	transfer.value(pd->mCavePokoCount);
	transfer.value(pd->mPokoCountOld);
	transfer.value(pd->mDebtProgressFlags);
	transfer.value(pd->mBackupDebtProgressFlags);
	transfer.value(p2gz->poko_editor->repay_demo_enabled);
	for (int i = 0; i < Game::stageList->mCourseCount; i++) {
		transfer.value(pd->mBitfieldPerCourse[i]);
		transfer.value(pd->mGroundOtakaraCollected[i]);
		transfer.value(pd->mGroundOtakaraCollectedOld[i]);
		transfer.flags(pd->mLimitGen[i].mNonLoops);
		transfer.flags(pd->mLimitGen[i].mLoops);
		Game::PlayData::CaveOtakara* caves[2] = { &pd->mCaveOtakara[i], &pd->mCaveOtakaraOld[i] };
		for (int j = 0; j < 2; j++) {
			transfer.bytes(caves[j]->mOtakaraCountsOld, caves[j]->mCaveCount);
			transfer.bytes(caves[j]->mVisitStatus, caves[j]->mCaveCount * sizeof(int));
		}
	}
	return transfer.size;
}
} // namespace

SegmentSnapshot::SegmentSnapshot()
{
	progress       = nullptr;
	progress_size  = 0;
	cache_data     = nullptr;
	cache_size     = 0;
	course_index   = -1;
	active_captain = NAVIID_Olimar;
}

SegmentSnapshot::~SegmentSnapshot()
{
	delete[] progress;
	delete[] cache_data;
}

void SegmentSnapshot::capture(bool above_ground)
{
	GZASSERTLINE(!progress);
	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	progress_size      = transfer_segment_progress(nullptr, false);
	progress           = new u8[progress_size];
	transfer_segment_progress(progress, false);
	Game::Navi* navi = Game::naviMgr->getActiveNavi();
	active_captain   = navi ? navi->mNaviIndex : NAVIID_Olimar;

	if (above_ground) {
		GZASSERTLINE(!p2gz->warp->warping);
		Game::SingleGameSection* game = get_SGS();
		Game::GeneratorCache* cache   = Game::generatorCache;
		course_index                  = game->mCurrentCourseInfo->mCourseIndex;
		cache->beginSave(course_index);
		FOREACH_NODE(Game::Generator, cache->getFirstGenerator(), gen)
		{
			if (gen->isReservedFlag(Game::Generator::Reserved_doSaveGen)) {
				cache->saveGenerator(gen);
			}
		}
		FOREACH_NODE(Game::Generator, cache->getFirstGenerator(), gen)
		{
			if (gen->isReservedFlag(Game::Generator::Reserved_doSaveGen) && gen->isReservedFlag(Game::Generator::Reserved_doSaveCreature)) {
				cache->saveCreature(gen);
			}
		}
		p2gz->segment_history->capturing_segment_cache = true;
		cache->savePikiheads();
		p2gz->segment_history->capturing_segment_cache = false;
		cache->endSave();
		Game::CourseCache* course = cache->mCurrentCache;
		RamStream header(cache_header, sizeof(cache_header));
		course->write(header);
		GZASSERTLINE(header.mPosition == sizeof(cache_header));
		cache_size = course->mSize;
		cache_data = new u8[cache_size];
		memcpy(cache_data, cache->mHeapBuffer + course->mOffset, cache_size);
		cache->slideCache();
		cache->mCurrentCache = nullptr;

		Iterator<Game::BaseItem> it(Game::ItemBridge::mgr);
		CI_LOOP(it)
		{
			Game::ItemBridge::Item* bridge = static_cast<Game::ItemBridge::Item*>(*it);
			BridgeState state;
			state.position = bridge->getPosition();
			state.glitched = bridge->mIsGlitched;
			bridges.push(state);
		}
	}
	prev_heap->becomeCurrentHeap();
}

void SegmentSnapshot::restore_progress()
{
	GZASSERTLINE(progress && transfer_segment_progress(nullptr, false) == progress_size);
	transfer_segment_progress(progress, true);
	p2gz->warp->set_active_captain(active_captain);
}

void SegmentSnapshot::restore_cache()
{
	if (course_index < 0) {
		return;
	}
	Game::GeneratorCache* cache = Game::generatorCache;
	Game::CourseCache* course   = cache->findCache(cache->mRootCache, course_index);
	if (course) {
		cache->mCurrentCache = course;
		cache->slideCache();
	}
	GZASSERTLINE(cache_size <= cache->mFreeSize);
	cache->beginSave(course_index);
	course = cache->mCurrentCache;
	RamStream header(cache_header, sizeof(cache_header));
	course->read(header);
	course->mOffset = cache->mFreeOffset;
	memcpy(cache->mHeapBuffer + course->mOffset, cache_data, cache_size);
	cache->mFreeOffset += cache_size;
	cache->mFreeSize -= cache_size;
	cache->endSave();
	Game::playData->mBitfieldPerCourse[course_index] |= Game::PlayData::PDCF_Visited;
}

void SegmentSnapshot::restore_post_load()
{
	if (course_index < 0) {
		return;
	}
	Iterator<Game::BaseItem> it(Game::ItemBridge::mgr);
	CI_LOOP(it)
	{
		Game::ItemBridge::Item* bridge = static_cast<Game::ItemBridge::Item*>(*it);
		Vector3f position              = bridge->getPosition();
		for (u32 i = 0; i < bridges.len(); i++) {
			if (bridges[i].position.sqrDistance(position) < 1.0f) {
				bridge->mIsGlitched = bridges[i].glitched;
				for (int j = bridge->mCurrStageIdx + 1; j < bridge->mStageCount; j++) {
					bridge->mStageHealths[j] = bridges[i].glitched ? 0.0f : Game::ItemBridge::mgr->mParms->mBridgeParms.mHealth();
				}
				break;
			}
		}
	}
}
