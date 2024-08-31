/*
---------------------------------------------
Gameboy Advance Sound Channel 4 example
By Alec Bourque (Uze)
	
Last Modified: 10-Jan-2002
---------------------------------------------
*/

#include <gba.h>
#include "stuff.h"
extern const u16 _binary_logo_raw_start[];
#include "logopal.h"


u16 delta,u,cur=10;
u16 envinit=0xf, envdir=0, envsteptime=7,soundlength=0;
u16 loopmode=0;
u16 freq_ratio=6,stepping=1,shift_freq=3;

void ShowValues(void){
	PrintShort(20,10,soundlength);

	PrintShort(20,11,envsteptime);
	if(envdir){Print(20,12,"0001 (Inc)");}else{Print(20,12,"0000 (Dec)");};
	PrintShort(20,13,envinit);
	

	PrintShort(20,14,freq_ratio);
	if(stepping){Print(20,15,"0001 (7 steps) ");}else{Print(20,15,"0000 (15 steps)");};	
	PrintShort(20,16,shift_freq);
	
	if(loopmode){Print(20,17,"Timed ");}else{Print(20,17,"Loop  ");};

	PrintShort(31,2,(envinit<<12)+(envdir<<11)+(envsteptime<<8)+soundlength);
	PrintShort(31,3,(loopmode<<14)+(shift_freq<<4)+(stepping<<3)+freq_ratio);
	
	PrintShort(36,2,REG_SOUND4CNT_L);
	PrintShort(36,3,REG_SOUND4CNT_H);


}


int AgbMain(void){    
	int wait,f;

	init();//set video mode and pallette for font
	LoadPic((u16*)_binary_logo_raw_start,(u8*)logopal);
	for(wait=90;wait;wait--)WaitForVblank(); //wait a bit
	init();//clear the screen 

	//turn on sound circuit
	REG_SOUNDCNT_X = 0x80;

	//full volume, enable sound 4 to left and right
	REG_SOUNDCNT_L=0x8877;

	// Overall output ratio - Full
	REG_SOUNDCNT_H = 2;


	Print(0,0,"****** Sound Channel 4 Demo By Uze *****");

	//enable timer 0 interrupt (for note off)
	//REG_IE=INT_TIMER0;
	//REG_IME=1;

	Print(0,2,"  0x04000060 (REG_SOUND4CNT_L) = ");
	Print(0,3,"  0x04000062 (REG_SOUND4CNT_H) = ");


	Print(0,10,"> Sound Lenght    :");
	Print(0,11,"  Envlp Step Time :");
	Print(0,12,"  Envlp Step Dir  :");
	Print(0,13,"  Envlp Init Vol  :");

	Print(0,14,"  Frequency Ratio :");
	Print(0,15,"  Polynomial Step :");
	Print(0,16,"  Shift Frequency :");
	
	Print(0,17,"  Loop Mode       :");
	

	Print(3,19,"More at http://www.belogic.com/gba");

	ShowValues();

	while(1)
	{
		//check keys - u is cleared when all keys had been released
		u=(REG_KEYPAD!=0x3ff);
		if(u){wait++;}else{f=0;} //wait for fast repeat
		if((~REG_KEYPAD&0x3ff && !f) || (wait>5000)){
			if(!f){wait=-20000;}else{wait=0;};
			f=1;
			if(~REG_KEYPAD&BUTTON_A) //reset & play the sound
			{ 
				REG_SOUND4CNT_L=(envinit<<12)+(envdir<<11)+(envsteptime<<8)+soundlength;
				REG_SOUND4CNT_H=SOUND4INIT+(loopmode<<14)+(shift_freq<<4)+(stepping<<3)+freq_ratio;
				ShowValues();
			}
			//move cursor
			if(~REG_KEYPAD&(KEYPAD_DOWN|KEYPAD_UP)) 
			{ 
				Print(0,cur," ");
				if(~REG_KEYPAD&KEYPAD_DOWN && cur<17)cur++;
				if(~REG_KEYPAD&KEYPAD_UP && cur>10)cur--;
				Print(0,cur,">");
			}
			//adjust values
			if(~REG_KEYPAD&(KEYPAD_LEFT|KEYPAD_RIGHT)) 
			{
				switch(cur){				
					case 10: //sound lenght
						if(~REG_KEYPAD&KEYPAD_LEFT && soundlength>0)soundlength--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && soundlength<255)soundlength++;						
						break;
					case 11: //envelope step time
						if(~REG_KEYPAD&KEYPAD_LEFT && envsteptime>0)envsteptime--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && envsteptime<7)envsteptime++;
						REG_SOUND4CNT_L=(REG_SOUND4CNT_L&0xf8ff)+(envsteptime<<8);
						break;
					case 12: //envelope direction
						envdir^=1; //flip bit
						REG_SOUND4CNT_L=(REG_SOUND4CNT_L&0xf7ff)+(envdir<<11);
						break;
					case 13: //envelope init volume
						if(~REG_KEYPAD&KEYPAD_LEFT && envinit>0)envinit--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && envinit<15)envinit++;						
						REG_SOUND4CNT_L=(REG_SOUND4CNT_L&0x0fff)+(envinit<<12);
						break;
					case 14: //frequency ratio
						if(~REG_KEYPAD&KEYPAD_LEFT && freq_ratio>0)freq_ratio--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && freq_ratio<7)freq_ratio++;
						REG_SOUND4CNT_H=(REG_SOUND4CNT_H&0xff80)+freq_ratio;
						break;
					case 15: //stepping
						stepping^=1; //flip bit
						REG_SOUND4CNT_H=(REG_SOUND4CNT_H&0xfff7)+(stepping<<3);
						break;							
					case 16: //shift frequency
						if(~REG_KEYPAD&KEYPAD_LEFT && shift_freq>0)shift_freq--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && shift_freq<15)shift_freq++;
						REG_SOUND4CNT_H=(REG_SOUND4CNT_H&0xff0f)+(shift_freq<<4);
						break;										
					case 17: //loop mode
						loopmode^=1;						
						break;


				}
				
				ShowValues();
		
			}
		}

	}
					
}

