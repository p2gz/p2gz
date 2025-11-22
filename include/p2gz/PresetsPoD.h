#ifndef _GZ_PRESETSPOD_H
#define _GZ_PRESETSPOD_H

// NB: don't include this header with the PresetsAT header (name overlaps)
// PoD Cutscene flags
#define NUM_DAY_2_DEMO_FLAGS 3
#define DAY_2_DEMO_FLAGS     DEMO_Day_One_Start, DEMO_Meet_Red_Pikmin, DEMO_Find_Spiderwort_Mold

#define NUM_DAY_3_DEMO_FLAGS NUM_DAY_2_DEMO_FLAGS + 4
#define DAY_3_DEMO_FLAGS \
	DAY_2_DEMO_FLAGS, DEMO_Pluck_First_Pikmin, DEMO_First_Number_Pellet, DEMO_Unlock_Captain_Switch, DEMO_First_Use_Louie

#define NUM_DAY_4_DEMO_FLAGS NUM_DAY_3_DEMO_FLAGS + 2
#define DAY_4_DEMO_FLAGS     DAY_3_DEMO_FLAGS, DEMO_Louie_Finds_Red_Onion, DEMO_Reds_In_Onion

#define NUM_EC1_DEMO_FLAGS NUM_DAY_4_DEMO_FLAGS + 5
#define EC1_DEMO_FLAGS \
	DAY_4_DEMO_FLAGS, DEMO_Discover_Treasure, DEMO_First_Gate_Down, DEMO_First_Nectar_Use, DEMO_Reunite_Captains, DEMO_You_Appear_Lost

#define NUM_EC2_DEMO_FLAGS NUM_EC1_DEMO_FLAGS + 2
#define EC2_DEMO_FLAGS     EC1_DEMO_FLAGS, DEMO_Find_Cave_Deeper_Hole, DEMO_First_Cave_Enter

#define NUM_AW_DEMO_FLAGS NUM_EC2_DEMO_FLAGS + 5
#define AW_DEMO_FLAGS \
	EC2_DEMO_FLAGS, DEMO_Find_Cave_Geyser, DEMO_First_Cave_Return, DEMO_First_Globe_Day_End, DEMO_Purple_Candypop, DEMO_First_Corpse_In_Cave

#define NUM_HOB_DEMO_FLAGS NUM_AW_DEMO_FLAGS + 4
#define HOB_DEMO_FLAGS \
	AW_DEMO_FLAGS, DEMO_Enter_Awakening_Wood, DEMO_Purples_In_Ship, DEMO_Reds_Purples_Tutorial, DEMO_Pikmin_In_Danger_Fire

#define NUM_WFG_DEMO_FLAGS NUM_HOB_DEMO_FLAGS + 3
#define WFG_DEMO_FLAGS     HOB_DEMO_FLAGS, DEMO_White_Candypop, DEMO_Whites_Digging, DEMO_Eat_White_Pikmin

#define NUM_SH_DEMO_FLAGS NUM_WFG_DEMO_FLAGS + 3
#define SH_DEMO_FLAGS     WFG_DEMO_FLAGS, DEMO_Find_Blue_Onion, DEMO_Pikmin_In_Danger_Poison, DEMO_Pikmin_In_Danger_Water

#define NUM_VOR2_DEMO_FLAGS NUM_SH_DEMO_FLAGS + 2
#define VOR2_DEMO_FLAGS     SH_DEMO_FLAGS, DEMO_Whites_In_Ship, DEMO_Max_Pikmin_On_Field

#define NUM_COS_DEMO_FLAGS NUM_VOR2_DEMO_FLAGS + 2
#define COS_DEMO_FLAGS     VOR2_DEMO_FLAGS, DEMO_Discover_Bulbmin, DEMO_Enter_Perplexing_Pool

#define NUM_GK_DEMO_FLAGS NUM_COS_DEMO_FLAGS + 1
#define GK_DEMO_FLAGS     COS_DEMO_FLAGS, DEMO_Find_Yellow_Onion

// PoD Exploration Kit cutscene flags
#define NUM_AW_EK_FLAGS 1
#define AW_EK_FLAGS     OlimarData::ODII_SphericalAtlas

#define NUM_WFG_EK_FLAGS NUM_AW_EK_FLAGS + 1
#define WFG_EK_FLAGS     AW_EK_FLAGS, OlimarData::ODII_PrototypeDetector

#define NUM_SH_EK_FLAGS NUM_WFG_EK_FLAGS + 1
#define SH_EK_FLAGS     WFG_EK_FLAGS, OlimarData::ODII_FiveManNapsack

#define NUM_ENTER_BK_EK_FLAGS NUM_SH_EK_FLAGS + 1
#define ENTER_BK_EK_FLAGS     SH_EK_FLAGS, OlimarData::ODII_JusticeAlloy

#define NUM_BK_EK_FLAGS NUM_ENTER_BK_EK_FLAGS + 1
#define BK_EK_FLAGS     ENTER_BK_EK_FLAGS, OlimarData::ODII_GeographicProjection

#define NUM_POST_BK_EK_FLAGS NUM_BK_EK_FLAGS + 1
#define POST_BK_EK_FLAGS     BK_EK_FLAGS, OlimarData::ODII_ForgedCourage

#define NUM_POST_FC_EK_FLAGS NUM_POST_BK_EK_FLAGS + 1
#define POST_FC_EK_FLAGS     POST_BK_EK_FLAGS, OlimarData::ODII_BruteKnuckles

#define NUM_GK_EK_FLAGS NUM_POST_FC_EK_FLAGS + 1
#define GK_EK_FLAGS     POST_FC_EK_FLAGS, OlimarData::ODII_TheKey

// PoD Cave discovery cutscene flags
#define NUM_AW_CAVE_FLAGS 1
#define AW_CAVE_FLAGS     CAVE_EC

#define NUM_HOB_CAVE_FLAGS NUM_AW_CAVE_FLAGS + 1
#define HOB_CAVE_FLAGS     AW_CAVE_FLAGS, CAVE_HoB

#define NUM_WFG_CAVE_FLAGS NUM_HOB_CAVE_FLAGS + 1
#define WFG_CAVE_FLAGS     HOB_CAVE_FLAGS, CAVE_WFG

#define NUM_SH_CAVE_FLAGS NUM_WFG_CAVE_FLAGS + 1
#define SH_CAVE_FLAGS     WFG_CAVE_FLAGS, CAVE_SH

#define NUM_BK_CAVE_FLAGS NUM_SH_CAVE_FLAGS + 1
#define BK_CAVE_FLAGS     SH_CAVE_FLAGS, CAVE_BK

#define NUM_SCX_CAVE_FLAGS NUM_BK_CAVE_FLAGS + 1
#define SCX_CAVE_FLAGS     BK_CAVE_FLAGS, CAVE_SCx

#define NUM_FC_CAVE_FLAGS NUM_SCX_CAVE_FLAGS + 1
#define FC_CAVE_FLAGS     SCX_CAVE_FLAGS, CAVE_FC

#define NUM_COS_CAVE_FLAGS NUM_FC_CAVE_FLAGS + 1
#define COS_CAVE_FLAGS     FC_CAVE_FLAGS, CAVE_CoS

#define NUM_GK_CAVE_FLAGS NUM_COS_CAVE_FLAGS + 1
#define GK_CAVE_FLAGS     COS_CAVE_FLAGS, CAVE_GK

#endif
