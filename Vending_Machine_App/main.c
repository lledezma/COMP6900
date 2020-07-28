					//Vending machine application via ARM Cortex-M4 Launchpad
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include "tm4c123gh6pm.h"

unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
unsigned char UART_InChar(void);
void UART_Init(void);
void printString (char * string);

void delayUs(int n);
void keypad_init(void);
unsigned char keypad_getkey(void);
unsigned char key; 

int main(void)
{
	keypad_init();
	char values[10][2][15]={
		"A1", "Pepsi",
		"A2", "Coca-cola",
		"A3", "Doritos",
		"B4", "juice",
		"B5", "Chocolate",
		"C6", "candy",
		"C7", "chips",
		"D8", "gum",
		"D9", "cookies",
		"D1", "gummies",
	};
	char *prices[] = {"3.25", "1.00", "4.00", "3.00", "9.00", "2.50", "4.00", "5.00", "6.00", "2.00"};
	UART_Init();
	char myChar;
	while (1)
	{
		for(int i = 0; i< 1000; i++);
		printString("\n \r");
		for(int h = 0; h <10; h++){
			printString(values[h][0]);
			printString(": ");
			printString(values[h][1]);
			printString("----$");
			printString(prices[h]);
			printString("\n \r");
		}
		bool message = true;
		while(message)
		{
			char selection[] = "";
			float tamount = 0;
			printString("\n \r");
			printString("Choose an item from the choices above: ");
			for(int x =0; x <2;x++){
				myChar = UART_InChar(); //read character from PC keyboard input – wait for user input
				UART_OutChar(myChar);// print on PuTTy console to see what the user typed
				selection[x] = myChar;
				delayUs(1000000);
				// Mapping between UART input and GPIO output
			}
			printString ("\n \r");
			int i = 0;
			char result[50];
			while(i<10)
			{
				if(strcmp(selection,values[i][0]) == 0){
					printString("You total is: ");
					printString(prices[i]);
					char arr[strlen(prices[i]) + 1];  // Make sure there's enough space
					strcpy(arr, prices[i]);
					printString("\n \r");
					printString("Insert money....");
					printString("\n \r");
					bool enough = true;
					while(enough)
					{
						char payment[] = "";
							for(int j =0; j <4;j++){
									if(j == 1){
										payment[j] = '.';
										UART_OutChar(payment[j]);
									}
									else{
										myChar = UART_InChar();
										payment[j] = myChar;
										UART_OutChar(payment[j]);
										delayUs(1000000);
									}
							}
							float tprice, tpayment, finalvalue;
							tprice = atof (arr);			//stores prices[x]
							tpayment = atof (payment);	//stores payment
							tamount = tamount + tpayment;
							if(tamount < tprice){
									char answer [] = "";
									finalvalue = (tamount - tprice) *-1;
									sprintf(result, "%.2f", finalvalue);
									printString("\n \r");
									printString("Amount left: ");
									printString(result);
									printString("\n \r");
									printString("Do you want to choose another item? ");
									for(int h = 0; h <1; h++) {
												myChar = UART_InChar();
											answer[h] = myChar;
											UART_OutChar(answer[h]);
											delayUs(1000000);
									}
									if(strcmp ("Y",answer) == 0) {
										printString("\n \r");
										enough = false;
										message = false;
										break;
									}
                                    else{
										printString("\n \r");
										printString("Insert money... \n \r");
                                    }
							}
							if(tamount >= tprice){
								finalvalue = (tamount - tprice) ;
								sprintf(result, "%.2f", finalvalue);
								enough = false; 
							}		
					}
					if(message != true){
						break;
					}
					printString ("\n \r");
					printString("You have chosen ");
					printString(values[i][1]);
					printString ("\n \r");
					delayUs(1000000);
					printString("Please wait while get your item.... \n \r");
					delayUs(4000000);
					printString("Your change is: $");
					printString(result);
					printString("\n \r");
					printString("Thank you! \n \r");
					printString("\n \r");
					delayUs(3000000);
					message = false;
					break;
				}
				i++;
				if(i == 10){
					printString(selection);
					printString(" is not an option.");
					printString("\n \r");
				}
			}
		}		
	}
}

