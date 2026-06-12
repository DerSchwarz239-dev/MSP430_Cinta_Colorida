/*
 * TCS_3200.c
 *
 *  Created on: 24 may. 2026
 *      Author: benja
 */
#include <driverlib.h>
#include "TCS_3200.h"

//Variables globales para guardar el periodo
volatile uint16_t periodo_onda = 0;
volatile uint8_t dato_listo = 0;
volatile uint16_t Freq[3] = {0,0,0};

volatile uint32_t Pulsos = 0;

void TCS3200_init(void) {
    /* Se configuran los Pines S0, S1, S2, S3 como salidas */
    GPIO_setAsOutputPin(TCS_PORT, TCS_PIN_S0 | TCS_PIN_S1 | TCS_PIN_S2 | TCS_PIN_S3);

    /*Se configura S0 y S1 para una frecuencia 20% de escala*/
    /* S0 = ALTO, S1 = BAJO*/
    GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S0);
    GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S1);

    /*Se configura PIN OUT como entrada con Interrupcion */
    GPIO_setAsInputPin(TCS_PORT_OUT, TCS_PIN_OUT);
    GPIO_selectInterruptEdge(TCS_PORT_OUT, TCS_PIN_OUT, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(TCS_PORT_OUT, TCS_PIN_OUT);
    GPIO_enableInterrupt(TCS_PORT_OUT, TCS_PIN_OUT);

    GPIO_setAsOutputPin(TCS_OEN_PORT, TCS_OEN_PIN);

    TimerA1_init();
    TCS3200_init_Capture();

}

void TCS3200_Select_Color(COLOR_T color){
    //Logica para S2 y S3
    // ROJO S2 = Low, S3 = Low
    // AZUL S2 = Low, S3 = High
    // VERDE S2 = High, S3 = High
    // TRANSPARENTE S2 = High, S3 = Low

    switch(color){
    case ROJO:
        GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S2 | TCS_PIN_S3);
        break;
    case AZUL:
        GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S2);
        GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S3);
        break;
    case VERDE:
        GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S2 | TCS_PIN_S3);
        //GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S3);
        break;
    case TRANSPARENTE:
        GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S2);
        GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S3);
        break;
    default:
        GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S2);
        GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S3);

    }
}

void TCS3200_init_Capture(void){

    Timer_A_initCaptureModeParam captureParam = {0};
    captureParam.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    captureParam.captureMode = TIMER_A_CAPTUREMODE_RISING_EDGE;
    captureParam.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    captureParam.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
    captureParam.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    captureParam.captureOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;

    Timer_A_initCaptureMode(TIMER_A1_BASE, &captureParam);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

uint32_t TCS3200_Read_Frequency (void){
// En esta funcion se debe leer los pulsos que vienen del sensor y retornar
// Aun no se como se hace

    for(uint8_t i = 0;i < 3;i++){
            TCS3200_Select_Color(i);
            GPIO_setOutputHighOnPin(TCS_OEN_PORT, TCS_OEN_PIN);
            while(dato_listo == 0);//REVISAR


        }
}

void TCS3200_Read_Color (void){
// Esta funcion debera Configurar el color, Leer la frecuencia para ese color (Repetir esto para cada color)
// Con los valores de frecuencia para cada color, determinar el color que se esta leyendo y retornar ese color




}

void TimerA1_init(void){

    //Configura el timer en modo continuo
       Timer_A_initContinuousModeParam c = {0};
       c.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
       c.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
       c.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE; //Desactiva interrupcion por desborde
       c.timerClear = TIMER_A_DO_CLEAR;
       c.startTimer = false;

       Timer_A_initUpMode(TIMER_A1_BASE, &c);
       Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

pragma vector = TIMER1_A1_VECTOR
__interrupt void ISR_TimerA1_Captura(void){

    static uint8_t estado_lectura = 0;
    uint16_t captura_anterior = 0;
    uint16_t captura_actual = 0;

    switch(__even_in_range(TA1IV, TA1IV_TAIFG)) {

        case TA1IV_TACCR1:
            if(estado_lectura == 0){
                //Lee el número exacto que el hardware guardó en la foto
                captura_anterior = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
                estado_lectura = 1;

            }else if( estado_lectura == 1){

                captura_actual = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
                // Resta para saber cuántos ticks pasaron desde el último pulso
                periodo_onda = captura_actual - captura_anterior;
                estado_lectura = 0;

                GPIO_setOutputLowOnPin(TCS_OEN_PORT, TCS_OEN_PIN);
                // Levanta la bandera para avisarle al main() que hay un nuevo período medido
                dato_listo = 1;

            }

            break;

        default:
            break;
    }
}
