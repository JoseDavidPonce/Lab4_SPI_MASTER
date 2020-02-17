/*
 * File:   Lib_SPI.c
 * Author: José Ponce 
 * Decripción: Librería para inicializar y controlar el módulo MSSP SPI
 *
 * Created on 17 de febrero de 2020, 06:15 AM
 * 
 */

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h> 
#include "SPI_header.h"

void Init_SPI (uint8_t a, uint8_t just_one_slave){
    TRISCbits.TRISC5 = 0;

    if (a == 1){            //Configuración de Master
        TRISCbits.TRISC3 = 0;
        SSPSTATbits.SMP = 0;
        SSPSTATbits.CKE = 1;
        SSPCONbits.SSPEN = 1;
        SSPCONbits.CKP = 0;

        SSPCONbits.SSPM0 = 0;
        SSPCONbits.SSPM1 = 0;
        SSPCONbits.SSPM2 = 0;
        SSPCONbits.SSPM3 = 0;
    }else if (a == 0){      //Configuración de esclavo
        TRISCbits.TRISC3 = 1;
        SSPSTATbits.SMP = 0;
        SSPSTATbits.CKE = 1;
        SSPCONbits.SSPEN = 1;
        SSPCONbits.CKP = 0;
        
        SSPCONbits.SSPM1 = 0;
        SSPCONbits.SSPM2 = 1;
        SSPCONbits.SSPM3 = 0;
        if (just_one_slave == 1){
            SSPCONbits.SSPM0 = 1;
        }else if (just_one_slave == 0){
            TRISAbits.TRISA5 = 1;
            SSPCONbits.SSPM0 = 0;
        }
    
    }
}

void Init_SPI_int(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    PIR1bits.SSPIF = 0;     //Bandera que se activará cada vez que 
            //finalice una transmisión o recepción del SPI
}
