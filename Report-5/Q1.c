#include <stdio.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"

unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
unsigned char UART_InChar(void);
void UART_Init(void);

void printString (char * string);


uint32_t PortF_Input(void);
void printMessage(uint32_t data);
uint32_t message;
void delayUs(int n);
int main(void)
{
    
    UART_Init();
    char myChar;
    
    while (1)
    {
                
        for(int i = 0; i< 1000; i++);
        printString("Enter r or g or b: \n \r");
        myChar = UART_InChar(); //read character from PC keyboard input – wait for user input
        UART_OutChar(myChar);// print on PuTTy console to see what the user typed
        printString ("\n \r");// New line on PuTTY console
        // Mapping between UART input and GPIO output
        
        
        switch (myChar)
        {
                case 'r':
                    GPIO_PORTF_DATA_R = 0x02; // Red LED ON
                    break;
                case 'b' :
                    GPIO_PORTF_DATA_R = 0x04; // BLUE LED ON
                    break;
                case 'g' :
                    GPIO_PORTF_DATA_R = 0x08; // GREEN LED ON
                    break;
                default:
                    GPIO_PORTF_DATA_R = 0x00; // ALL LEDs OFF
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
        message = PortF_Input();
        printMessage(message);
        
    }
    // !=0 means RXFE is 1 (empty); keep
    //looping, nothing to read
    // wait until RXFE is 0 (not empty)
    // Not empty: There is something to read
    // from buffer
    return((unsigned char)(UART0_DR_R&0xFF));
}// Wait for buffer to be not full,
    // then output
void UART_OutChar(unsigned char data)
{
    while((UART0_FR_R&0x0020) != 0);
    //!=0 means TXFF =1, full, no space to
    //transmit, keep looping (wait)
    // wait until TXFF is 0, TXFF = 0, not
//    full, write data to register that goes to
    //FIFO for transmission
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
        SYSCTL_RCGCGPIO_R |= 0x20;
        GPIO_PORTF_LOCK_R = 0x4C4F434B;
        GPIO_PORTF_CR_R |= 0x1F;
        GPIO_PORTF_DIR_R = 0x0E;
        GPIO_PORTF_PUR_R = 0x11;
        GPIO_PORTF_DEN_R |= 0x1F;
    
}
uint32_t PortF_Input(void){
        uint32_t data = GPIO_PORTF_DATA_R & 0x01;
        data ^= 0x01;
         return (data);
}

void printMessage(uint32_t data){
    if(data){
        printString("Nashville,TN: \n \r");
        printString ("\n \r");
        printString("Enter r or g or b: \n \r");
        delayUs(100000);
    }
}
void delayUs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
    for(j = 0; j < 3; j++)
    {} 
}

