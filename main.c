#include <driverlib.h>
#include "hal_LCD.h"
#include "LCD_proyecto.h"
#include "Mod_HC_05.h"


int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();
    __enable_interrupt();
    Init_HC05();

    CS_initFLLSettle(1000, 30);
    CS_initClockSignal(CS_MCLK,  CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);
        CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);
        CS_initClockSignal(CS_ACLK,  CS_REFOCLK_SELECT,   CS_CLOCK_DIVIDER_1);



    // Esperar a que HC-05 inicie
       __delay_cycles(1000000);  // 1 segundo

       while(1){
           // Enviar un carácter de prueba
           send_String_UART("ROJO\r\n");
           send_String_UART("AZUL\r\n");
           send_String_UART("VERDE\r\n");

           // Esperar 1 segundo
           __delay_cycles(1000000);
           send_String_UART("MENSAJE EXTREMADAMENTE LARGO PARA MOLESTAR\r\n");
       }


    return 0;

}


