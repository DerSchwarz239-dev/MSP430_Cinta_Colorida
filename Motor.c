/*
 * Motor.c
 *
 *  Created on: 1 jun. 2026
 *      Author: Matias
 */
#include <driverlib.h>
#include "Motor.h"
#include "hal_LCD.h"

volatile uint16_t lectura_pot = 0;
volatile uint8_t ADC_flag = 0;
volatile uint8_t Pausado = 0;


void Motores_init(void){
    //Configura P1.7 como salida del timer con CCR1
    GPIO_setAsPeripheralModuleFunctionOutputPin(SERVO_PORT,SERVO_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
    //Configura P1.6 como salida del timer con CCR2
    GPIO_setAsPeripheralModuleFunctionOutputPin(MOTORDC_PORT,MOTORDC_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
    //Configura P8.1 como entrada hacia el ADC
    GPIO_setAsPeripheralModuleFunctionInputPin(ADC_PORT, ADC_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

    //Configura botón en P1.2 con pull-up
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    //Selecciona flanco H→L (botón presionado = pin LOW)
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2,GPIO_HIGH_TO_LOW_TRANSITION);
    //Habilitar interrupción del pin
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);


    Timer_init();
    //Servo_Start_PWM();
    MotorDC_Start_PWM();
    ADC_config ();

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
    p.startTimer = false;

    Timer_A_initUpMode(TIMER_A0_BASE, &p);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);


}

void Servo_Start_PWM(void){

    // Configura el canal de PWM para conmutar
    Timer_A_initCompareModeParam cpServo = {0};
    cpServo.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; // Se usa CCR1 para conmutar
    cpServo.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    cpServo.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;  //modo 7

    /*
     * Pulso de 1ms -> Servo a 0°
     * Pulso de 1.5ms -> Servo a 90°
     * Pulso de 2ms -> Servo a 180°
     */

    cpServo.compareValue = 1500;   // Arranca en 90° (1500us = 1.5ms)


    Timer_A_initCompareMode(TIMER_A0_BASE, &cpServo);
}

void MotorDC_Start_PWM(void){

    // Configura el canal de PWM para conmutar
    Timer_A_initCompareModeParam cpMotor = {0};
    cpMotor.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2; // Se usa CCR2 para conmutar
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

    ADC_configureMemory(ADC_BASE, ADC_INPUT_A9  , ADC_VREFPOS_AVCC, ADC_VREFNEG_AVSS); //VrefPos = 3.3V y VrefNeg = 0V
    ADC_enableInterrupt(ADC_BASE,ADC_COMPLETED_INTERRUPT);

}


#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void) {
        ADC_clearInterrupt(ADC_BASE,ADC_COMPLETED_INTERRUPT_FLAG);
        lectura_pot = ADC_getResults(ADC_BASE);
        ADC_flag = 1;
    //__bic_SR_register_on_exit(LPM0_bits); // despertar CPU
}

#pragma vector = PORT1_VECTOR
__interrupt void ISR_Puerto1(void){

   //Guarda estado de P1.2
   uint16_t status = GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN2);

    if(status & GPIO_PIN2){   //Evalua que se haya presionado el botón en P1.2
        //Se limpia flag de botón en P1.2
        GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
        if(Pausado){//Si está pausado, prende el timer
            Pausado = 0;
            Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
        }else{
            Pausado = 1;
            Timer_A_stop(TIMER_A0_BASE);
        }
    }



}


