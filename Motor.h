/*
 * Motor.h
 *
 *  Created on: 26 may. 2026
 *      Author: Matias
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include <driverlib.h>

#define SERVO_PORT GPIO_PORT_P1
#define SERVO_PIN  GPIO_PIN7
#define MOTORDC_PORT GPIO_PORT_P1
#define MOTORDC_PIN  GPIO_PIN6
#define ADC_PORT  GPIO_PORT_P8
#define ADC_PIN  GPIO_PIN1
#define BTN_SS_PORT  GPIO_PORT_P1
#define BTN_SS_PIN  GPIO_PIN2




void Timer_init(void);
void Servo_Start_PWM(void);
void MotorDC_Start_PWM(void);
void ADC_config (void);
void Motores_init(void);
void ADC_function(void);



#endif /* MOTOR_H_ */
