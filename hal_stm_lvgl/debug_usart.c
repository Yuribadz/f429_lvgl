/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "debug_usart.h"

UART_HandleTypeDef huart1;
static DMA_HandleTypeDef hdma_usart1_rx;
static DMA_HandleTypeDef hdma_usart1_tx;
static uint8_t tx_buffer[TX_BUFFER_SIZE];
static uint8_t rx_buffer[RX_BUFFER_SIZE];

HAL_StatusTypeDef Debug_Uart_Print(const uint8_t msg[], size_t size) {
	if (size > 4000UL) {
		return HAL_ERROR;
	} else {
		memset(tx_buffer, 0U, TX_BUFFER_SIZE);
		memcpy(tx_buffer, msg, size);
		return HAL_UART_Transmit_DMA(&huart1, tx_buffer, size);
	}
}

/**
 * @brief This function handles DMA2 stream2 global interrupt.
 */
void DMA2_Stream2_IRQHandler(void) {
	/* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

	/* USER CODE END DMA2_Stream2_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
	/* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

	/* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream7 global interrupt.
 */
void DMA2_Stream7_IRQHandler(void) {
	/* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

	/* USER CODE END DMA2_Stream7_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
	/* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

	/* USER CODE END DMA2_Stream7_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) {
	/* USER CODE BEGIN USART1_IRQn 0 */

	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */

	/* USER CODE END USART1_IRQn 1 */
}

static void Error_Handler(void) {
	__disable_irq();
	while (1) {
	}
}

static void RCC_Config(void) {
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
}

static void Gpio_Config(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/**USART1 GPIO Configuration
	 PA9     ------> USART1_TX
	 PA10     ------> USART1_RX
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void Uart_Config(void) {
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
}

static void Dma_Config(UART_HandleTypeDef *uartHandle) {
	/* USART1 DMA Init */
	/* USART1_RX Init */
	hdma_usart1_rx.Instance = DMA2_Stream2;
	hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_NORMAL;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_usart1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_usart1_rx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_usart1_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK) {
		Error_Handler();
	}

	__HAL_LINKDMA(uartHandle, hdmarx, hdma_usart1_rx);

	/* USART1_TX Init */
	hdma_usart1_tx.Instance = DMA2_Stream7;
	hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_tx.Init.Mode = DMA_NORMAL;
	hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK) {
		Error_Handler();
	}
	__HAL_LINKDMA(uartHandle, hdmatx, hdma_usart1_tx);
}

static void Irq_Config(void) {
	/* DMA interrupt init */
	/* DMA2_Stream2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	/* USART1 interrupt Init */
	HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void Debug_Uart_Init(void) {
	Uart_Config();
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {
	if (uartHandle->Instance == USART1) {
		RCC_Config();
		Gpio_Config();
		Dma_Config(uartHandle);
		Irq_Config();
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {

	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		/* USART1 DMA DeInit */
		HAL_DMA_DeInit(uartHandle->hdmarx);
		HAL_DMA_DeInit(uartHandle->hdmatx);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
