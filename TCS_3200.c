/*
 * TCS_3200.c
 *
 *  Created on: 24 may. 2026
 *      Author: benja
 */
#include <driverlib.h>
#include "TCS_3200.h"

volatile uint32_t Pulsos = 0;

void TCS3200_init(void) {
    /* Se configuran los Pines S0, S1, S2, S3 como salidas */
    GPIO_setAsOutputPin(TCS_PORT, TCS_PIN_S0 | TCS_PIN_S1 | TCS_PIN_S2 | TCS_PIN_S3);

    /*Se configura S0 y S1 para una frecuencia 20% de escala*/
    /* S0 = ALTO, S1 = BAJO*/
    GPIO_setOutputHighOnPin(TCS_PORT, TCS_PIN_S0);
    GPIO_setOutputLowOnPin(TCS_PORT, TCS_PIN_S1);

    /*Se configura PIN OUT como entrada con Interrupcion */
    GPIO_setAsInputPin(TCS_PORT, TCS_PIN_OUT);
    GPIO_selectInterruptEdge(TCS_PORT, TCS_PIN_OUT, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(TCS_PORT, TCS_PIN_OUT);
    GPIO_enableInterrupt(TCS_PORT, TCS_PIN_OUT);
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


uint32_t TCS3200_Read_Frequency (void){
// En esta funcion se debe leer los pulsos que vienen del sensor y retornar
// Aun no se como se hace
}

COLOR_T TCS3200_Read_Color (void){
// Esta funcion debera Configurar el color, Leer la frecuencia para ese color (Repetir esto para cada color)
// Con los valores de frecuencia para cada color, determinar el color que se esta leyendo y retornar ese color
    uint32_t Freq_Red = 0, Freq_Blue = 0, Freq_Green = 0;


}
