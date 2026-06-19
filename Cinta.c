/*
 * Cinta.c
 *
 *  Created on: 19 jun. 2026
 *      Author: benja
 */

#include "Cinta.h"

void Init_Cinta (void){

    GPIO_setAsInputPinWithPullUpResistor(BTN_PORT, BTN_PIN);
    GPIO_clearInterrupt(BTN_PORT, BTN_PIN);
    GPIO_selectInterruptEdge(BTN_PORT, BTN_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(BTN_PORT, BTN_PIN);

}

void Cinta_Recibe_Caja (COLOR_T Color){

    switch (Color) {
            case ROJO:
                Cnt_Rojo ++;
                show_Color_Amount(Cnt_Rojo, ROJO);
                send_Color_UART(ROJO);
                break;
            case VERDE:
                Cnt_Verde ++;
                show_Color_Amount(Cnt_Verde, VERDE);
                send_Color_UART(VERDE);
                break;
            case AZUL:
                Cnt_Azul ++;
                show_Color_Amount(Cnt_Azul, AZUL);
                send_Color_UART(AZUL);
                break;
            default:
                Cnt_Total = Cnt_Rojo + Cnt_Verde + Cnt_Azul;
                show_Color_Amount(Cnt_Total, TRANSPARENTE);
        }

}

#pragma vector = PORT2_VECTOR
__interrupt void ISR_Puerto2(void) {
    if (GPIO_getInterruptStatus(BTN_PORT, BTN_PIN)) {
        //Borramos Interrupcion
        GPIO_clearInterrupt(BTN_PORT, BTN_PIN);

        /*Se resetean contadores*/
        Cnt_Rojo = 0;
        Cnt_Verde = 0;
        Cnt_Azul = 0;
        Cnt_Total = 0;
    }
}



