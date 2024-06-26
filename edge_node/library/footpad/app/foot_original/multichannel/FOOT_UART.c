/*
 * FOOT_UART.c
 *
 *  Created on: 2022. 3. 10.
 *      Author: Heo
 */
#include <ubinos.h>
#if (FOOT_ORIGINAL__USE_USART == 1)

#include "FOOT_UART.h"
#include <ubinos/bsp/arch.h>
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

void Foot_Uart_Init(void){
	UartHandle.Instance = ESP8266_UART;

	    UartHandle.Init.BaudRate = 115200;
	    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	    UartHandle.Init.StopBits = UART_STOPBITS_1;
	    UartHandle.Init.Parity = UART_PARITY_NONE;
	    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	    UartHandle.Init.Mode = UART_MODE_TX_RX;
	    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	    if (HAL_UART_Init(&UartHandle) != HAL_OK)
	    {
	        /* Initialization Error */
	    	UART_Error_Handler();
	    }

}
void UART_Error_Handler(void){
	LED_On('g');
  task_sleepms(10);
  LED_Off('g');
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  ESP8266_UART_TX_GPIO_CLK_ENABLE();
  ESP8266_UART_RX_GPIO_CLK_ENABLE();
  /* Enable USART1 clock */
  ESP8266_UART_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = ESP8266_UART_TX_Pin;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = ESP8266_UART_TX_AF;

  HAL_GPIO_Init(ESP8266_UART_TX_GPIO_Port, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = ESP8266_UART_RX_Pin;
  GPIO_InitStruct.Alternate = ESP8266_UART_RX_AF;

  HAL_GPIO_Init(ESP8266_UART_RX_GPIO_Port, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USARTx */
  HAL_NVIC_SetPriority(ESP8266_UART_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(ESP8266_UART_IRQn);
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
  ESP8266_UART_FORCE_RESET();
  ESP8266_UART_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(ESP8266_UART_TX_GPIO_Port, ESP8266_UART_TX_Pin);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(ESP8266_UART_RX_GPIO_Port, ESP8266_UART_RX_Pin);

  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(ESP8266_UART_IRQn);
}
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for USART data transmission
  */
void ESP8266_UART_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}
#endif /* (FOOT_ORIGINAL__USE_USART == 1) */