void printString (char * string){
	while (*string) // until the string is NULL
	{
		UART_OutChar(*(string++));
		// Read characters from sequentially memory addresses one by one and push them out to PC via UART
	}
}
unsigned char UART_InChar(void) 
{
	while((UART0_FR_R&0x0010) != 0){
		key = keypad_getkey(); //read keypad
		if (key != 0)
		{
			return((key));
		}
	}
	return((unsigned char)(UART0_DR_R&0xFF));
}// Wait for selectionfer to be not full, then output
void UART_OutChar(unsigned char data) 
{
	while((UART0_FR_R&0x0020) != 0);
	UART0_DR_R = data;
}
// Assumes a 50 MHz bus clock, creates 115200 baud rate
void UART_Init(void)
{ // should be called only once
		SYSCTL_RCGCUART_R |= 0x0001; // activate UART0
		SYSCTL_RCGCGPIO_R |= 0x0001; // activate port A
		UART0_CTL_R &= ~0x0001; // disable UART
		UART0_IBRD_R = 104; // IBRD=int(50000000/(16*115,200)) = int(27.1267)
		UART0_FBRD_R = 11; // FBRD = round(0.1267 * 64) = 8
		UART0_LCRH_R = 0x0070; // 8-bit word length, enable FIFO
		UART0_CTL_R = 0x0301; // enable RXE, TXE and UART
		GPIO_PORTA_PCTL_R =(GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011; // UART
		GPIO_PORTA_AMSEL_R &= ~0x03; // disable analog function on PA1-0
		GPIO_PORTA_AFSEL_R |= 0x03; // enable alt funct on PA1-0
		GPIO_PORTA_DEN_R |= 0x03; // enable digital I/O on PA1-0

}
void keypad_init(void){
			 SYSCTL_RCGCGPIO_R |= 0x04; // activate clock for port C
			 SYSCTL_RCGCGPIO_R |= 0x08; // activate clock for port E
	  //ROWS
			 GPIO_PORTD_DIR_R |= 0x0F; //set row pins 3-0 as output 
			 GPIO_PORTD_DEN_R |= 0x0F; //set row pins 3-0 as digital pins 
			 GPIO_PORTD_ODR_R |= 0x0F; //set row pins 3-0 as open drain 
	//COLUMS
			 GPIO_PORTC_DIR_R &= ~0xF0; //set row pins 7-4as input
			 GPIO_PORTC_DEN_R |= 0xF0;  
			 GPIO_PORTC_PUR_R |= 0xF0; 
}
unsigned char keypad_getkey(void)
{
		const unsigned char keymap[4][4] = {
				{ '1', '2', '3', 'A'},
				{ '4', '5', '6', 'B'},
				{ '7', '8', '9', 'C'},
				{ 'Y', '0', 'N', 'D'},
		};
 
int row, col;
// check to see any key pressed 
GPIO_PORTD_DATA_R = 0;
col = GPIO_PORTC_DATA_R & 0xF0;
if (col == 0xF0) 
	return 0; //no press
 
while (1)
{
	row = 0;
	GPIO_PORTD_DATA_R = 0x0E;
	delayUs(2); 
	col = GPIO_PORTC_DATA_R & 0xF0;
	if (col != 0xF0) break;
	row = 1;
	GPIO_PORTD_DATA_R = 0x0D;
	delayUs(2); 
	col = GPIO_PORTC_DATA_R & 0xF0;
	if (col != 0xF0) break;
	row = 2;
	GPIO_PORTD_DATA_R = 0x0B;
	delayUs(2); 
	col = GPIO_PORTC_DATA_R & 0xF0;
	if (col != 0xF0) break;
	row = 3;
	GPIO_PORTD_DATA_R = 0x07;
	delayUs(2); 
	col = GPIO_PORTC_DATA_R & 0xF0;
	if (col != 0xF0) 
		break;
	return 0; // no key was pressed
}
if (col == 0xE0) return keymap[row][0]; // key in column 0 
if (col == 0xD0) return keymap[row][1]; // key in column 1 
if (col == 0xB0) return keymap[row][2]; // key in column 2 
if (col == 0x70) return keymap[row][3]; // key in column 3 
return 0; 
}
/* delay n microseconds */
void delayUs(int n)
{
	int i, j;
	for(i = 0 ; i < n; i++)
	for(j = 0; j < 3; j++)
	{} 
}
