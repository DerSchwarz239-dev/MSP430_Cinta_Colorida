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

//Variables conn prefijo extern para poder ser usadas en main.c -->REVISAR
extern volatile uint16_t lectura_pot = 0;
extern volatile uint16_t ADC_flag = 0;

void Timer_init(void);
void Servo_Start_PWM(void);
void MotorDC_Start_PWM(void);
void ADC_config (void);




#endif /* MOTOR_H_ */
