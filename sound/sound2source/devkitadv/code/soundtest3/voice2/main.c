/*
---------------------------------------------
Gameboy Advance Sound Channel 2 example
Coded by Uze (Alec Bourque)
	
Last Modified: 03-Jan-2002
---------------------------------------------
*/

#include <gba.h>
#include "stuff.h"
extern const u16 _binary_logo_raw_start[];
#include "logopal.h"

void InterruptProcess(void) __attribute__ ((section(".iwram"))); //the interrupt handle called by crt0.s

u16 note;
u16 delta,u,sweepshifts=2,sweepdir=1,sweeptime=7,cur=9;
u16 envinit=0xf, envdir=0, envsteptime=7,waveduty=0,soundlength=0;
u16 loopmode=0,sfreq=0x400,resamp=1;

void InterruptProcess(void){
	//clear the interrupt(s)
	REG_IF |= REG_IF;
}


void ShowValues(void){
	PrintShort(20,9,soundlength);
	PrintShort(20,10,waveduty);
	switch(waveduty){
	case 0:
		Print(20,10,"0000 (12.5%)");	
		break;
	case 1:
		Print(20,10,"0001 (25%)  ");	
		break;
	case 2:
		Print(20,10,"0002 (50%)  ");	
		break;
	case 3:
		Print(20,10,"0003 (75%)  ");	
		break;
	}
	PrintShort(20,11,envsteptime);
	if(envdir){Print(20,12,"0001 (Inc)");}else{Print(20,12,"0000 (Dec)");};
	PrintShort(20,13,envinit);
	PrintShort(20,14,sfreq);	
	if(loopmode){Print(20,15,"Timed ");}else{Print(20,15,"Loop  ");};

	//PrintShort(31,2,(envinit<<12)+(envdir<<11)+(envsteptime<<8)+(waveduty<<6)+soundlength);
	//PrintShort(31,3,(loopmode<<14)+sfreq);
	
	PrintShort(31,2,REG_SOUND2CNT_L);
	PrintShort(31,3,REG_SOUND2CNT_H);


}


int AgbMain(void){    
	int wait,f;


	init();//set video mode and pallette for font
	LoadPic((u16*)_binary_logo_raw_start,(u8*)logopal);
	for(wait=90;wait;wait--)WaitForVblank(); //wait a bit
	init();//clear the screen 

	//turn on sound circuit
	REG_SOUNDCNT_X = 0x80;

	//full volume, enable sound 1 to left and right
	REG_SOUNDCNT_L=0x2277;

	// Overall output ratio - Full
	REG_SOUNDCNT_H = 2;


	Print(0,0,"****** Sound Channel 2 Demo By Uze *****");

	//enable timer 0 interrupt (for note off)
	//REG_IE=INT_TIMER0;
	//REG_IME=1;

	Print(0,2,"  0x04000060 (REG_SOUND2CNT_L) = ");
	Print(0,3,"  0x04000062 (REG_SOUND2CNT_H) = ");


	Print(0,9, "> Sound Lenght    :");
	Print(0,10,"  Wave Duty Cycle :");
	Print(0,11,"  Envlp Step Time :");
	Print(0,12,"  Envlp Step Dir  :");
	Print(0,13,"  Envlp Init Vol  :");

	Print(0,14,"  Frequency       :");
	Print(0,15,"  Loop Mode       :");
	

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
				REG_SOUND2CNT_L=(envinit<<12)+(envdir<<11)+(envsteptime<<8)+(waveduty<<6)+soundlength;
				REG_SOUND2CNT_H=SOUND1INIT+(loopmode<<14)+sfreq;
			}
			//move cursor
			if(~REG_KEYPAD&(KEYPAD_DOWN|KEYPAD_UP)) 
			{ 
				Print(0,cur," ");
				if(~REG_KEYPAD&KEYPAD_DOWN && cur<15)cur++;
				if(~REG_KEYPAD&KEYPAD_UP && cur>9)cur--;
				Print(0,cur,">");
			}
			//adjust values
			if(~REG_KEYPAD&(KEYPAD_LEFT|KEYPAD_RIGHT)) 
			{
				switch(cur){				
					case 9: //sound len
						if(~REG_KEYPAD&KEYPAD_LEFT && soundlength>0)soundlength--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && soundlength<63)soundlength++;						
						break;
					case 10: //wave duty
						if(~REG_KEYPAD&KEYPAD_LEFT && waveduty>0)waveduty--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && waveduty<3)waveduty++;						
						REG_SOUND2CNT_L=(REG_SOUND2CNT_L&0xff3f)+(waveduty<<6);
						break;
					case 11: //envellope step time
						if(~REG_KEYPAD&KEYPAD_LEFT && envsteptime>0)envsteptime--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && envsteptime<7)envsteptime++;
						REG_SOUND2CNT_L=(REG_SOUND2CNT_L&0xf8ff)+(envsteptime<<8);
						break;
					case 12: //envellope direction
						envdir^=1; //flip bit
						REG_SOUND2CNT_L=(REG_SOUND2CNT_L&0xf7ff)+(envdir<<11);
						break;
					case 13: //envellope init volume
						if(~REG_KEYPAD&KEYPAD_LEFT && envinit>0)envinit--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && envinit<15)envinit++;						
						REG_SOUND2CNT_L=(REG_SOUND2CNT_L&0x0fff)+(envinit<<12);
						break;
					case 14: //frequency
						if(~REG_KEYPAD&KEYPAD_LEFT && sfreq>0)sfreq-=16;
						if(~REG_KEYPAD&KEYPAD_RIGHT && sfreq<2047)sfreq+=16;
						REG_SOUND2CNT_H=(REG_SOUND2CNT_H&0xf800)+sfreq;
						break;
					case 15: //loop mode
						loopmode^=1;						
						break;


				}
				
				ShowValues();
		
			}
		}

	}
					
}

