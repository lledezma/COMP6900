#include "tm4c123gh6pm.h"
#include <stdint.h>

struct State{
	uint32_t output; 
	uint32_t waittime; 
	const struct State *transsion[4];
};
typedef const struct State current;
//Define three states
#define Idle &FSM[0]
#define Ready &FSM[1]
#define Standby &FSM[2]
current FSM[3]={
	{0x01, 1, {Idle, Idle, Ready, Idle}},
	{0x02, 1, {Ready, Idle, Ready, Standby}},
	{0x04, 1, {Standby, Idle, Standby, Standby}},
};
static uint32_t data;	
void delay(uint32_t waitTime);
void PortE_Init(void);  
void PortB_Init(void);
uint32_t PortE_Input(void);
void SysTick_Init(void);  
int main(void){
	PortB_Init();
	PortE_Init();
	SysTick_Init();	
	current *Pointer;  
	Pointer = Idle;
	while(1){
		data = PortE_Input(); 
		Pointer = Pointer->transsion[data]; 
		GPIO_PORTB_DATA_R = Pointer->output;  
		delay(Pointer->waittime);      
	}
	return 0;
}
void PortB_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02; //activate PortB
	GPIO_PORTB_DIR_R |= 0x07;  // PB0, PB1, PB2 pins will be used for lights
	GPIO_PORTB_DEN_R |= 0x07; 
}
void PortE_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;  //activate PortE
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x1F;
	GPIO_PORTE_DIR_R &= ~0x03; //pins 0 and 1 are output pins
	GPIO_PORTE_DEN_R |= 0x03;  
}
uint32_t PortE_Input(void){
	uint32_t data = GPIO_PORTE_DATA_R & 0x13;
	return(data); 
}
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0; 
	NVIC_ST_RELOAD_R = 0x00FFFFFF;  
	NVIC_ST_CURRENT_R = 0;  
	NVIC_ST_CTRL_R = 0x00000005;  
}
void delay(uint32_t waitTime){
	int cnt =0;
	while(cnt< waitTime*10) {
		NVIC_ST_RELOAD_R = 1000000-1; 
		NVIC_ST_CURRENT_R = 0;       
		while((NVIC_ST_CTRL_R&0x00010000)==0);
		cnt++;
	}
}
