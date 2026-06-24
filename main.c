#include <driverlib.h>
#include "Motor.h"
#include "hal_LCD.h"
#include "TCS_3200.h"
#include "LCD_proyecto.h"
#include "Mod_HC_05.h"
#include "Cinta.h"



//extern volatile uint16_t lectura_pot;
//extern volatile uint8_t ADC_flag;
extern volatile uint8_t flag_1_seg;
extern volatile uint8_t Cnt_Rojo, Cnt_Verde, Cnt_Azul, Cnt_Total;
extern volatile uint8_t flag_Caja;

int main(void)
{

    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();
    __enable_interrupt();

    /*Inits*/
    Init_HC05();
    Init_Cinta();
    Init_LCD();
    Motores_init();
    TCS3200_init();
    /*Fin INITS*/

    while(1){
        if (flag_Caja == 1){
            flag_Caja = 0;
            flag_1_seg = 0; /*Queremos que se muestre el total 1 segundo despues de que se mostro la ultima lectura*/
            Cinta_Recibe_Caja();
        }

        if (flag_1_seg == 1){
            flag_1_seg = 0;
            Cnt_Total = Cnt_Azul + Cnt_Rojo + Cnt_Verde;
            show_Color_Amount(Cnt_Total, TOTAL);
        }

        ADC_function();


    }
    return 0;
}






