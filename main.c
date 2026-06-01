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

    while(1){

    }
    return 0;

}


