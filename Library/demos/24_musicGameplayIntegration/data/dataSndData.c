#include "audio/modPlayer.h"

extern const u8 dMod0Pattern0[];
extern const u8 dMod0Pattern1[];
extern const u8 dMod0Pattern2[];
extern const u8 dMod0Pattern3[];
extern const u8 dMod0Pattern4[];
extern const u8 dMod0Pattern5[];
extern const u8 dMod0Pattern6[];
extern const u8 dMod0Pattern7[];
extern const u8 dMod0Pattern8[];
extern const u8 dMod0Pattern9[];
extern const u8 dMod0Pattern10[];
extern const u8 dMod0Pattern11[];
extern const u8 dMod0Pattern12[];
extern const u8 dMod0Pattern13[];
extern const u8 dMod0Pattern14[];
extern const u8 dMod0Pattern15[];
extern const u8 dMod0Pattern16[];
extern const u8 dMod0Pattern17[];
extern const u8 dMod0Pattern18[];
extern const u8 dMod0Pattern19[];
extern const u8 dMod0Pattern20[];
extern const u8 dMod0Pattern21[];
extern const u8 dMod0Pattern22[];
extern const u8 dMod0Pattern23[];
extern const u8 dMod0Pattern24[];
extern const u8 dMod0Pattern25[];
extern const u8 dMod0Pattern26[];
extern const u8 dMod0Pattern27[];
extern const u8 dMod0Pattern28[];
extern const u8 dMod0Pattern29[];
extern const u8 dMod0Pattern30[];
extern const u8 dMod0Pattern31[];
extern const u8 dMod0Pattern32[];
extern const u8 dMod0Pattern33[];
extern const u8 dMod0Pattern34[];
extern const u8 dMod0Pattern35[];
extern const u8 dMod0Pattern36[];
extern const u8 dMod0Pattern37[];
extern const u8 dMod0Pattern38[];
extern const u8 dMod0Pattern39[];
extern const u8 dMod0Pattern40[];
extern const u8 dMod0Pattern41[];
extern const u8 dMod0Pattern42[];
extern const u8 dMod0Pattern43[];
extern const u8 dMod0Pattern44[];
extern const u8 dMod0Pattern45[];
extern const u8 dMod0Pattern46[];
extern const u8 dMod0Pattern47[];
extern const u8 dMod0Pattern48[];
extern const u8 dMod0Pattern49[];
extern const u8 dMod0Pattern50[];
extern const u8 dMod0Pattern51[];

extern const u8 dMod1Pattern0[];
extern const u8 dMod1Pattern1[];
extern const u8 dMod1Pattern2[];
extern const u8 dMod1Pattern3[];
extern const u8 dMod1Pattern4[];
extern const u8 dMod1Pattern5[];
extern const u8 dMod1Pattern6[];
extern const u8 dMod1Pattern7[];
extern const u8 dMod1Pattern8[];
extern const u8 dMod1Pattern9[];
extern const u8 dMod1Pattern10[];
extern const u8 dMod1Pattern11[];
extern const u8 dMod1Pattern12[];

extern const s8 dSmpData0[];
extern const s8 dSmpData1[];
extern const s8 dSmpData2[];
extern const s8 dSmpData3[];
extern const s8 dSmpData4[];
extern const s8 dSmpData5[];
extern const s8 dSmpData6[];
extern const s8 dSmpData7[];
extern const s8 dSmpData8[];
extern const s8 dSmpData9[];
extern const s8 dSmpData10[];
extern const s8 dSmpData11[];
extern const s8 dSmpData12[];
extern const s8 dSmpData13[];
extern const s8 dSmpData14[];
extern const s8 dSmpData15[];
extern const s8 dSmpData16[];
extern const s8 dSmpData17[];
extern const s8 dSmpData18[];
extern const s8 dSmpData19[];
extern const s8 dSmpData20[];
extern const s8 dSmpData21[];
extern const s8 dSmpData22[];
extern const s8 dSmpData23[];
extern const s8 dSmpData24[];
extern const s8 dSmpData25[];
extern const s8 dSmpData26[];
extern const s8 dSmpData27[];
extern const s8 dSmpData28[];
extern const s8 dSmpData29[];

