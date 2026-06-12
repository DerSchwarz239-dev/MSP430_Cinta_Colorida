/*
 * LCD_proyecto.c
 *
 *  Created on: 1 jun. 2026
 *      Author: benja
 */


/*
 * En este Archivo .c se configurara las funciones del LCD para mostrar las distintas salidas del sistema
 */

#include <driverlib.h>
#include "LCD_proyecto.h"

void show_Color_Amount (uint8_t amount, COLOR_T color){
    /*
     * Funcion encargada de mostrar color y cantidad en LCD.
     * Si se supera la cantidad de 255, automaticamente resetea contador (compilador solo)
     * Si se introduce como parametro color TOTAL, mostrara en LCD la letra T, lo que permite mostrar como total.
     */
    clearLCD();
    switch (color) {
        case ROJO:
            showChar('R', pos1);
            break;
        case VERDE:
            showChar('V', pos1);
            break;
        case AZUL:
            showChar('A', pos1);
            break;
        default:
            showChar('T', pos1);
    }

    showChar('0' + (amount%1000)/100, pos3);
    showChar('0' + (amount%100)/10, pos4);
    showChar('0' + amount%10, pos5);
}

