#include "GlobalData.h"
#include "Game/Piki.h"

using namespace Game;

static const Preset DEFAULT_PRESETS[] = {
    // PoD EC
    Preset().setMsgId('3200_00')
        .setPikmin(Flower, Red,    46)
        .setPikmin(Leaf,   Red,     6),
    // PoD Enter HoB
    Preset().setMsgId('3201_00')
        .setOnionPikmin(Flower, Red,    46)
        .setOnionPikmin(Leaf,   Red,     6)
        .setOnionPikmin(Leaf,   Purple, 10),
    // PoD HoB1
    Preset().setMsgId('3202_00')
        .setPikmin(Flower, Red,    62)
        .setPikmin(Flower, Purple, 10),
    // PoD HoB2-4
    Preset().setMsgId('3203_00')
        .setPikmin(Flower, Red,    52)
        .setPikmin(Flower, Purple, 10)
        .setPikmin(Leaf,   Purple, 10),
    // PoD HoB5-WFG3
    Preset().setMsgId('3204_00')
        .setPikmin(Flower, Red,    52)
        .setPikmin(Flower, Purple, 20),
    // PoD WFG4-Enter SH
    Preset().setMsgId('3205_00')
        .setPikmin(Flower, Red,    37)
        .setPikmin(Flower, Purple, 20)
        .setPikmin(Flower, White,  15),
    // PoD SH1-2
    Preset().setMsgId('3206_00')
        .setPikmin(     Flower, Red,    34)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setOnionPikmin(Leaf,   Blue,   16),
    // PoD SH3-7
    Preset().setMsgId('3207_00')
        .setPikmin(     Flower, Red,    29)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,   5)
        .setOnionPikmin(Leaf,   Blue,   16),
    // PoD SH7 (+5W)
    Preset().setMsgId('3208_00')
        .setPikmin(     Flower, Red,    24)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,  10)
        .setOnionPikmin(Leaf,   Blue,   16),
    // PoD Enter BK
    Preset().setMsgId('3209_00')
        .setPikmin(     Flower, Red,    26)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,   5)
        .setOnionPikmin(Leaf,   Blue,   16)
        .setOnionPikmin(Flower, Red,     3)
        .setOnionPikmin(Flower, Purple,  2),
    // PoD Enter BK (+2B)
    Preset().setMsgId('3210_00')
        .setPikmin(     Flower, Red,    26)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,   5)
        .setOnionPikmin(Leaf,   Blue,   18)
        .setOnionPikmin(Flower, Red,     3)
        .setOnionPikmin(Flower, Purple,  2),
    // PoD Enter BK (+5W)
    Preset().setMsgId('3211_00')
        .setPikmin(     Flower, Red,    21)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,  10)
        .setOnionPikmin(Leaf,   Blue,   16)
        .setOnionPikmin(Flower, Red,     3)
        .setOnionPikmin(Flower, Purple,  2),
    // PoD Enter BK (+5W +2B)
    Preset().setMsgId('3212_00')
        .setPikmin(     Flower, Red,    21)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,  10)
        .setOnionPikmin(Leaf,   Blue,   18)
        .setOnionPikmin(Flower, Red,     3)
        .setOnionPikmin(Flower, Purple,  2),
    // PoD BK1-BK7
    Preset().setMsgId('3213_00')
        .setPikmin(     Flower, Red,    26)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,   5)
        .setPikmin(     Leaf,   Blue,   36)
        .setOnionPikmin(Leaf,   Blue,   24)
        .setOnionPikmin(Flower, Red,     3)
        .setOnionPikmin(Flower, Purple,  2),
    // PoD BK1-BK7 (+5W)
    Preset().setMsgId('3214_00')
        .setPikmin(     Flower, Red,    21)
        .setPikmin(     Flower, Purple, 18)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,  10)
        .setPikmin(     Leaf,   Blue,   36)
        .setOnionPikmin(Leaf,   Blue,   24)
        .setOnionPikmin(Flower, Red,     3)
        .setOnionPikmin(Flower, Purple,  2),
    // PoD Enter SCx (Day 6)
    Preset().setMsgId('3215_00')
        .setOnionPikmin(Leaf,   Blue,   65)
        .setOnionPikmin(Flower, Red,    29)
        .setOnionPikmin(Flower, Purple, 20)
        .setOnionPikmin(Flower, White,  15)
        .setOnionPikmin(Leaf,   White,  10),
    // PoD Enter SCx (Day 7)
    Preset().setMsgId('3216_00')
        .setOnionPikmin(Flower, Blue,   65)
        .setOnionPikmin(Flower, Red,    29)
        .setOnionPikmin(Flower, Purple, 20)
        .setOnionPikmin(Flower, White,  15)
        .setOnionPikmin(Leaf,   White,  10),
    // PoD SCx1-4
    Preset().setMsgId('3217_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,  5)
        .setPikmin(     Flower, Blue,   60)
        .setOnionPikmin(Flower, Red,    29),
    // PoD SCx1-4 (+5W)
    Preset().setMsgId('3218_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  15)
        .setPikmin(     Leaf,   White,  10)
        .setPikmin(     Flower, Blue,   55)
        .setOnionPikmin(Flower, Red,    24)
        .setOnionPikmin(Leaf,   Blue,    5),
    // PoD SCx5-Enter FC
    Preset().setMsgId('3219_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  35)
        .setPikmin(     Flower, Blue,   45)
        .setOnionPikmin(Flower, Red,    29),
    // PoD SCx5-Enter FC (40W)
    Preset().setMsgId('3220_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  40)
        .setPikmin(     Flower, Blue,   40)
        .setOnionPikmin(Flower, Red,    24)
        .setOnionPikmin(Leaf,   Blue,    5),
    // PoD FC1-FC7
    Preset().setMsgId('3221_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  35)
        .setPikmin(     Flower, Blue,   20)
        .setOnionPikmin(Flower, Red,    29)
        .setOnionPikmin(Flower, Blue,   25),
    // PoD FC1-FC7 (40W)
    Preset().setMsgId('3222_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  40)
        .setPikmin(     Flower, Blue,   15)
        .setOnionPikmin(Flower, Red,    24)
        .setOnionPikmin(Flower, Blue,   25)
        .setOnionPikmin(Leaf,   Blue,    5),
    // PoD Enter CoS
    Preset().setMsgId('3223_00')
        .setOnionPikmin(Flower, Blue,   50)
        .setOnionPikmin(Flower, Red,    29)
        .setOnionPikmin(Flower, Purple, 20)
        .setOnionPikmin(Flower, White,  40),
    // PoD CoS1-GK6
    Preset().setMsgId('3224_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  35)
        .setPikmin(     Flower, Blue,   45)
        .setOnionPikmin(Flower, Red,    29)
        .setOnionPikmin(Flower, Blue,    5),
    // PoD CoS1-GK6 (+5W)
    Preset().setMsgId('3225_00')
        .setPikmin(     Flower, Purple, 20)
        .setPikmin(     Flower, White,  40)
        .setPikmin(     Flower, Blue,   40)
        .setOnionPikmin(Flower, Red,    29)
        .setOnionPikmin(Flower, Blue,   10),
    // Testing
    Preset().setMsgId('3299_00')
        .setPikmin(     Flower, Red,     20)
        .setPikmin(     Flower, Yellow,  20)
        .setPikmin(     Flower, Blue,    20)
        .setPikmin(     Flower, Purple,  20)
        .setPikmin(     Flower, White,   20)
        .setOnionPikmin(Flower, Red,    999)
        .setOnionPikmin(Flower, Yellow, 999)
        .setOnionPikmin(Flower, Blue,   999)
        .setOnionPikmin(Flower, Purple, 999)
        .setOnionPikmin(Flower, White,  999)
        .setSprays(99, 99),
};