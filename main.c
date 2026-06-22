#include <driverlib.h>
#include "Motor.h"
#include "hal_LCD.h"
#include "TCS_3200.h"
#include "LCD_proyecto.h"
#include "Mod_HC_05.h"


extern volatile uint16_t lectura_pot;
extern volatile uint8_t ADC_flag;



int main(void)
{
    //uint16_t ancho_pulso = 1500;
    //uint16_t velocidad = 10000;


    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();

    //Activa Servo, motor, ADC e interrupción de botón
    //Motores_init();
    TCS3200_init();

    __enable_interrupt();

    volatile COLOR_T color_aux = AZUL;

    while(1){

        color_aux = TCS3200_Read_Color();
        __delay_cycles(5000000);

        color_aux = TCS3200_Read_Color();
        __delay_cycles(5000000);

        color_aux = TCS3200_Read_Color();
         __delay_cycles(5000000);




        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);//Modifica velocidad según el potenciómetro
        if(ADC_flag){
            ADC_flag = 0;
            velocidad = (((uint32_t)lectura_pot*20000)/1023); //Adapta el valor del ADC a la cantidad de ticks
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, velocidad);
        }

    }

    return 0;

}






