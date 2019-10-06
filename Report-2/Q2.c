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
    
    SYSCTL_RCGCGPIO_R = 0x20U;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x0E;
}

void portC(){
	SYSCTL_RCGCGPIO_R = 0x08;   
	GPIO_PORTC_DIR_R = 0x0F;    
	GPIO_PORTC_DEN_R = 0x0E;    
}

void flash(int n){
	for(int x = 1; x <=3; x++)
	{
		for(int y = 1; y <=n; y++)
		{
			unsigned char light = 0x10;
			GPIO_PORTF_DATA_R = light>>x;
			delay(1000000);
			GPIO_PORTF_DATA_R = 0x00;
			delay(1000000);
		}
	}	
}
