/*
 * TCS_3200.h
 *
 *  Created on: 24 may. 2026
 *      Author: benja
 */

#ifndef TCS_3200_H_
#define TCS_3200_H_
#include <driverlib.h>



/*Definicion de los pines que se usan para el sensor TCS3200*/
#define TCS_PORT    GPIO_PORT_P5

#define TCS_PIN_S0  GPIO_PIN0
#define TCS_PIN_S1  GPIO_PIN1
#define TCS_PIN_S2  GPIO_PIN2
#define TCS_PIN_S3  GPIO_PIN3
//Pin de salida de frecuencia
//Es importante que coincida con el modo captura de timer_A1
#define TCS_PORT_OUT GPIO_PORT_P8
#define TCS_PIN_OUT GPIO_PIN3

#define TCS_OEN_PORT  GPIO_PORT_P1
#define TCS_OEN_PIN  GPIO_PIN5

// Enumeracion para mejor legibilidad
typedef enum{
    ROJO = 0,
    VERDE = 1,
    AZUL = 2,
    TOTAL = 3
}COLOR_T;

//Prototipo Funciones

void TCS3200_init(void);

void TCS3200_Select_Color(COLOR_T color);

void TCS3200_init_Capture(void);

void TCS3200_Read_period (void);

COLOR_T TCS3200_Read_Color (void);

void TimerA1_init(void);

#endif /* TCS_3200_H_ */
