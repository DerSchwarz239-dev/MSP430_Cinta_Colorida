/*
 * LCD_proyecto.h
 *
 *  Created on: 1 jun. 2026
 *      Author: benja
 */

/*
 * En este Archivo .h se definiran las funciones del LCD para mostrar las distintas salidas del sistema.
 * Es importante remarcar que es exclusivamente para la cinta.
 * Se hace asi para no editar el .h propio del sensor.
 */

#ifndef LCD_PROYECTO_H_
#define LCD_PROYECTO_H_

#include <driverlib.h>
#include "hal_LCD.h"
#include "TCS_3200.h"



void show_Color_Amount (uint8_t amount, COLOR_T color);

#endif /* LCD_PROYECTO_H_ */
