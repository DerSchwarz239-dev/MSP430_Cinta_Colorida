/*
 * Motor.c
 *
 *  Created on: 1 jun. 2026
 *      Author: Matias
 */
#include <driverlib.h>
#include "Motor.h"

volatile uint16_t lectura_pot = 0;
 volatile uint16_t ADC_flag = 0;

void Motores_init(void){
    //Configura P1.7 como salida del timer con CCR1
    GPIO_setAsPeripheralModuleFunctionOutputPin(SERVO_PORT,SERVO_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
    //Configura P1.8 como salida del timer con CCR2
    GPIO_setAsPeripheralModuleFunctionOutputPin(MOTORDC_PORT,MOTORDC_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
    Timer_init();
    Servo_Start_PWM();
    //MotorDC_Start_PWM();
    //void ADC_config ();

}
void Timer_init(void){

    //Configura el timer con un periodo de 20ms
    Timer_A_initUpModeParam p = {0};
    p.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    p.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1; //1 tick cada 1us
    p.timerPeriod = 19999; //(CCRO+1)*1us = 20ms
    p.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE; //Desactiva interrupcion por CCR0
    p.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE; //Desactiva interrupcion por desborde
    p.timerClear = TIMER_A_DO_CLEAR;
    p.startTimer = true;

    Timer_A_initUpMode(TIMER_A0_BASE, &p);
}

void Servo_Start_PWM(void){

    // Configura el canal de PWM para conmutar
    Timer_A_initCompareModeParam cpServo = {0};
    cpServo.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; // Se usa CCR1 para conmutar
    cpServo.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    cpServo.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;  //modo 7

    /*
     * Pulso de 1ms -> Servo a 0°
     * Pulso de 1.5ms -> Servo a 90°
     * Pulso de 2ms -> Servo a 180°
     */

    cpServo.compareValue = 1000;   // Arranca en 90° (1500us = 1.5ms)


    Timer_A_initCompareMode(TIMER_A0_BASE, &cpServo);
}

void MotorDC_Start_PWM(void){

    // Configura el canal de PWM para conmutar
    Timer_A_initCompareModeParam cpMotor = {0};
    cpMotor.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2; // Se usa CCR1 para conmutar
    cpMotor.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    cpMotor.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;  //modo 7

    cpMotor.compareValue = 10000;   // Arranca entregando un 50% de potencia(velocidad media)

    Timer_A_initCompareMode(TIMER_A0_BASE, &cpMotor);

}

void ADC_config (void){
    //Inicializa ADC
    ADC_init(ADC_BASE, ADC_SAMPLEHOLDSOURCE_SC, ADC_CLOCKSOURCE_ADCOSC, ADC_CLOCKDIVIDER_1);
    //Enciende núcleo ADC
    ADC_enable(ADC_BASE);
    //Duración de ventana de muestreo. Configuración estándar.
    ADC_setupSamplingTimer(ADC_BASE, ADC_CYCLEHOLD_16_CYCLES,false);

    ADC_configureMemory(ADC_BASE, ADC_INPUT_A4  , ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS); //VrefPos = 3.3V y VrefNeg = 0V

}


#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void) {
        ADC_clearInterrupt(ADC_BASE,ADC_COMPLETED_INTERRUPT_FLAG);
        lectura_pot = ADC_getResults(ADC_BASE);
        ADC_flag = 1;
    //__bic_SR_register_on_exit(LPM0_bits); // despertar CPU
}


//REVISAR
/*
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TA0_A1_ISR(void){
    switch(__even_in_range(TA0IV,14)){

        case 0x02: //ccr1
                GPIO_setOutputLowOnPin(SERVO_PORT, selectedPins)(SERVO_PORT, SERVO_PIN);
                break;
        }
        case 0x0x: //ccr2
                GPIO_low(SERVO_PORTx, SERVO_PINx);
                break;
        }

}/*
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_A1_ISR(void){
                    GPIO_high(SERVO_PORT, SERVO_PIN);
             Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, valorglobal);

}
*/