const SampleHeader dMod0SmpTable[] = {
    {4777, 0, 64, 1141, 3614, dSmpData0},
    {7502, 0, 64, 0, 7502, dSmpData1},
    {7527, 0, 64, 0, 7527, dSmpData2},
    {1157, 0, 64, 0, 1, dSmpData3},
    {7475, 0, 64, 2795, 4680, dSmpData4},
    {1942, 0, 64, 0, 1, dSmpData5},
    {1095, 0, 64, 156, 939, dSmpData6},
    {3514, 0, 64, 0, 1, dSmpData7},
    {801, 15, 64, 712, 88, dSmpData8},
    {2648, 0, 64, 0, 1, dSmpData9},
    {7437, 0, 64, 2700, 4170, dSmpData10},
    {1063, 0, 64, 0, 1, dSmpData11},
    {7471, 0, 64, 0, 7471, dSmpData12},
    {946, 0, 64, 0, 1, dSmpData13},
    {873, 0, 64, 0, 1, dSmpData14},
    {1527, 0, 64, 1396, 127, dSmpData15},
    {6806, 0, 64, 5538, 1268, dSmpData16},
    {5189, 0, 64, 3449, 1740, dSmpData17},
    {863, 0, 64, 773, 90, dSmpData18},
    {2942, 2, 64, 1599, 1338, dSmpData19},
    {7495, 0, 64, 0, 7495, dSmpData20},
    {2936, 0, 64, 56, 2880, dSmpData21},
};

const u8 *dMod0PatternTable[] = {
    dMod0Pattern0,  dMod0Pattern1,  dMod0Pattern2,  dMod0Pattern3,
    dMod0Pattern4,  dMod0Pattern5,  dMod0Pattern6,  dMod0Pattern7,
    dMod0Pattern8,  dMod0Pattern9,  dMod0Pattern10, dMod0Pattern11,
    dMod0Pattern12, dMod0Pattern13, dMod0Pattern14, dMod0Pattern15,
    dMod0Pattern16, dMod0Pattern17, dMod0Pattern18, dMod0Pattern19,
    dMod0Pattern20, dMod0Pattern21, dMod0Pattern22, dMod0Pattern23,
    dMod0Pattern24, dMod0Pattern25, dMod0Pattern26, dMod0Pattern27,
    dMod0Pattern28, dMod0Pattern29, dMod0Pattern30, dMod0Pattern31,
    dMod0Pattern32, dMod0Pattern33, dMod0Pattern34, dMod0Pattern35,
    dMod0Pattern36, dMod0Pattern37, dMod0Pattern38, dMod0Pattern39,
    dMod0Pattern40, dMod0Pattern41, dMod0Pattern42, dMod0Pattern43,
    dMod0Pattern44, dMod0Pattern45, dMod0Pattern46, dMod0Pattern47,
    dMod0Pattern48, dMod0Pattern49, dMod0Pattern50, dMod0Pattern51,
};

const u8 dMod0OrderTable[] = {
    17, 18, 19, 5,  4,  3,  6,  7,  8,  9,  11, 0,  12, 1,  16, 30, 16,
    21, 20, 13, 14, 13, 15, 24, 25, 26, 27, 28, 31, 32, 29, 33, 34, 35,
    40, 41, 36, 38, 39, 42, 44, 45, 37, 46, 47, 48, 49, 50, 51,
};

const SampleHeader dMod1SmpTable[] = {
    {17, 0, 64, 0, 16, dSmpData22},  {65, 0, 64, 0, 64, dSmpData23},
    {8, 0, 32, 0, 8, dSmpData24},    {8, 0, 16, 0, 8, dSmpData25},
    {8, 0, 64, 0, 8, dSmpData26},    {1123, 0, 48, 0, 0, dSmpData27},
    {1505, 0, 64, 0, 0, dSmpData28}, {1463, 0, 64, 0, 0, dSmpData29},
};

const u8 *dMod1PatternTable[] = {
    dMod1Pattern0,  dMod1Pattern1, dMod1Pattern2,  dMod1Pattern3,
    dMod1Pattern4,  dMod1Pattern5, dMod1Pattern6,  dMod1Pattern7,
    dMod1Pattern8,  dMod1Pattern9, dMod1Pattern10, dMod1Pattern11,
    dMod1Pattern12,
};

const u8 dMod1OrderTable[] = {
    0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

const ModHeader dModTable[] = {
    {dMod0SmpTable, dMod0OrderTable, dMod0PatternTable, 49, 52},
    {dMod1SmpTable, dMod1OrderTable, dMod1PatternTable, 15, 13},
};

const SampleHeader dSfxTable[] = {
    {0, 0, 0, 0, 0, 0} // Placeholder, no SFX specified
};
