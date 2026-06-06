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

    // Esperar a que HC-05 inicie
       __delay_cycles(1000000);  // 1 segundo

       while(1){
           // Enviar un carácter de prueba
           send_Byte_UART('A');

           // Esperar 1 segundo
           __delay_cycles(1000000);
       }


    return 0;

}


