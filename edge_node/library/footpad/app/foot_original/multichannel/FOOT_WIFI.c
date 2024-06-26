/*
 * FOOT_WIFI.c
 *
 *  Created on: 2021. 5. 25.
 *      Author: Heo
 */


#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_WIFI == 1)

#include <ubinos/bsp/arch.h>

#include <FOOT_WIFI.h>

UART_HandleTypeDef DTTY_STM32_UART_HANDLE;

UART_HandleTypeDef ESP8266_UART_HANDLE;
esp8266at_t _g_esp8266at;

/**
 * @brief  Tx Transfer completed callback
 * @param  huart: UART handle.
 * @note   This example shows a simple way to report end of DMA Tx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void ESP8266_UART_IRQHandler(void)
{
    HAL_UART_IRQHandler(&ESP8266_UART_HANDLE);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == ESP8266_UART)
    {
        esp8266_uart_tx_callback();
    }

}

/**
 * @brief  Rx Transfer completed callback
 * @param  huart: UART handle
 * @note   This example shows a simple way to report end of DMA Rx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == ESP8266_UART)
    {
        esp8266_uart_rx_callback();
    }
}

/**
 * @brief  UART error callbacks
 * @param  huart: UART handle
 * @note   This example shows a simple way to report transfer error, and you can
 *         add your own implementation.
 * @retval None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == ESP8266_UART)
    {
        esp8266_uart_err_callback();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (huart->Instance == ESP8266_UART)
    {
        /* Configure ESP8266 NRST, NCS */

        /* Enable the GPIO clock */
        ESP8266_NRST_GPIO_CLK_ENABLE();
        ESP8266_CS_GPIO_CLK_ENABLE();

        /* Set the NRST GPIO pin configuration parametres */
        GPIO_InitStruct.Pin = ESP8266_NRST_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

        /* Configure the NRST IO */
        HAL_GPIO_Init(ESP8266_NRST_GPIO_Port, &GPIO_InitStruct);

        /* Assert NRST pin */
        HAL_GPIO_WritePin(ESP8266_NRST_GPIO_Port, ESP8266_NRST_Pin, GPIO_PIN_RESET);

        /* Set the CS GPIO pin configuration parametres */
        GPIO_InitStruct.Pin = ESP8266_CS_Pin;

        /* Configure the CS IO */
        HAL_GPIO_Init(ESP8266_CS_GPIO_Port, &GPIO_InitStruct);

        /* Assert CS */
        HAL_GPIO_WritePin(ESP8266_CS_GPIO_Port, ESP8266_CS_Pin, GPIO_PIN_SET);


        /* Configure ESP8266 UART */

        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        ESP8266_UART_TX_GPIO_CLK_ENABLE();
        ESP8266_UART_RX_GPIO_CLK_ENABLE();
        /* Enable ESP8266_UART clock */
        ESP8266_UART_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin = ESP8266_UART_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = ESP8266_UART_TX_AF;

        HAL_GPIO_Init(ESP8266_UART_TX_GPIO_Port, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = ESP8266_UART_RX_Pin;
        GPIO_InitStruct.Alternate = ESP8266_UART_RX_AF;

        HAL_GPIO_Init(ESP8266_UART_RX_GPIO_Port, &GPIO_InitStruct);

        /*##-3- Configure the NVIC for UART ########################################*/
        /* NVIC for USART */
        HAL_NVIC_SetPriority(ESP8266_UART_IRQn, NVIC_PRIO_MIDDLE, 0);
        HAL_NVIC_EnableIRQ(ESP8266_UART_IRQn);
    }
}

/**
 * @brief UART MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO, DMA and NVIC configuration to their default state
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

    if (huart->Instance == ESP8266_UART)
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
}

#endif /* (FOOT_ORIGINAL__USE_WIFI == 1) */
