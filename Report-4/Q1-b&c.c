#include <stdio.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"

void delay(int tm);
void portF();
void portC();
void flash(int n);

static int myData;
static int PF1_Data, PF2_Data, PF3_Data;
void MapBit();
void SysTick_Init(void);

void LED_ON(int h);
void LED_OFF(int h);

int main()
{

	int n = 5;
	portF();
	for (int x = 1; x <= 3; x++)
	{
		for (int y = 1; y <= n; y++)
		{
			LED_ON(x);
			LED_OFF(x);
		}
	}
}
void LED_ON(int h)
{
	unsigned char light = 0x10;
	myData = GPIO_PORTF_DATA_R |= light >> h;
	MapBit();
	//delay(1000);

}
void LED_OFF(int h) {
	myData = GPIO_PORTF_DATA_R = 0x00;
	MapBit();
	//delay(1000);
}


void delay(int tm) {
	int cnt = 0;
	while (cnt < tm)
		++cnt;
}

void portF() {

	SYSCTL_RCGCGPIO_R = 0x20U; // 0010 0000, activating the sixth bit to Enable clock to GPIO Port F in Run mode,
	GPIO_PORTF_DIR_R = 0x0E; // 0000 1110, PIN directions Set the 2nd, 3rd, 4th bits (Pins 1, 2, 3 of Port F), 
	GPIO_PORTF_DEN_R = 0x0E; // 0000 1110, enable digital function for the pins
}

void flash(int n) {
	for (int x = 1; x <= 3; x++)
	{
		for (int y = 1; y <= n; y++)
		{
			unsigned char light = 0x10;
			myData = GPIO_PORTF_DATA_R |= light >> x; //shifts binary light by x places which activates pins 3,2,1 in that other. 
			MapBit();
			delay(100000);
			myData = GPIO_PORTF_DATA_R |= 0x00; //turns off LED
			MapBit();
			delay(100000);
		}
	}
}

void MapBit(void) {
	PF1_Data = myData & 0x02;
	PF2_Data = myData & 0x04;
	PF3_Data = myData & 0x08;
}

void SysTick_Init(void) {
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0x00FFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;

}