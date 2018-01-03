/*
 * uart.h
 *
 *  Created on: 19.11.2017
 *      Author: Dávid Bolvanský
 */

#ifndef SOURCE_UART_H_
#define SOURCE_UART_H_

#include "board.h"
#include "fsl_uart.h"

/**
 * Initializes UART5
 */
void UART5_Init(void);
/**
 * Prints data to UART5
 * @param msg string to be printed
 */
void UART5_print(char *msg);





#endif /* SOURCE_UART_H_ */
