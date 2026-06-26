/*
 * Cinta.c
 *
 *  Created on: 19 jun. 2026
 *      Author: benja
 */

#include "Cinta.h"

  volatile COLOR_T Color = AZUL;
  volatile uint8_t Cnt_Rojo = 0, Cnt_Verde = 0, Cnt_Azul = 0, Cnt_Total = 0;
  volatile uint8_t flag_1_seg = 0;
  volatile uint8_t flag_Caja = 0;



void Init_Cinta (void){
    GPIO_setAsInputPinWithPullUpResistor(BTN_PORT, BTN_PIN);
    GPIO_clearInterrupt(BTN_PORT, BTN_PIN);
    GPIO_selectInterruptEdge(BTN_PORT, BTN_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(BTN_PORT, BTN_PIN);

    //Configura botón en P2.7 con pull-up
    GPIO_setAsInputPinWithPullUpResistor(BTN_EXT_PORT, BTN_EXT_PIN);
    GPIO_clearInterrupt(BTN_EXT_PORT, BTN_EXT_PIN);
    //Selecciona flanco H→L (botón presionado = pin LOW)
    GPIO_selectInterruptEdge(BTN_EXT_PORT, BTN_EXT_PIN,GPIO_HIGH_TO_LOW_TRANSITION);
    //Habilitar interrupción del pin
    GPIO_enableInterrupt(BTN_EXT_PORT, BTN_EXT_PIN);

}

void Cinta_Recibe_Caja (void){

    //Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 1500); /*MUEVE SERVO AL Origen*/
    Color = TCS3200_Read_Color();
    switch (Color) {
            case ROJO:
                Cnt_Rojo ++;
                show_Color_Amount(Cnt_Rojo, ROJO);
                send_Color_UART(ROJO);
                Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 1000); /*MUEVE SERVO*/
                break;
            case VERDE:
                Cnt_Verde ++;
                show_Color_Amount(Cnt_Verde, VERDE);
                send_Color_UART(VERDE);
                //Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 1500); /*MUEVE SERVO AL Origen*/
                Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 2250);/*MUEVE SERVO*/
                break;
            case AZUL:
                Cnt_Azul ++;
                show_Color_Amount(Cnt_Azul, AZUL);
                send_Color_UART(AZUL);
                Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 1600); /*MUEVE SERVO AL Origen*/
                break;
            default:
                break;
        }


}

#pragma vector = PORT2_VECTOR
__interrupt void ISR_Puerto2(void) {

    //Guarda estado de P2.6
    uint16_t status = GPIO_getInterruptStatus(BTN_PORT, BTN_PIN);

    if (status & GPIO_PIN6) {
        //Borramos Interrupcion
        GPIO_clearInterrupt(BTN_PORT, BTN_PIN);

        /*Se resetean contadores*/
        Cnt_Rojo = 0;
        Cnt_Verde = 0;
        Cnt_Azul = 0;
        Cnt_Total = 0;
    }

    //Guarda estado de P2.7
    status = GPIO_getInterruptStatus(BTN_EXT_PORT, BTN_EXT_PIN);

     if(status & GPIO_PIN7){   //Evalua que se haya presionado el botón en P1.7

         //Se limpia flag de botón en P1.7
         GPIO_clearInterrupt(BTN_EXT_PORT, BTN_EXT_PIN);
         flag_Caja = 1;
     }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_CCR0_ISR(void) {
    volatile static uint16_t Cnt_1_Seg = 0;
    Cnt_1_Seg ++;
    if (Cnt_1_Seg >= INTERRUPCIONES_PARA_1_SEG) {
        Cnt_1_Seg = 0;
        flag_1_seg = 1;
        // Paso 1 segundo
    }

}


