/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * hal_LCD.h
 *
 * Hardware abstraction layer for the FH-1138P Segmented LCD
 *
 * September 2014
 * E. Chen
 *
 ******************************************************************************/

#include <driverlib.h>

#ifndef HAL_LCD_H_
#define HAL_LCD_H_

#define pos1 4   /* Digit A1 - L4  */
#define pos2 6   /* Digit A2 - L6  */
#define pos3 8   /* Digit A3 - L8  */
#define pos4 10  /* Digit A4 - L10 */
#define pos5 2   /* Digit A5 - L2  */
#define pos6 18  /* Digit A6 - L18 */

// Define word access definitions to LCD memories
#define LCDMEMW ((int*)LCDMEM)

// Workaround LCDBMEM definition bug in IAR header file
#ifdef __IAR_SYSTEMS_ICC__
#define LCDBMEMW ((int*)&LCDM32)
#else
#define LCDBMEMW ((int*)LCDBMEM)
#endif

extern const char digit[10][2];
extern const char alphabetBig[26][2];

/* -----------------------------------------------------------------------
 * Simbolos especiales del LCD FH-1138P (MSP-EXP430FR4133 LaunchPad)
 * Mapa verificado: diagnostico bit a bit + codigo TI out-of-box.
 *
 * Uso:  LCDMEM[12] |= LCD_TMR;   encender
 *        LCDMEM[12] &= ~LCD_TMR;  apagar
 *
 * LCDMEM[12] — iconos principales (bits 0..7):
 * ----------------------------------------------------------------------- */
#define LCD_EXCL      0x01  /* Signo admiracion "!"     — LCDMEM[12] bit 0 */
#define LCD_REC       0x02  /* Grabacion  "REC"          — LCDMEM[12] bit 1 */
#define LCD_HRT       0x04  /* Corazon    "HRT"          — LCDMEM[12] bit 2 */
#define LCD_TMR       0x08  /* Cronometro "TMR"          — LCDMEM[12] bit 3 */
#define LCD_BATT_CAP  0x10  /* Terminal (+) de bateria   — LCDMEM[12] bit 4 */
#define LCD_BATT_B2   0x20  /* Barra bateria B2          — LCDMEM[12] bit 5 */
#define LCD_BATT_B4   0x40  /* Barra bateria B4          — LCDMEM[12] bit 6 */
#define LCD_BATT_B6   0x80  /* Barra bateria B6          — LCDMEM[12] bit 7 */

/* LCDMEM[13] — bateria (bits 0..3; bits 4..7 no conectados):              */
#define LCD_BATT_FRAME 0x01 /* Corchetes [    ]          — LCDMEM[13] bit 0 */
#define LCD_BATT_B1    0x02 /* Barra bateria B1          — LCDMEM[13] bit 1 */
#define LCD_BATT_B3    0x04 /* Barra bateria B3          — LCDMEM[13] bit 2 */
#define LCD_BATT_B5    0x08 /* Barra bateria B5          — LCDMEM[13] bit 3 */

/* Bateria completa:
 *   LCDMEM[12] |= LCD_BATT_CAP|LCD_BATT_B2|LCD_BATT_B4|LCD_BATT_B6;
 *   LCDMEM[13] |= LCD_BATT_FRAME|LCD_BATT_B1|LCD_BATT_B3|LCD_BATT_B5;  */

/* -----------------------------------------------------------------------
 * Puntuacion y signos — verificados en TI TempSensorMode.c / StopWatchMode.c
 *
 * Cada byte adyacente posX+1 tiene en bit2 (0x04) un simbolo especial
 * y en bit0 (0x01) el punto decimal de esa posicion:
 *
 *   LCDMEM[pos1+1] = LCDMEM[5]:  bit2 = NEG "-"   (signo negativo)
 *   LCDMEM[pos2+1] = LCDMEM[7]:  bit2 = ":"        (colon entre pos2-pos3)
 *   LCDMEM[pos4+1] = LCDMEM[11]: bit2 = ":"        (colon entre pos4-pos5)
 *                                 bit0 = "."         (punto decimal)
 *   LCDMEM[pos5+1] = LCDMEM[3]:  bit2 = grado "o"
 * ----------------------------------------------------------------------- */
#define LCD_NEG    0x04  /* Signo negativo "-"  — LCDMEM[pos1+1] bit 2  */
#define LCD_COLON  0x04  /* Dos puntos  ":"     — LCDMEM[pos2+1] o LCDMEM[pos4+1] bit 2 */
#define LCD_POINT  0x01  /* Punto decimal "."   — LCDMEM[posX+1] bit 0  */
                         /* Existe en todas las posiciones. DEBE ponerse */
                         /* DESPUES del showChar de esa posicion o sera  */
                         /* borrado por la escritura de la word completa. */
#define LCD_DEGREE 0x04  /* Grado               — LCDMEM[pos5+1] bit 2  */

/* Antena y comunicacion (fuente: biblioteca Energia MSP430):             */
#define LCD_ANT   0x04  /* Antena / RADIO       — LCDMEM[pos3+1] bit 2   */
#define LCD_TX    0x04  /* Transmision TX        — LCDMEM[pos6+1] bit 2   */
#define LCD_RX    0x01  /* Recepcion  RX         — LCDMEM[pos6+1] bit 0   */
/* Nota: pos6+1 bit 0x01 = RX (no DP). pos6 no tiene punto decimal.       */

void Init_LCD(void);
void displayScrollText(char*);
void showChar(char, int);
void clearLCD(void);


#endif /* HAL_LCD_H_ */
