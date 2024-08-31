/*
---------------------------------------------
Gameboy Advance Sound Channel 3 example
(c)2001 Alec Bourque (Uze)
	
Last Modified: December 9,2001
---------------------------------------------
*/

#include <gba.h> 
#include "notes.h"
#include "stuff.h"
extern const u16 _binary_logo_raw_start[];
#include "logopal.h"

void InterruptProcess(void) __attribute__ ((section(".iwram"))); //the interrupt handle called by crt0.s

u16 note;


void LoadWav(int inst){	
	REG_DMA3SAD=(u32)&voices[inst<<2]; 
	REG_DMA3DAD=(u32)&REG_WAVE_RAM0;
	REG_DMA3CNT_L=4;
	REG_DMA3CNT_H=0x8400; 	
	REG_SOUND3CNT_L^= SOUND3SETBANK1;	//change to play bank , doesnt work on emulators :( ...yet
}
void InterruptProcess(void){
		//note
		//sound 3 is turned on/off sharply		
		//Note for REG_SOUND3CNT_L or REG_SOUND3CNT_H:
		//changing REG_SOUNDCNT_L when sound was turned off by
		//REG_SOUND3CNT_L or REG_SOUND3CNT_H causes clicks.Also when turning off, 				
		//output is set at -127 and slowly raise at 0
		
		REG_SOUNDCNT_L&=0xbbff; 
		REG_TM0CNT_H=0;

		//clear the interrupt(s)
	REG_IF |= REG_IF;
}


void ShowWav(int inst){
	if(REG_SOUND3CNT_L&SOUND3SETBANK1){
		PrintInt(29,8,voices[(inst<<2)]);
		PrintInt(29,9,voices[(inst<<2)+1]);
		PrintInt(29,10,voices[(inst<<2)+2]);
		PrintInt(29,11,voices[(inst<<2)+3]);	
	}else{
		PrintInt(29,3,voices[(inst<<2)]);
		PrintInt(29,4,voices[(inst<<2)+1]);
		PrintInt(29,5,voices[(inst<<2)+2]);
		PrintInt(29,6,voices[(inst<<2)+3]);		
	}
	
	PrintChar(13,3,(char)(REG_SOUND3CNT_L&SOUND3SETBANK1)>>6); //show playing bank
	PrintChar(6,2,(char)inst); 
	
	//show how bytes are really played from memory
	//PrintIntRev(7,6,voices[(inst<<2)]);
	//PrintIntRev(15,6,voices[(inst<<2)+1]); 
	//PrintIntRev(23,6,voices[(inst<<2)+2]);
	//PrintIntRev(31,6,voices[(inst<<2)+3]);	
   

} 
      
