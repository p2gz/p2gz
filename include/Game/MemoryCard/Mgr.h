#ifndef _GAME_MEMORYCARD_MGR_H
#define _GAME_MEMORYCARD_MGR_H

#include "MemoryCardMgr.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "stream.h"
#include "types.h"
#include <Game/MemoryCard/Player.h> // @P2GZ: make memory card files equivalent

struct Stream;

namespace Game {
namespace MemoryCard {
struct PlayerFileInfo;
struct PlayerInfoHeader;

// @P2GZ: make memory card files equivalent
// struct PlayerInfo {
// 	u32 mMagic;       // _00
// 	u32 mVersionType; // _04
// };
struct PlayerInfo : public PlayerInfoHeader {
	// _0000-_0040 = PlayerInfoHeader
	u8 _0040[0xBFBC]; // _0040
	u32 mChecksum;    // _BFFC
};

struct OptionInfo {
	// @P2GZ: make memory card files equivalent
	// u32 mMagic;         // _00
	// u32 mVersionType;   // _04
	// u32 mSaveSlotIndex; // _08
	u32 mMagic;         // _0000
	u32 mVersionType;   // _0004
	u32 mSaveSlotIndex; // _0008
	u8 _000C[0x1FF0];   // _000C
	u32 mChecksum;      // _1FFC
};

enum MemoryCardMgrFlags {
	MCMFLAG_IsWriting = 0x1,
};

struct Mgr : public MemoryCardMgr {
	// @P2GZ: settings on mem card
	// layout of dedicated settings file (see cP2GZFileName)
	// the file is pre-sized with spare data capacity so the written payload can grow later
	// - to increase size, change P2GZ_PAYLOAD_SIZE from 0x2000 up to a max of 0x1E000
	enum P2GZDataLayout {
		P2GZ_FILE_SIZE    = 0x20000, // total allocation: 0x2000 header + 0x1E000 data capacity (16 blocks)
		P2GZ_DATA_OFFSET  = 0x2000,  // payload begins right after the header block
		P2GZ_PAYLOAD_SIZE = 0x2000,  // bytes currently written/read/checksummed (may grow to <= 0x1E000)
	};

	Mgr();

	virtual ~Mgr() { }                                     // _08 (weak)
	virtual void update();                                 // _0C
	virtual bool doCardProc(void*, MemoryCardMgrCommand*); // _14
	virtual u32 getHeaderSize() { return 0x2000; }         // _18 (weak)
	virtual void doMakeHeader(u8*);                        // _1C
	virtual void doSetCardStat(CARDStat*);                 // _20
	virtual bool doCheckCardStat(CARDStat*);               // _24
	virtual bool isErrorOccured();                         // _28

	enum MemoryCardStatus {
		MCS_Ready            = 0,
		MCS_NoCard           = 1,
		MCS_IOError          = 2,
		MCS_WrongDevice      = 3,
		MCS_WrongSector      = 4,
		MCS_Broken           = 5,
		MCS_Encoding         = 6,
		MCS_NoFileSpace      = 7,
		MCS_NoFileEntry      = 8,
		MCS_FileOpenError    = 9,
		MCS_SerialNoError    = 10,
		MCS_11               = 11,
		MCS_12               = 12,
		MCS_13               = 13,
		MCS_PlayerDataBroken = 14,
	};

