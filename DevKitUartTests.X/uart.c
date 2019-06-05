/*
 * File:   uart.c
 * Author: jonathan.comley
 *
 * Created on 04 June 2019, 09:14
 */


#include "xc.h"
#include "uart.h"

#define U5RX_PORT  PORTDbits.RD6
#define U5TX_PORT  PORTDbits.RD1
#define BAUDRATEREG 103  // [16M/16/9600]-1

// Uart 6 init
void Uart_Enable(UART_ENUM UART_DEMO) {
    
    if(UART6 == UART_DEMO) {
        ANSDbits.ANSD6 = 0; //config for digital
        //ANSDbits.ANSD1 = 0;
        
        TRISDbits.TRISD1 = 0; // TX pin set as output
        TRISDbits.TRISD6 = 1; // RX pin set as input 

        U5MODE = 0x0000; // Clear MODE register 
        U5STA = 0x0000; // Clear STATUS register
        
        U5MODEbits.STSEL = 0; // 1-stop bit
        U5MODEbits.PDSEL = 0; // No parity, 8-data bits
        U5MODEbits.ABAUD = 0; // Auto-Baud disables
        
        U5MODEbits.UARTEN = 1; // Enable the UART
        U5MODEbits.BRGH = 0; // Low Speed - BRG generates 16 clocks per bit period (16x baud clock, Standard mode)

        U5BRG = BAUDRATEREG;
        U5STAbits.UTXEN = 1;
        IPC27bits.U5RXIP = 0x07; // Receive interrupt priority - Highest
        IPC28bits.U5TXIP = 0x05; // Transmit interrupt priority 
//        IEC6bits.U5RXIE = 1; // Receive interrupt enable 
//        IEC7bits.U5TXIE = 1; // Transmit interrupt enable 
//        IFS6bits.U5RXIF = 0; // Receive interrupt flag clear
//        IFS7bits.U5TXIF = 0; // Transmit interrupt flag clear
    }
    
}

//UART transmit function, parameter Ch is the character to send

void UART5PutChar(char Ch) {

    //transmit ONLY if TX buffer is empty
    while (U5STAbits.UTXBF == 1);

    U5TXREG = Ch;
}



//UART receive function, returns the value received.

char UART5GetChar(void) {

    char Temp;

    //while (IFS6bits.U5RXIF == 0);     //wait for buffer to fill up, wait for interrupt

    Temp = U5RXREG;         //get char from register
    
    IFS6bits.U5RXIF = 0;    //reset interrupt

    return Temp;            //return my received byte

}