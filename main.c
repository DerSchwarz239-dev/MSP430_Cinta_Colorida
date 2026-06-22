#include <driverlib.h>
#include "Motor.h"
#include "hal_LCD.h"
#include "TCS_3200.h"
#include "LCD_proyecto.h"
#include "Mod_HC_05.h"
#include "Cinta.h"



extern volatile uint16_t lectura_pot;
extern volatile uint8_t ADC_flag;
extern volatile uint8_t flag_1_seg;
extern volatile uint8_t Cnt_Rojo, Cnt_Verde, Cnt_Azul, Cnt_Total;
extern volatile uint8_t flag_caja;


int main(void)
{

    uint16_t velocidad = 10000;


    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    __enable_interrupt();

    //Inits
   // Init_HC05();
   // Init_Cinta();
    //Init_LCD();
    Motores_init(); //Activa Servo, motor, ADC e interrupción de botón
    //TCS3200_init();
    //Fin INITS

    volatile COLOR_T color_aux = AZUL;

    while(1){

        if(flag_caja){
            flag_caja = 0;
            Cinta_Recibe_Caja();
        }

        if (flag_1_seg == 1){
            flag_1_seg = 0;
            Cnt_Total = Cnt_Rojo + Cnt_Verde + Cnt_Azul;
            show_Color_Amount(Cnt_Total, TRANSPARENTE);
            __delay_cycles(3000000);

        }


        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);//Modifica velocidad según el potenciómetro
        if(ADC_flag){
            ADC_flag = 0;
            velocidad = (((uint32_t)lectura_pot*20000)/1023); //Adapta el valor del ADC a la cantidad de ticks
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, velocidad);
        }

    }

    return 0;

}






