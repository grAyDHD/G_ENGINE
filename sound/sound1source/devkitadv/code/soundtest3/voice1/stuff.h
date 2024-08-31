#include "gba.h"
extern void init(void);
extern void Print(int x, int y, unsigned char *str);
extern void PrintInt(int x,int y,int i);
extern void PrintChar(int x,int y, unsigned char i);
extern void PrintIntRev(int x,int y,int i);
extern void PrintShort(int x, int y,int i);
extern void WaitForVblank(void);
extern void LoadPic(unsigned short *pic, unsigned char *pal);

void WaitForVblank(void)
{
		volatile unsigned short *ScanlineCounter=(unsigned short*)0x4000006;
		while(*ScanlineCounter != 160){ /*do nothing until >160*/}
        while(*ScanlineCounter == 160){ /*do nothing until >160*/}
}

