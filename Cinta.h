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
#include "Motor.h"


#define INTERRUPCIONES_PARA_1_SEG   1000

#define BTN_PORT    GPIO_PORT_P2
#define BTN_PIN     GPIO_PIN6



void Init_Cinta (void);

void Cinta_Recibe_Caja (void);





#endif /* CINTA_H_ */
