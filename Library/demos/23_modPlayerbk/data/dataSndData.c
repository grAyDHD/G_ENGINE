#include "../includes/23_modPlayer.h"

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
extern const u8 dMod0Pattern52[];
extern const u8 dMod0Pattern53[];
extern const u8 dMod0Pattern54[];
extern const u8 dMod0Pattern55[];
extern const u8 dMod0Pattern56[];
extern const u8 dMod0Pattern57[];
extern const u8 dMod0Pattern58[];
extern const u8 dMod0Pattern59[];
extern const u8 dMod0Pattern60[];
extern const u8 dMod0Pattern61[];
extern const u8 dMod0Pattern62[];
extern const u8 dMod0Pattern63[];
extern const u8 dMod0Pattern64[];
extern const u8 dMod0Pattern65[];
extern const u8 dMod0Pattern66[];
extern const u8 dMod0Pattern67[];

extern const u8 dMod1Pattern0[];

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

const SAMPLE_HEADER dMod0SmpTable[] = {
    {10594, 0, 64, 0, 0, dSmpData0},    {3307, 0, 30, 0, 3307, dSmpData1},
    {12412, 0, 64, 0, 0, dSmpData2},    {1708, 0, 58, 0, 0, dSmpData3},
    {3500, 0, 30, 0, 3500, dSmpData4},  {2000, 0, 40, 0, 0, dSmpData5},
    {7718, 0, 50, 0, 0, dSmpData6},     {1781, 0, 17, 0, 0, dSmpData7},
    {4724, 0, 40, 0, 0, dSmpData8},     {6828, 0, 64, 0, 0, dSmpData9},
    {4850, 0, 45, 0, 4850, dSmpData10}, {2600, 0, 20, 0, 0, dSmpData11},
    {3025, 0, 64, 0, 0, dSmpData12},    {4000, 0, 64, 0, 0, dSmpData13},
    {4950, 0, 64, 0, 0, dSmpData14},
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
    dMod0Pattern52, dMod0Pattern53, dMod0Pattern54, dMod0Pattern55,
    dMod0Pattern56, dMod0Pattern57, dMod0Pattern58, dMod0Pattern59,
    dMod0Pattern60, dMod0Pattern61, dMod0Pattern62, dMod0Pattern63,
    dMod0Pattern64, dMod0Pattern65, dMod0Pattern66, dMod0Pattern67,
};

const u8 dMod0OrderTable[] = {
    54, 61, 48, 51, 53, 52, 55, 63, 64, 66, 67, 65, 58, 59, 60,
};

const SAMPLE_HEADER dMod1SmpTable[] = {
    {32, 0, 64, 0, 32, dSmpData15},
};

const u8 *dMod1PatternTable[] = {
    dMod1Pattern0,
};

const u8 dMod1OrderTable[] = {
    0,
};

const MOD_HEADER dModTable[] = {
    {dMod0SmpTable, dMod0OrderTable, dMod0PatternTable, 15, 68},
    {dMod1SmpTable, dMod1OrderTable, dMod1PatternTable, 1, 1},
};

const SAMPLE_HEADER dSfxTable[] = {
    {0, 0, 0, 0, 0, 0} // Placeholder, no SFX specified
};
