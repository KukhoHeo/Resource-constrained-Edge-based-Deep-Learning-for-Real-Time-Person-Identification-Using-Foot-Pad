/*
 * FOOT_WIFI.h
 *
 *  Created on: 2020. 7. 15.
 *      Author: Heo
 */

#ifndef APP_FOOT_WIFI_C_
#define APP_FOOT_WIFI_C_

#include <FOOT_HARD.h>
static GPIO_InitTypeDef GPIO_InitStruct_WIFI;

UART_HandleTypeDef Uart_WIFI;

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  WIFI_USARTx_TX_GPIO_CLK_ENABLE();
  WIFI_USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USARTx clock */
  WIFI_USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = WIFI_USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = WIFI_USARTx_TX_AF;

  HAL_GPIO_Init(WIFI_USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = WIFI_USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = WIFI_USARTx_RX_AF;

  HAL_GPIO_Init(WIFI_USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
	WIFI_USARTx_FORCE_RESET();
	WIFI_USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(WIFI_USARTx_TX_GPIO_PORT, WIFI_USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(WIFI_USARTx_RX_GPIO_PORT, WIFI_USARTx_RX_PIN);

}

static void Wifi_Init() {

	Uart_WIFI.Instance = WIFI_USARTx;
	Uart_WIFI.Init.BaudRate = 115200;
	Uart_WIFI.Init.WordLength = UART_WORDLENGTH_8B;
	Uart_WIFI.Init.StopBits = UART_STOPBITS_1;
	Uart_WIFI.Init.Parity = UART_PARITY_NONE;
	Uart_WIFI.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	Uart_WIFI.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&Uart_WIFI) != HAL_OK) {
		/* Initialization Error */
		//Error_Handler();
		printf("UART ERROR\n");
	}
	WIFI_ENABLE_ENABLE();
	WIFI_RESET_ENABLE();
	GPIO_InitStruct_WIFI.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_WIFI.Pull = GPIO_PULLUP;
	GPIO_InitStruct_WIFI.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct_WIFI.Pin = WIFI_ENABLE_PIN;
	HAL_GPIO_Init(WIFI_ENABLE_PORT, &GPIO_InitStruct_WIFI);
	task_sleepms(1); //Sleep Task (min 25ns)

	//GPIO_InitStruct_WIFI.Pin = WIFI_RESET_PIN;
	//HAL_GPIO_Init(WIFI_RESET_PORT, &GPIO_InitStruct_WIFI);

	HAL_GPIO_WritePin(WIFI_ENABLE_PORT, WIFI_ENABLE_PIN, GPIO_PIN_SET);
	task_sleepms(1); //Sleep Task (min 25ns)
	HAL_GPIO_WritePin(WIFI_ENABLE_PORT, WIFI_ENABLE_PIN, GPIO_PIN_RESET);
	task_sleepms(1); //Sleep Task (min 25ns)
}

#endif /* APP_FOOT_WIFI_C_ */
