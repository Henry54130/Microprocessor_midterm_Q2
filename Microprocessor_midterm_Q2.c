//wait for copy 

#include <regx52.h>
#include "Pin_Definition_H.h"
#include "delay_x1ms_H.h"

#define PHASE_NO 4

// Function prototypes
void Timer0_Init();
void Timer1_Init();
void LED_speed(unsigned int);
void play_note(unsigned char);

// Variables
code unsigned char led_tab[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned int count = 0;
bit int0_flag = 0, int1_flag = 0;
bit timer_running = 0;  // Timer running flag
bit intx_flag, int0x_flag, int1x_flag;
unsigned int k;   
unsigned char step_count=0;
char i = -1; // LED position index
unsigned char note_index = 0;  //HZ index
bit direction_change = 1;

// Note frequencies (Hz)
unsigned int notes[8] = {262, 294, 330, 349, 392, 440, 494, 523}; // Do, Re, Mi, Fa, So, La, Si, High Do
unsigned int sound_period = 0;  // Timer 1 reload value for sound generation

// **Timer 0 - LED Control**
void Timer0_Init() 
{
    TMOD |= 0x01;   // Set Timer 0 to Mode 1 (16-bit timer)
    TH0 = 0xFC;     // Load initial value for 1ms timing
    TL0 = 0x67;
    TF0 = 0;        // Clear overflow flag
    ET0 = 1;        // Enable Timer 0 interrupt
    EA = 1;         // Enable global interrupts
    TR0 = 0;        // Timer initially OFF
}

// **Timer 1 - Sound Generation**
void Timer1_Init()
{
    TMOD |= 0x10;   // Set Timer 1 to Mode 1 (16-bit timer)
    ET1 = 1;        // Enable Timer 1 interrupt
    TR1 = 0;        // Initially OFF
}

void main() 
{
    Timer0_Init();  // Initialize Timer 0 (LED control)
    Timer1_Init();  // Initialize Timer 1 (Sound control)
    IE |= 0x85;     // Enable external interrupts (EX0, EX1)

    while (1) 
    {
       

        // Start function (check if any interrupt flag is set)
        intx_flag = int0_flag || int1_flag;
        if (intx_flag && timer_running) 
        {
            TR0 = 1;  // Ensure Timer0 is running
            
            // Speed mode
            if (JPB4) 
            {
                LED_speed(DSW);
            }
            // Step mode
            else  
            {
                LED_speed(300);
            }
        }
				else
					TR1=0;
    }
}

// **INT0 Interrupt (Start LED & Move Right)**
void int0_isr() interrupt 0 
{
    if (int1_flag) return;  // Skip execution if INT1 is active

    if (JPB4) 
		{
			int0_flag = ~int0_flag;
			count=0;
			step_count = 0;
			direction_change = ~	direction_change;
		}	
		
    else  
        int0_flag = 1; 

		 

    // Debounce delay
    for ( k = 0; k < 300; k++) 
		{
        SPK = ~SPK;  
        delay_x1ms(1); 
    } 
    TR0 = 1;          // Start Timer0
    timer_running = 1;
}

// **INT1 Interrupt (Start LED & Move Left)**
void int1_isr() interrupt 2	
{
    if (int0_flag) return;  // Skip execution if INT0 is active

    if (JPB4) 
		{
			int1_flag = ~int1_flag; 
			count=0;
			step_count = 0;
			direction_change = ~	direction_change;
			
		}
    else  
        int1_flag = 1;
		
	
// Debounce delay
    for ( k = 0; k < 300; k++) 
		{
        SPK = ~SPK;  
        delay_x1ms(1); 
    } 
    TR0 = 1;          // Start Timer0
    timer_running = 1;
}

// **LED Speed Control Function**
void LED_speed(unsigned int ms)
{
		
		
    unsigned int nCnt;  
    count++;  

   
    if (JPB4) 
    { 
        nCnt = (unsigned int)300 * ms + 10;  
        step_count = 0; 
    }
    else 
    {
        nCnt = 3000;  
    }

    if (count >= nCnt)  
    {  
        count = 0;  
        

        if (!JPB3)  
        {  
            TR1 = 0;  //mute
						SPK=0;
        }  
        else  
        {  
            play_note(note_index);  
            note_index++;  
            if (note_index >= 8) note_index = 0;  
        }  

        int0x_flag = int0_flag && (!int1_flag);  
        int1x_flag = int1_flag && (!int0_flag);  

				

        if (int0x_flag&&(!direction_change))  
        {  
					
					i++;  
            if (i >= PHASE_NO) i = 0; 
				
        }  

        else if (int1x_flag&&(!direction_change))  
        {  
				
					i--;  
            if (i<0) i = PHASE_NO-1;}
				
        }
			
       direction_change=0;
        if (!JPB4) 
        {  
            step_count++;  
            if (step_count >=DSW)  
            {  
                int0_flag = 0;  
                int1_flag = 0;  
                step_count = 0;  
                return;  
            }  
        }  
				LEDS = ~led_tab[i];  
}  


// **Play a Note using Timer 1**
void play_note(unsigned char note)
{
    unsigned int freq = notes[note];  // Get note frequency
    sound_period = 65536 - (500000 / freq);  // Calculate timer reload value for square wave

    TH1 = sound_period >> 8;  // Load high byte
    TL1 = sound_period & 0xFF; // Load low byte

    TR1 = 1;  // Start Timer 1 for sound
}

// **Timer 0 Interrupt - Controls LED Speed**
void Timer0_ISR() interrupt 1 
{
    TH0 = 0xFC;  // Reload for 1ms timing
    TL0 = 0x67;
    TF0 = 0;     // Clear overflow flag

    timer_running = 1;
    
		if (!int0_flag && !int1_flag) {  
        TR0 = 0;  
        return;
    }
	
}

// **Timer 1 Interrupt - Generates Sound Wave**
void Timer1_ISR() interrupt 3
{
    TH1 = sound_period >> 8;  // Reload high byte
    TL1 = sound_period & 0xFF; // Reload low byte

    SPK = ~SPK;  // Toggle speaker output (Square wave)
}