int AgbMain(void){    
	u16 wait,u,pan;  
	signed short inst,pattern,length,OffMode,PanMask;
	init();//set video mode and pallette for font
 	LoadPic((u16*)_binary_logo_raw_start,(u8*)logopal);
	for(wait=90;wait;wait--)WaitForVblank(); //wait a bit
	init();//clear the screen 

	pan=0; //used to auto-pan
	PanMask=0x4000;
	pattern=0;
	length=0xeb;
	inst=3;

	//turn on sound circuit
	REG_SOUNDCNT_X = 0x80;

	//full volume, enable sound 3 to left and right
	REG_SOUNDCNT_L=0x4477;

	// Overall output ratio - Full
	REG_SOUNDCNT_H = 2;

	REG_SOUND3CNT_L = SOUND3BANK32 | SOUND3SETBANK1;
	LoadWav(inst); //load in bank0

	Print(0,0,"****** Sound Channel 3 Demo By Uze *****");
	Print(0,2,"Voice:");
	Print(0,3,"Playing Bank:00");
	Print(0,4,"Bank Mode:Single");
	Print(0,5,"Note:");
	Print(0,6,"Pattern:00");
	Print(0,7,"Sound Length:");PrintChar(13,7,(char)length);
	Print(0,8,"Stop Mode:Interrupt");
	

	Print(20,2,"-----Bank 0------");
	Print(20,3,"WAVERAM0:");
	Print(20,4,"WAVERAM1:");
	Print(20,5,"WAVERAM2:");
	Print(20,6,"WAVERAM3:");
	Print(20,7,"-----Bank 1------");
	Print(20,8,"WAVERAM0:");
	Print(20,9,"WAVERAM1:");
	Print(20,10,"WAVERAM2:");
	Print(20,11,"WAVERAM3:");


	Print(0,12,"L/R=Change pattern");
	Print(0,13,"U/D=Change sound length");
	Print(0,14,"Shoulder L/R=Change voice");
	Print(0,15,"A=Swap playing bank");
	Print(0,16,"B=Change bank mode(2x32 or 1x64 samples)");
	Print(0,17,"Select=Toggle stereo auto-pan");
	Print(0,18,"Start=Toggle stop mode");
	Print(0,19,"* More stuff at http://www.Belogic.com *");
	ShowWav(inst);

	REG_SOUND3CNT_H = length;
	
	OffMode= 0; // note off mode (0=interrupt based, 1=lenght based)

	//enable timer 0 interrupt (for note off)
	REG_IE=INT_TIMER0;
	REG_IME=1;
	
	REG_SOUND3CNT_L |= SOUND3PLAY;
	REG_SOUND3CNT_X  |= SOUND3INIT; 
	REG_SOUND3CNT_H = SOUND3OUTPUT1;
		

	while(1){
		for(note=0;note<32;note++){		
			*(unsigned short*)0x05000000=(unsigned short)note; //increment background color (for fun :)			
			
			//get next note	
			if(OffMode){
				//use note length mode		
				//reg value=note lenght(in seconds)*256
				REG_SOUND3CNT_H = SOUND3OUTPUT1|length;				
				REG_SOUND3CNT_X = freq[song[(pattern<<5)+note]]|SOUND3PLAYONCE|SOUND3INIT;
				
			}else{
				//use smooth note off mode
				REG_TM0CNT_H=0;
				REG_SOUND3CNT_X=freq[song[(pattern<<5)+note]];
				REG_TM0CNT_L=0xffff-((255-length)<<6);				
				REG_TM0CNT_H= TMR_ENABLE|TMR_IF_ENABLE|TMR_PRESCALER_1024CK;				
			}

			//auto-pan each note
			if(pan){
				REG_SOUNDCNT_L=(REG_SOUNDCNT_L&0xBBFF)|PanMask;
			}else{
				REG_SOUNDCNT_L|=0x4400;
			}
			PanMask^=0x4400;

			//freqency part of register is write only
			PrintChar(5,5, (char)note);
			for(wait=0;wait<7;wait++){ //wait for next note
				WaitForVblank();						
				
			
				//check keys - u is cleared when all keys had been released
				if(~REG_KEYPAD&0x3ff && !u){
					u=1;
					if(~REG_KEYPAD&(BUTTON_RIGHT|BUTTON_LEFT)) //change chan 3 instrument (wave ram)
					{ 
  
						if(~REG_KEYPAD&BUTTON_RIGHT)inst++; 
						if(~REG_KEYPAD&BUTTON_LEFT)inst--;				
						
						if(inst<0)inst=23;
						if(inst>=24)inst=0;

						if(REG_SOUND3CNT_L&SOUND3BANK64){
							//load sound in single bank mode
							//it uses to normal instrument so it sounds a bit crappy
							//but you get the idea...
							//you have to stop the sound before reloading
							//wave ram when using the single bank mode or
							//no reloading will take place
							//Sound init is required when restarting sound
							REG_SOUND3CNT_L &= ~SOUND3PLAY; //stop sound
							LoadWav(inst);
							LoadWav(inst+2);
							REG_SOUND3CNT_L|= SOUND3PLAY;
							REG_SOUND3CNT_X  |= SOUND3INIT; 
						}else{
							LoadWav(inst);	//load new instrument in dual-bank mode										
						}
						
						ShowWav(inst); //show wave info

					}
					if(~REG_KEYPAD&BUTTON_A){
						//swap play bank.
						REG_SOUND3CNT_L ^= SOUND3SETBANK1; 
						PrintChar(13,3,(char)(REG_SOUND3CNT_L&SOUND3SETBANK1)>>6); //show current bank
					}
					
					if(~REG_KEYPAD&BUTTON_B){
						//switch between single/dual bank mode
						REG_SOUND3CNT_L^=SOUND3BANK64; 
						if(REG_SOUND3CNT_L&SOUND3BANK64){
							Print(10,4,"Dual  ");
						}else{
							Print(10,4,"Single");
						}
					} 
					
					if(~REG_KEYPAD&(KEYPAD_RIGHT|KEYPAD_LEFT)){ 
						//change pattern
						if(~REG_KEYPAD&KEYPAD_RIGHT && pattern<2)pattern++; 
						if(~REG_KEYPAD&KEYPAD_LEFT && pattern>0)pattern--;				
						
						//if(pattern<0) pattern=2;
						//if(pattern>=3) pattern=0;	
						
						PrintChar(8,6,(char)pattern);
					}
									
					if(~REG_KEYPAD&BUTTON_SELECT){
						//toggle auto-panning
						pan^=1; 
						if(pan){
							//REG_SOUNDCNT_L^=PanMask; //0x4000; //turn only l or r
						//	REG_SOUNDCNT_L=(REG_SOUNDCNT_L&0xBBFF)|PanMask;	
						}else{						
							//REG_SOUNDCNT_L|=0x4400; //turn both on
						} 
					}

					if(~REG_KEYPAD&BUTTON_START){												
						//switch stop mode
						OffMode^=1;
						if(OffMode){
							//REG_TM0CNT_H=0;
							Print(10,8,"Counter   ");
						}else{
							//SG30H = SG30HOUTPUT1;							
							REG_SOUND3CNT_L |= SOUND3PLAY;
							REG_SOUND3CNT_X  |= SOUND3INIT; 
							Print(10,8,"Interrupt ");
						}


					}


				}
				
				if(~REG_KEYPAD&(KEYPAD_UP|KEYPAD_DOWN)) //change note length
				{ 
					if(~REG_KEYPAD&KEYPAD_UP && length<255)length++; 
					if(~REG_KEYPAD&KEYPAD_DOWN && length>0)length--;				
					PrintChar(13,7,(u8)length);
				
				}

				u=(REG_KEYPAD!=0x3ff);	//clear the "all-buttons-released" flag		
				
			}

		}
	}; 

  	


}
