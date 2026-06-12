/*
 * Mod_HC_05.c
 *
 *  Created on: 1 jun. 2026
 *      Author: benja
 */

#include "Mod_HC_05.h"


void Init_HC05 (void){
    /*Funcion dedicada a configurar el Modulo HC05 cuando se inicia*/
    /*Se hace configuracion para puerto UART*/
    /*
     * El modulo HC05 utiliza una configuracion de inicio con los siguientes parametros
     * Nombre: "HC-05"
     * PIN: 1234
     * Baudrate: 9600 bps
     * Modo: Esclavo
     * Si se desea otra configuracion, se debera alterar en MODO AT (leer datasheet de HC05 para mas informacion).
     */

    /*Se empleara el modulo bluetooth del ESP32 para transmitir al Celular, por problemas con el HC05
     * Se sigue usando UART para comunicar el MSP430 al ESP32.
     */

    /*Se configuran los pines P1.0 y P1.1 para comunicacion UART*/
    GPIO_setAsPeripheralModuleFunctionOutputPin(HC_PORT, HC_TX_PIN, GPIO_PRIMARY_MODULE_FUNCTION); //TX
    GPIO_setAsPeripheralModuleFunctionInputPin(HC_PORT, HC_RX_PIN, GPIO_PRIMARY_MODULE_FUNCTION); //RX
    /*Configurar P1.3 como salida en alto para EN*/
    //GPIO_setAsOutputPin(HC_PORT, HC_EN_PIN);
    //GPIO_setOutputHighOnPin(HC_PORT, HC_EN_PIN);

    /*Configuracion de UART*/
    // - Reloj: SMCLK = 1 MHz (desde el generador FLL de ACLK)
    // - Baudrate: 9600 bps (configuración estándar del HC-05)
    // - Formato: 8N1 (8 bits, sin paridad, 1 stop bit)
    /**************************************************
     * IMPORTANTE
     * AL USAR EL SMCLK SE DEBE TENER CUIDADO CON LOS MODOS DE BAJO CONSUMO QUE APAGUEN
     * EL SMCLK, PORQUE SINO, NO VA A FUNCIONAR VISTE.
     * LA IDEA ES QUE SOLO VAMOS A ENVIAR DATOS, NO RECEPCIONAR, ENTONCES VAMOS A ACTIVAR
     * EL MODULO SOLO CUANDO ENVIAMOS DATOS.
     **************************************************/
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 6;              // UCBRx para 9600 bps @ 1MHz
    param.firstModReg = 8;                 // UCBRFx (tabla de baudrate)
    param.secondModReg = 0x20;             // UCBRSx (tabla de baudrate)
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    EUSCI_A_UART_init(EUSCI_A0_BASE, &param);
    EUSCI_A_UART_enable(EUSCI_A0_BASE);
    /*Fin Configuracion UART*/
}

void send_Byte_UART(uint8_t data){
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, data);
}

void send_String_UART (const char* str){
    while (*str != '\0') {
        send_Byte_UART((uint8_t)*str);
        str++;
    }
}

void send_Color_UART (COLOR_T color){
    switch (color) {
            case ROJO:
                send_String_UART("ROJO\r\n");
                break;
            case VERDE:
                send_String_UART("VERDE\r\n");
                break;
            case AZUL:
                send_String_UART("AZUL\r\n");
                break;
            case TRANSPARENTE:
                break;
            default:
                send_String_UART("ESTE ES UN EASTER EGG\r\n");
     }
}





