/*
 * File:   Main_MASTER.c
 * Author: José Ponce
 * Carné 18187
 * Created on 17 de febrero de 2020, 12:13 PM
 */
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include <stdint.h>
#include "EUSARTheader.h"
#include "SPI_header.h"

#define _XTAL_FREQ 4000000  //Configura una frecuencia para los delays
void Port_Init (void);
uint8_t contador  = 0;
uint8_t pot1 = 0; 
uint8_t pot2 = 0;
uint8_t alternancia = 0;
uint8_t nodatotx = 0;

void __interrupt() isr(void){  
    if (PIR1bits.SSPIF == 1){
        PIR1bits.SSPIF = 0;
        if (alternancia == 0){
            pot1 = SSPBUF;
            SSPBUF = 0x01;
            alternancia = 1;
        }else{
            pot2 = SSPBUF;
            SSPBUF = 0x00;
            alternancia = 0;
        }
    } if (PIR1bits.RCIF == 1){
        contador = RCREG;
    } if (PIR1bits.TXIF == 1){
        if (pot1 == 0x23){
            pot1 = 0x22;
        }if (pot2 == 0x23){
            pot2 = 0x22;
        }
        
        switch (nodatotx) {
            case 0:
                TXREG = pot1;
                nodatotx++;
                break;
            case 1:
                TXREG = pot2;
                nodatotx++;
                break;
            case 2:
                TXREG = 0x23;
                nodatotx = 0;
                break;
        }
    }
    } 

void main(void) {
    Port_Init();
    Init_SPI(1,1);
    Init_SPI_int();
    EUSART_Init(0,1); 
    SSPBUF = 1;
    while(1){
        if (PIR1bits.RCIF == 1){
            PORTBbits.RB6 = 1;
            contador = RCREG;
            PORTB = contador;
        }
    }
}

void Port_Init(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
}