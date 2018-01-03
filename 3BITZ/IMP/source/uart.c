/*
 * uart.c
 *
 *  Created on: 19.11.2017
 *      Author: Dávid Bolvanský (xbolva00)
 */

#include "uart.h"

void UART5_Init(void) {
	uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	UART_Init(UART5, &config, kCLOCK_BusClk);
}

void UART5_print(char *msg) {
	UART_WriteBlocking(UART5, (uint8_t *) msg, strlen(msg));
}




