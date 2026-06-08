/*
 * Mod_HC_05.h
 *
 *  Created on: 1 jun. 2026
 *      Author: benja
 */

#ifndef MOD_HC_05_H_
#define MOD_HC_05_H_

#include <driverlib.h>

/* Definicion de PINES que se utilizan en Modulo*/
#define HC_PORT     GPIO_PORT_P1

#define HC_TX_PIN   GPIO_PIN0
#define HC_RX_PIN   GPIO_PIN1
#define HC_EN_PIN   GPIO_PIN3

typedef enum{
    ROJO = 0,
    VERDE = 1,
    AZUL = 2
}COLOR_T;

void Init_HC05 (void);
void send_Byte_UART(uint8_t data);
void send_String_UART (const char* str);
void send_Color_UART (COLOR_T color);


#endif /* MOD_HC_05_H_ */
