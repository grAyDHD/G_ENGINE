/*
---------------------------------------------
Gameboy Advance Sound Channel 1 example
(c)2002 Alec Bourque (Uze)
	
Last Modified: 03-Jan-2002
---------------------------------------------
*/

#include <gba.h>
#include "stuff.h"
extern const u16 _binary_logo_raw_start[];
#include "logopal.h"
 
void InterruptProcess(void) __attribute__ ((section(".iwram"))); //the interrupt handle called by crt0.s

u16 note;
u16 delta,u,sweepshifts=2,sweepdir=1,sweeptime=7,cur=6;
u16 envinit=0xf, envdir=0, envsteptime=7,waveduty=2,soundlength=1;
u16 loopmode=0,sfreq=0x400,resamp=1;

void InterruptProcess(void){
	//not used in this demo
	//clear the interrupt(s)
	REG_IF |= REG_IF;
}


void ShowValues(void){
	//display the different parameters
	PrintShort(20,6,sweepshifts);
	if(sweepdir){Print(20,7,"0001 (Sub)");}else{Print(20,7,"0000 (Add)");};
	PrintShort(20,8,sweeptime);
	PrintShort(20,9,soundlength);
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
	switch(resamp){
	case 0:
		Print(20,17,"9-bits/32768 Hz ");	
		break;
	case 1:
		Print(20,17,"8-bits/65536 Hz ");	
		break;
	case 2:
		Print(20,17,"7-bits/131072 Hz");	
		break;
	case 3:
		Print(20,17,"6-bits/262144 Hz");	
		break;
	}
	//display what's put in the registers
	PrintShort(33,2,(sweeptime<<4)+(sweepdir<<3)+sweepshifts);
	PrintShort(33,3,(envinit<<12)+(envdir<<11)+(envsteptime<<8)+(waveduty<<6)+soundlength);
	PrintShort(33,4,(loopmode<<14)+sfreq);
	
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
	REG_SOUNDCNT_L=0x1177;

	// Overall output ratio - Full
	REG_SOUNDCNT_H = 2;


	Print(0,0,"****** Sound Channel 1 Demo By Uze *****");

	Print(0,2,"  0x04000060 (REG_SOUND1CNT_L) = ");
	Print(0,3,"  0x04000062 (REG_SOUND1CNT_H) = ");
	Print(0,4,"  0x04000064 (REG_SOUND1CNT_X) = ");

	Print(0,6,"> Sweep Shifts    :");
	Print(0,7,"  Sweep Direction :");
	Print(0,8,"  Sweep Time      :");

	Print(0,9, "  Sound Lenght    :");
	Print(0,10,"  Wave Duty Cycle :");
	Print(0,11,"  Envlp Step Time :");
	Print(0,12,"  Envlp Step Dir  :");
	Print(0,13,"  Envlp Init Vol  :");

	Print(0,14,"  Frequency       :");
	Print(0,15,"  Loop Mode       :");
	
	Print(0,17,"  Resampling Freq :");

	Print(3,19,"More at http://www.belogic.com/gba");
	
	ShowValues(); //display the parameters

	while(1)
	{
		//check keys - u is cleared when all keys had been released
		u=(REG_KEYPAD!=0x3ff);
		if(u){wait++;}else{f=0;} //wait for key auto-repeat
		
		if((~REG_KEYPAD&0x3ff && !f) || (wait>5000)){
			
			if(!f){wait=-20000;}else{wait=0;};
			f=1;//enable key auto-repeat

			if(~REG_KEYPAD&BUTTON_A) //play the sound
			{ 
				REG_SOUND1CNT_L=(sweeptime<<4)+(sweepdir<<3)+sweepshifts;
				REG_SOUND1CNT_H=(envinit<<12)+(envdir<<11)+(envsteptime<<8)+(waveduty<<6)+soundlength;
				REG_SOUND1CNT_X=SOUND1INIT+(loopmode<<14)+sfreq;
			}
			
			//move cursor
			if(~REG_KEYPAD&(KEYPAD_DOWN|KEYPAD_UP)) 
			{ 
				Print(0,cur," ");
				if(~REG_KEYPAD&KEYPAD_DOWN && cur<17)cur++; //move cursor down
				if(~REG_KEYPAD&KEYPAD_UP && cur>6)cur--; //move cursor up
				Print(0,cur,">");
			}
			
			//adjust parameters values
			if(~REG_KEYPAD&(KEYPAD_LEFT|KEYPAD_RIGHT)) 
			{
				switch(cur){
					case 6: //sweep shifts
						if(~REG_KEYPAD&KEYPAD_LEFT && sweepshifts>0)sweepshifts--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && sweepshifts<7)sweepshifts++;
						REG_SOUND1CNT_L=(REG_SOUND1CNT_L&0xfff8)+sweepshifts;
						break;					
					case 7: //sweep dir
						sweepdir^=1; //flip the bit
						REG_SOUND1CNT_L=(REG_SOUND1CNT_L&0xfff7)+(sweepdir<<3);
						break;
					case 8: //sweep time
						if(~REG_KEYPAD&KEYPAD_LEFT && sweeptime>0)sweeptime--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && sweeptime<7)sweeptime++;						
						REG_SOUND1CNT_L=(REG_SOUND1CNT_L&0xff0f)+(sweeptime<<4);
						break;
					case 9: //sound len
						if(~REG_KEYPAD&KEYPAD_LEFT && soundlength>0)soundlength--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && soundlength<63)soundlength++;						
						break;
					case 10: //wave duty
						if(~REG_KEYPAD&KEYPAD_LEFT && waveduty>0)waveduty--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && waveduty<3)waveduty++;						
						REG_SOUND1CNT_H=(REG_SOUND1CNT_H&0xff3f)+(waveduty<<6);
						break;
					case 11: //envellope step time
						if(~REG_KEYPAD&KEYPAD_LEFT && envsteptime>0)envsteptime--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && envsteptime<7)envsteptime++;
						REG_SOUND1CNT_H=(REG_SOUND1CNT_H&0xf8ff)+(envsteptime<<8);
						break;
					case 12: //envellope direction
						envdir^=1; //flip bit
						REG_SOUND1CNT_H=(REG_SOUND1CNT_H&0xf7ff)+(envdir<<11);
						break;
					case 13: //envellope init volume
						if(~REG_KEYPAD&KEYPAD_LEFT && envinit>0)envinit--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && envinit<15)envinit++;						
						REG_SOUND1CNT_H=(REG_SOUND1CNT_H&0x0fff)+(envinit<<12);
						break;
					case 14: //frequency
						if(~REG_KEYPAD&KEYPAD_LEFT && sfreq>0)sfreq-=16;
						if(~REG_KEYPAD&KEYPAD_RIGHT && sfreq<2047)sfreq+=16;
						REG_SOUND1CNT_X=(REG_SOUND1CNT_X&0xf800)+sfreq;
						break;
					case 15: //loop mode
						loopmode^=1;						
						break;
					case 17: //resampling freq
						if(~REG_KEYPAD&KEYPAD_LEFT && resamp>0)resamp--;
						if(~REG_KEYPAD&KEYPAD_RIGHT && resamp<3)resamp++;	
						REG_SOUNDBIAS=(resamp<<14)+(REG_SOUNDBIAS&0x3fff);
						break;
				}
				
				ShowValues(); //display parameter values
		
			}
		}

	}
					
}

