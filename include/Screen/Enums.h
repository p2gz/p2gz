#ifndef _SCREEN_ENUMS_H
#define _SCREEN_ENUMS_H

#include "types.h"

enum SceneType {
	SCENE_DUMMY  = 0,
	SCENE_GROUND = 10000,
	SCENE_CAVE,
	SCENE_VS,
	SCENE_FLOOR,
	SCENE_CONTENA_BLUE,
	SCENE_CONTENA_RED,
	SCENE_CONTENA_YELLOW,
	SCENE_CONTENA_WHITE,
	SCENE_CONTENA_PURPLE,
	SCENE_UFO_MENU,
	SCENE_PAUSE_MENU,
	SCENE_PAUSE_MENU_DOUKUTU,
	SCENE_PAUSE_MENU_ITEMS,
	SCENE_PAUSE_MENU_MAP,
	SCENE_COURSE_NAME,
	SCENE_CAVE_IN_MENU,
	SCENE_KANKETU_MENU,
	SCENE_CAVE_MORE_MENU,
	SCENE_KANTEI_DEMO,
	SCENE_SPECIAL_ITEM,
	SCENE_CHALLENGE_2P,
	SCENE_SAVE,
	SCENE_FINAL_MSG,
	SCENE_CHALLENGE_1P,
	SCENE_PAUSE_MENU_VS,
	SCENE_WORLD_MAP_INFO_WINDOW_0,
	SCENE_WORLD_MAP_INFO_WINDOW_1,
	SCENE_PAUSE_MENU_CONTROLS,
	SCENE_UNKNOWN272C, // probably unused?
	SCENE_COUNT_DOWN,
	SCENE_HURRY_UP,
	SCENE_GAME_OVER_GENERAL,
	SCENE_ORIMA_DOWN,
	SCENE_LUJI_DOWN,
	SCENE_PRESIDENT_DOWN,
	SCENE_PIKMIN_DOWN,
	SCENE_ZUKAN_ENEMY,
	SCENE_ZUKAN_ITEM,
	SCENE_HIGH_SCORE,
	SCENE_CHALLENGE_SELECT,
	SCENE_CHALLENGE_RESULT,
	SCENE_VS_SELECT,
	SCENE_CAVE_RESULT = 20000,
	SCENE_READY_GO,
	SCENE_FINAL_FLOOR,
	SCENE_DAY_END_RESULT_ITEM,
	SCENE_DAY_END_RESULT_INC_P,
	SCENE_DAY_END_RESULT_MAIL,
	SCENE_FINAL_RESULT,
	SCENE_PAY_DEPT,
	SCENE_WIN_LOSE,
	SCENE_WIN_LOSE_REASON,
	SCENE_DAY_END_RESULT_TITL,
	SCENE_P2GZ_SQUAD,
	SCENE_P2GZ_WARP
};

// Responsible developer.
enum ScreenOwnerID { OWNER_KH = 'KH', OWNER_OGA = 'OGA', OWNER_MRMR = 'MRMR' };

// 8-byte screen ID
#pragma enumsalwaysint off
#pragma longlong_enums on
enum ScreenMemberID {
	// MEMBER_FLOOR = (u64)0xFFFFFFFFFFFFFFFE,
	// MEMBER_DUMMY = 0xFFFFFFFFFFFFFFFF
	_MEMBER_EMPTY_STRING            = 0x0,
	MEMBER_VS                       = 'VS',
	MEMBER_CAVE                     = 'CAVE',
	MEMBER_HIGH_SCORE               = 'HISC',
	MEMBER_SAVE                     = 'SAVE',
	MEMBER_P2GZ_WARP                = 'WARP', // @P2GZ
	MEMBER_ZUKAN_ITEM               = 'DITEM',
	MEMBER_DUMMY                    = 'DUMMY',
	MEMBER_FLOOR                    = 'FLOOR',
	MEMBER_P2GZ_SQUAD               = 'SQUAD', // @P2GZ
	MEMBER_CHALLENGE_1P             = 'CHAL1P',
	MEMBER_CHALLENGE_2P             = 'CHAL2P',
	MEMBER_COURSE_NAME              = 'COURSE',
	MEMBER_CAVE_RESULT              = 'C_RSLT',
	MEMBER_ZUKAN_ENEMY              = 'DENEMY',
	MEMBER_FINAL_RESULT             = 'F_RSLT',
	MEMBER_GROUND                   = 'GROUND',
	MEMBER_KANTEI                   = 'KANTEI',
	MEMBER_START_MENU_ALL           = 'SM_ALL',
	MEMBER_START_MENU_MAP           = 'SM_MAP',
	MEMBER_WIN_LOSE_REASON          = 'WL_RSN',
	MEMBER_CONTENA                  = 'CONTENA',
	MEMBER_DAY_END_RESULT           = 'DE_RSLT',
	MEMBER_FINAL_FLOOR              = 'FIN_FLR',
	MEMBER_GAME_OVER                = 'GOVER2D',
	MEMBER_HURRY_UP                 = 'HURRYUP',
	MEMBER_KANKETU_MENU             = 'KANKETU',
	MEMBER_START_MENU_PAUSE_DOUKUTU = 'PAUSE_D',
	MEMBER_START_MENU_CONTROLS      = 'SM_CONT',
	MEMBER_START_MENU_ITEM          = 'SM_ITEM',
	MEMBER_SPECIAL_ITEM             = 'SP_ITEM',
	MEMBER_UFO_MENU                 = 'UFOMENU',
	MEMBER_UFO_GROUP                = 'UFO_GRP',
	MEMBER_ANA_DEMO                 = 'ANA_DEMO',
	MEMBER_CAVE_MORE                = 'CAVEMORE',
	MEMBER_CHALLENGE_RESULT         = 'CHRESULT',
	MEMBER_CHALLENGE_SELECT         = 'CHSELECT',
	MEMBER_DAY_END_COUNT            = 'DAYENDCD',
	MEMBER_DAY_END_RESULT_INC_P     = 'DER_INCP',
	MEMBER_DAY_END_RESULT_ITEM      = 'DER_ITEM',
	MEMBER_DAY_END_RESULT_MAIL      = 'DER_MAIL',
	MEMBER_DAY_END_RESULT_TITLE     = 'DER_TITL',
	MEMBER_FINAL_MSG                = 'FINALMSG',
	MEMBER_PAY_DEBT                 = 'PAY_DEPT',
	MEMBER_READY_GO                 = 'READY_GO',
	MEMBER_START_MENU_PAUSE         = 'SM_PAUSE',
	MEMBER_START_MENU_PAUSE_VS      = 'SM_PS_VS',
	MEMBER_VS_SELECT                = 'VSSELECT',
	MEMBER_WIN_LOSE                 = 'WIN_LOSE',
	MEMBER_WORLD_MAP_INFO_WINDOW_0  = 'WMapWin0',
	MEMBER_WORLD_MAP_INFO_WINDOW_1  = 'WMapWin1',
	_MEMBER_FORCE_ULONGLONG         = 0xFFFFFFFFFFFFFFF
};
#pragma longlong_enums reset
#pragma enumsalwaysint reset

#endif