	void loadResource(JKRHeap*);
	void destroyResource();
	u32 getCardStatus(); // MemoryCardStatus
	bool format();
	bool checkBeforeSave();
	bool checkError();
	bool createNewFile();
	bool saveGameOption();
	bool loadGameOption();
	bool savePlayerNoCheckSerialNumber(int);
	bool savePlayer(int);
	bool loadPlayer(int);
	bool deletePlayer(int);
	bool copyPlayer(int, int);
	bool getPlayerHeader(PlayerFileInfo*);
	bool commandUpdatePlayerHeader(PlayerFileInfo*);
	bool commandCheckBeforeSave();
	bool commandCheckError();
	bool checkSpace(MemoryCardMgr::ECardSlot);
	bool commandSaveHeader();
	bool commandCreateNewFile();
	bool dataFormat(MemoryCardMgr::ECardSlot);
	bool varifyCardStatus();
	bool commandSaveGameOption(bool, bool);
	bool commandLoadGameOption();
	void writeGameOption(Stream&);
	void readGameOption(Stream&);
	bool checkSerialNo(bool);
	bool commandSavePlayer(s8, bool);
	bool commandSavePlayerNoCheckSerialNo(s8, bool);
	bool getPlayerInfo(s8, PlayerInfoHeader*, bool*);
	int getIndexPlayerInfo(s8, PlayerInfoHeader*, bool*);
	bool commandLoadPlayer(s8);
	bool loadPlayerForNoCard(s8);
	bool loadPlayerProc(s8, u8*);
	bool commandDeletePlayer(s8);
	bool savePlayerProc(s8, u8*, bool);
	bool commandCheckSerialNo();
	bool commandCopyPlayer(s8, s8);
	void writePlayer(Stream&);
	void readPlayer(Stream&);
	bool writeBrokenData(MemoryCardMgr::ECardSlot);
	bool checkOptionInfo(OptionInfo*);
	u32 calcCheckSumOptionInfo(OptionInfo*);
	bool testCheckSumOptionInfo(OptionInfo*);
	bool checkPlayerInfo(PlayerInfo*);
	u32 calcCheckSumPlayerInfo(PlayerInfo*);
	bool testCheckSumPlayerInfo(PlayerInfo*);
	bool writeInvalidGameOption();
	bool writeInvalidPlayerInfoAll();
	bool writeInvalidPlayerInfo(int, s8);
	bool checkPlayerNoPlayerInfo(int, s8, PlayerInfoHeader*);
	bool getIndexInvalidPlayerInfo(int*, s8*, s8, u32, bool);
	bool modifyPlayerInfo(s8, bool*);
	bool verifyCardSerialNo(u64*, MemoryCardMgr::ECardSlot);
	bool resetError();

	inline void setFlag(u32 flag) { mFlags.typeView |= flag; }
	inline void resetFlag(u32 flag) { mFlags.typeView &= ~flag; }
	inline bool isFlag(u32 flag) const { return mFlags.typeView & flag; }

	inline bool checkCheckSum(u32* buffer);
	inline bool checkInfo(u32* buffer);

	inline bool isCardReady() { return (int)getCardStatus() == MCS_Ready; }

	inline bool isCardNotReady() { return (int)getCardStatus() != MCS_Ready; }

	inline bool isCardInvalid() { return !mIsCard && checkStatus() != MCS_11; }

	// @P2GZ: settings on mem card
	// save and load wrappers for P2GZ data file
	bool saveP2GZData();
	bool loadP2GZData();
	bool commandSaveP2GZData();
	bool commandLoadP2GZData();

	// _00-_E8 = MemoryCardMgr
	u32 mErrorCode;         // _D8
	void* mBannerImageFile; // _DC
	void* mIconImageFile;   // _E0
	BitFlag<u32> mFlags;    // _E4
};

struct MgrCommandCopyPlayer : public MemoryCardMgrCommandBase {
	MgrCommandCopyPlayer(int flags, int fileIndex1, int fileIndex2)
	    : MemoryCardMgrCommandBase(flags)
	    , mFileIndex1(fileIndex1)
	    , mFileIndex2(fileIndex2)
	{
	}

	virtual u32 getClassSize() { return sizeof(MgrCommandCopyPlayer); } // _08 (weak)

	// _04     = VTBL
	// _00-_08 = MemoryCardMgrCommandBase
	u16 mFileIndex1; // _08
	u16 mFileIndex2; // _0A
};

struct MgrCommandPlayerNo : public MemoryCardMgrCommandBase {
	MgrCommandPlayerNo(int val, int fileIndex)
	    : MemoryCardMgrCommandBase(val)
	    , mFileIndex(fileIndex)
	{
	}

	virtual u32 getClassSize() { return sizeof(MgrCommandPlayerNo); } // _08 (weak)

	// _04     = VTBL
	// _00-_08 = MemoryCardMgrCommandBase
	int mFileIndex; // _08
};

struct MgrCommandGetPlayerHeader : public MemoryCardMgrCommandBase {
	MgrCommandGetPlayerHeader(int val, PlayerFileInfo* info)
	    : MemoryCardMgrCommandBase(val)
	    , mPlayerInfo(info)
	{
	}

	virtual u32 getClassSize() { return sizeof(MgrCommandGetPlayerHeader); } // _08 (weak)

	// _04     = VTBL
	// _00-_08 = MemoryCardMgrCommandBase
	PlayerFileInfo* mPlayerInfo; // _08
};

extern char* cFileName;
// @P2GZ: filename of our dedicated mod-data file, separate from the vanilla save
extern char* cP2GZFileName;

} // namespace MemoryCard
} // namespace Game

#endif
