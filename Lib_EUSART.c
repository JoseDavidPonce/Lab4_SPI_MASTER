#include <stdint.h>
#include <xc.h>
#include "EUSARTheader.h"

void EUSART_Init (uint8_t a, uint8_t b){    //Función inicialización SART
    //Configuracion para recepcion
    BAUDCTLbits.BRG16 = 0;      //Configurar los baudios en 9600
    TXSTAbits.BRGH = 1;         
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    SPBRG = 25;         
    if (a==1){              //Si se desean interrupciones, acivar los enable
        PIE1bits.RCIE = 1;  //correspondientes y apagar la bandera
        PIR1bits.RCIF = 0;
        INTCONbits.GIE = 1;
        INTCONbits.PEIE = 1;
    }
    RCSTAbits.RX9D = 0; //No activar los 9 bits porque nunca lo uso
    RCSTAbits.CREN = 1;
    
    //Configuracion para envio
    TXSTAbits.TXEN = 1;     //Activar el envío de datos 
    if(b==1){               //Si son necesarias las interrupciones, activar
        PIE1bits.TXIE = 1;  //los enable necesarios
        PIR1bits.TXIF = 0;
        INTCONbits.GIE = 1;
        INTCONbits.PEIE = 1;
        
    }
    
}

uint8_t CHECK_FOR_ERRORS (void){    //Si hay algún error, regresar un código 
    if (RCSTAbits.FERR == 1){       //correspondiente para saber el error que hubo
        return 1;
    }else if (RCSTAbits.OERR == 1){
        return 2;
    }else{
        return 0;
    }    
    
}

void SEND_STRING(char *a){      //Para enviar un string, se envía un char uno por uno
    int i;                      //con un ciclo for
    for(i=0; a[i]!='\0'; i++){
        SEND_CHAR(a[i]);
    }
}

void SEND_CHAR (char a){        //Para enviar un char, se escribe al buffer TXREG
    TXREG = a;              //mientras este no esté ocupado
    while(PIR1bits.TXIF == 0){
    }    
}