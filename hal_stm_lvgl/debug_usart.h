/*
 * debug_usart.h
 *
 *  Created on: 21 трав. 2023 р.
 *      Author: yurib
 */

#ifndef DEBUG_USART_H_
#define DEBUG_USART_H_

#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"

#define TX_BUFFER_SIZE 4096
#define RX_BUFFER_SIZE 4096
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define TRACE "TRACE: " __FILE__ ":" STR(__LINE__)
#define ERROR "ERROR: " __FILE__ ":" STR(__LINE__)
#define INFO "INFO: " __FILE__ ":" STR(__LINE__)


extern UART_HandleTypeDef huart1;
extern void Debug_Uart_Init(void);
extern HAL_StatusTypeDef Debug_Uart_Print(const uint8_t msg[], size_t size);

#endif /* DEBUG_USART_H_ */
