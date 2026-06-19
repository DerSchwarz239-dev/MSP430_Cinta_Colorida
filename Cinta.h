/*
 * Cinta.h
 *
 *  Created on: 19 jun. 2026
 *      Author: benja
 */

#ifndef CINTA_H_
#define CINTA_H_

#include "Mod_HC_05.h"
#include "LCD_proyecto.h"

static volatile uint8_t Cnt_Rojo = 0, Cnt_Verde = 0, Cnt_Azul = 0, Cnt_Total = 0;

#define BTN_PORT    GPIO_PORT_P2
#define BTN_PIN     GPIO_PIN6



void Init_Cinta (void);

void Cinta_Recibe_Caja (COLOR_T Color);





#endif /* CINTA_H_ */
