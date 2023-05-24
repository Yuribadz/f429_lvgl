/*
 * debug_usart.h
 *
 *  Created on: 21 трав. 2023 р.
 *      Author: yurib
 */

#ifndef DEBUG_USART_H_
#define DEBUG_USART_H_

#include "stm32f4xx.h"

extern UART_HandleTypeDef huart1;
extern void debug_Uart_Init(void);

#endif /* DEBUG_USART_H_ */
