#include <driverlib.h>
#include "Motor.h"


extern volatile uint16_t lectura_pot;
extern volatile uint16_t ADC_flag;



int main(void)
{
    //uint16_t ancho_pulso = 1500;
    //uint16_t velocidad = 10000;


    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();
    void Motores_init();
    __enable_interrupt();


    while(1){

        //Falta lectura sensor para ancho de pulso


        // --- POSICIÓN 1: 0° (Pulso de 1ms -> 1000 ticks) ---
                Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 1000);
                // Esperamos 1 segundo (asumiendo que el reloj del CPU corre a 1 MHz por defecto)
                __delay_cycles(1000000);

                // --- POSICIÓN 2: 90° (Pulso de 1.5ms -> 1500 ticks) ---
                Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 1500);
                __delay_cycles(1000000);

                // --- POSICIÓN 3: 180° (Pulso de 2ms -> 2000 ticks) ---
                Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 2000);
                __delay_cycles(1000000);


        /*
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        if(ADC_flag){
            ADC_flag = 0;
            velocidad = (((uint32_t)lectura_pot*20000)/1023);
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, velocidad);

        }*/

    }
    return 0;

}


