#include <stdio.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"

void delay (int tm);
void portF();
void portC();
void flash(int n);

int main()
{		
	portF();
	flash(2);
		
}

void delay (int tm){
	int cnt =0;
	while (cnt<tm)
	++cnt;
}

void portF(){
    
    SYSCTL_RCGCGPIO_R = 0x20U; // 0010 0000, activating the sixth bit to Enable clock to GPIO Port F in Run mode,
    GPIO_PORTF_DIR_R = 0x0E; // 0000 1110, PIN directions Set the 2nd, 3rd, 4th bits (Pins 1, 2, 3 of Port F), 
    GPIO_PORTF_DEN_R = 0x0E; // 0000 1110, enable digital function for the pins
}

void portC(){
	SYSCTL_RCGCGPIO_R = 0x08;   
	GPIO_PORTC_DIR_R = 0x0F;    
	GPIO_PORTC_DEN_R = 0x0F;    
}

void flash(int n){
	for(int x = 1; x <=3; x++)
	{
		for(int y = 1; y <=n; y++)
		{
			unsigned char light = 0x10;
			GPIO_PORTF_DATA_R = light>>x; //shifts binary light by x places which activates pins 3,2,1 in that other. 
			delay(1000000);
			GPIO_PORTF_DATA_R = 0x00; //turns off LED
			delay(1000000);
		}
	}	
}
