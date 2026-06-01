#include <driverlib.h>

int main(void)
{
    //uint16_t anchoPulso = 1500;
    //uint16_t velocidad = 10000;
    uint16_t velocidad = 0;

    WDT_A_hold(WDT_A_BASE);
    PMM_unlockLPM5();
    __enable_interrupt();


    while(1){



        //Falta lectura sensor para ancho de pulso
        //Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, ancho_pulso);
        ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
        if(ADC_flag){
            ADC_flag = 0;
            velocidad = ((uint32_t)lectura_pot*20000)/1023);
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, velocidad);

        }


    }
    return 0;

}


