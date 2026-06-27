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
volatile uint16_t periodos[3] = {0,0,0};

volatile uint32_t Pulsos = 0;

void TCS3200_init(void) {
    /* Se configuran los Pines S0, S1, S2, S3 como salidas */
    GPIO_setAsOutputPin(TCS_PORT, TCS_PIN_S0 | TCS_PIN_S1 | TCS_PIN_S2 | TCS_PIN_S3);

    /*Se configura S0 y S1 para una frecuencia 20% de escala*/
    /* S0 = ALTO, S1 = BAJO*/
    GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S0);
    GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S1);

    //Avisa que el P8.3 se va a usar para la captura del timer
    GPIO_setAsPeripheralModuleFunctionInputPin(TCS_PORT_OUT, TCS_PIN_OUT, GPIO_PRIMARY_MODULE_FUNCTION);

    //Configura pin para modificar señal output enable del sensor
    GPIO_setAsOutputPin(TCS_OEN_PORT, TCS_OEN_PIN);

    TimerA1_init();
    TCS3200_init_Capture();

}

void TCS3200_Select_Color(COLOR_T color){
    //Logica para S2 y S3
    // ROJO S2 = Low, S3 = Low
    // AZUL S2 = Low, S3 = High
    // VERDE S2 = High, S3 = High
    // TOTAL S2 = High, S3 = Low Se asume que este nunca deberia de entrar

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
    case TOTAL:
        GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S2);
        GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S3);
        break;
    default:
        GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S2);
        GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S3);

    }
}

void TCS3200_init_Capture(void){

    //Configura modo captura de Timer A1
    Timer_A_initCaptureModeParam captureParam = {0};
    captureParam.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    captureParam.captureMode = TIMER_A_CAPTUREMODE_RISING_EDGE;
    captureParam.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    captureParam.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
    captureParam.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    captureParam.captureOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;

    Timer_A_initCaptureMode(TIMER_A1_BASE, &captureParam);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

void TCS3200_Read_period (void){

    uint8_t i = 0;

    for(i = 0;i < 3;i++){

            Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);//Habilita interrupción de captura
            GPIO_setOutputLowOnPin(TCS_OEN_PORT, TCS_OEN_PIN); //Activa salida de sensor de color
            TCS3200_Select_Color((COLOR_T)i); //Elige color a medir

            while(dato_listo == 0); //Espera hasta que se haya calculado el periodo(lectura de dos flancos)
            periodos[i] = periodo_onda; // Guarda el periodo del color elegido

            dato_listo = 0; //Resetea flag para leer un nuevo periodo
     }
}

COLOR_T TCS3200_Read_Color (void){

    uint16_t Menor_Periodo = 0;
    COLOR_T color = (COLOR_T)0; //Casteo porque saltaba warning
    uint8_t k = 1;

    TCS3200_Read_period();

    Menor_Periodo = periodos[0]; //Asigna el periodo del color rojo como el menor por default

    for(k = 1;k < 3;k++){

        if(periodos[k] < Menor_Periodo){ //Busca el periodo más chico y asigna el color correspondiente.
            Menor_Periodo = periodos[k];  //EN NINGÚN MOMENTO ACTUALIZABA EL MENOR
            color = (COLOR_T)k;

        }
    }
    return color;
}

void TimerA1_init(void){

    //Configura el timer A1 en modo continuo
       Timer_A_initContinuousModeParam c = {0};
       c.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
       c.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
       c.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE; //Desactiva interrupcion por desborde
       c.timerClear = TIMER_A_DO_CLEAR;
       c.startTimer = false;

       Timer_A_initContinuousMode(TIMER_A1_BASE, &c);
       Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void ISR_TimerA1_Captura(void){

    static uint8_t estado_lectura = 0;
    static uint16_t captura_anterior = 0; //Esta va static porque tiene que guardar el valor para comparar con captura_actual
    uint16_t captura_actual = 0;

    switch(__even_in_range(TA1IV, TA1IV_TAIFG)) {

        case TA1IV_TACCR2:
            if(estado_lectura == 0){
                //Lee el número exacto que el hardware guardó en la foto
                captura_anterior = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
                estado_lectura = 1;

            }else if( estado_lectura == 1){

                captura_actual = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
                // Resta para saber cuántos ticks pasaron desde el último pulso
                periodo_onda = captura_actual - captura_anterior;

                //Resetea variables para la lectura del próximo periodo
                estado_lectura = 0;
                captura_actual = 0;
                captura_anterior = 0;

                //Desactiva salida de sensor de color
                GPIO_setOutputHighOnPin(TCS_OEN_PORT, TCS_OEN_PIN);
                //Deshabilita interrupcion de captura
                Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);

                // Levanta la bandera para avisar que hay un nuevo período medido
                dato_listo = 1;
            }

            break;

        default:
            break;
    }
}
