
#include "tm4c123gh6pm.h"

void PortA_Init (void);
uint32_t PortA_Input(void);
void PortA_Output(uint32_t Input);

void PortA_Output3(uint32_t Input);
void PortA_Output4(uint32_t Input);


void delay (int tm); // Delay function


void PortE_Init (void); // For Port initialization
uint32_t PortE_Input(void); // For acquiring the input bit
void PortE_Output(uint32_t Input); // For passing input bit to output LED





int main(){
		PortA_Init ();
		uint32_t data;

		PortE_Init();
        	uint32_t data2;

		 while (1){
			data = PortA_Input();
        		// PortA_Output (data);					//Q2
            		PortA_Output3 (data);					//Q3-a

            		data2 = PortE_Input();					//Q4-A
            		PortA_Output4 (data2);					//Q4-A

		}
 return 0;
}

void PortA_Init (void)
{

		 SYSCTL_RCGCGPIO_R |= 0x01; // activate clock for port A

		 GPIO_PORTA_DIR_R &= ~0x20; // An external switch will be used as input to PA5 pin (0010 0000 = 0x20),

		GPIO_PORTA_DIR_R |= 0x1C;  //  Set PE2, PE3, PE4 as outputs , (0001 1100 ),

		GPIO_PORTA_DEN_R |= 0x3C;//

}

void PortE_Init(void)
{
        SYSCTL_RCGCGPIO_R |= 0x10;  //activate clock for port E

        GPIO_PORTE_DIR_R &= ~0x20; // An external switch will be used as input to PE5 pin (0010 0000 = 0x20),

        GPIO_PORTE_DIR_R |= 0x1C; //Set PE2, PE3, PE4 as outputs , (0001 1100 ),

        GPIO_PORTE_DEN_R |= 0x3C; // enable digital function at pins 5, 4,3,2 (0011 1100 = 0x3C)
}


uint32_t PortA_Input(void){
		uint32_t data = GPIO_PORTA_DATA_R & 0x20;
		 return (data);
}
uint32_t PortE_Input(void){
		uint32_t data2 = GPIO_PORTE_DATA_R & 0x20;
		 return (data2);
}



void PortA_Output3(uint32_t Input){
	if(Input)
	{
	    While(1)
	     {

		GPIO_PORTA_DATA_R = 0x10; //NORTH-RED
                GPIO_PORTE_DATA_R = 0x04; // EAST-GREEN
                delay(10000000);

		GPIO_PORTA_DATA_R = 0x10 ;	//NORTH-Y
		GPIO_PORTE_DATA_R = 0x08; 	//East-G
                delay(10000000);

                GPIO_PORTA_DATA_R = 0x04;	//NORTH-G
                GPIO_PORTE_DATA_R = 0x10; 	//EAST-R
		delay(10000000);

                GPIO_PORTA_DATA_R = 0x08;	//NORTH-Y
                GPIO_PORTE_DATA_R = 0x10; 	//EAST-R
                delay(10000000);

                GPIO_PORTA_DATA_R = 0x10;	//NORTH-R
                GPIO_PORTE_DATA_R = 0x04; 	//EAST-G
                delay(10000000);

                GPIO_PORTA_DATA_R = 0x00;	//NORTH-R
                GPIO_PORTE_DATA_R = 0x00; 	//EAST-G
      }
   }
}



void PortA_Output4(uint32_t Input)
{
	if(Input)
	{
		while(1)
		{
			GPIO_PORTA_DATA_R = 0x08; //NORTH YELLOW
			GPIO_PORTE_DATA_R = 0x10; //EAST RED
            		delay(1000000);
            		GPIO_PORTA_DATA_R = 0x00; //NORTH YELLOW
			GPIO_PORTE_DATA_R = 0x00; //EAST RED
            		delay(1000000);
		}
  	}
}


void delay (int tm){
	int cnt =0;
	while (cnt<tm)
	++cnt;
}

void PortA_Output(uint32_t Input){

    GPIO_PORTA_DATA_R = Input>>3; // right shift the input 0010 0000 (switch input location) by 3 bits resulting in 0000 0100 	GREEN
    GPIO_PORTA_DATA_R = Input>>2; // right shift the input 0010 0000 (switch input location) by 2 bits resulting in 0000 1000   YELLOW
    GPIO_PORTA_DATA_R = Input>>1; // right shift the input 0010 0000 (switch input location) by 1 bits resulting in 0001 0000		RED

    GPIO_PORTE_DATA_R = Input>>3; // right shift the input 0010 0000 (switch input location) by 3 bits resulting in 0000 0100  	GREEN
    GPIO_PORTE_DATA_R = Input>>2; // right shift the input 0010 0000 (switch input location) by 2 bits resulting in 0000 1000		YELLOW
    GPIO_PORTE_DATA_R = Input>>1; // right shift the input 0010 0000 (switch input location) by 1 bits resulting in 0001 0000		RED

}
