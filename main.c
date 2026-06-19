#include <driverlib.h>
#include "hal_LCD.h"
#include "TCS_3200.h"
#include "LCD_proyecto.h"
#include "Mod_HC_05.h"
#include "Cinta.h"



int main(void)
{
    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();
    __enable_interrupt();

    /*Inits*/
   // TCS3200_init();
    Init_HC05();
    Init_Cinta();
    Init_LCD();

    /*Fin Inits*/

    clearLCD();
    while(1){
           Cinta_Recibe_Caja(ROJO);
           __delay_cycles(1000000);
           Cinta_Recibe_Caja(VERDE);
           __delay_cycles(1000000);
           Cinta_Recibe_Caja(TRANSPARENTE);
           __delay_cycles(1000000);
       }
    return 0;

}


