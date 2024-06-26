#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\r ****UART-Hyperterminal communication based on DMA ****\n\r Enter 10 characters using keyboard :\n\r";
uint8_t aTxEndMessage[] = "\n\r Example Finished\n\r";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

static void Error_Handler(void);

static void rootfunc(void *arg);
static void task1func(void *arg);
static void task2func(void *arg);

int appmain(int argc, char *argv[]) {
    int r;

    r = task_create(NULL, rootfunc, NULL, task_getmiddlepriority(), 0, "root");
    if (0 != r) {
        logme("fail at task_create\r\n");
    }

    ubik_comp_start();

    return 0;
}

static void rootfunc(void *arg) {
    int r;

    /* Configure LED1, LED2 and LED3 */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
     - Word Length = 8 Bits
     - Stop Bit    = One Stop bit
     - Parity      = ODD parity
     - BaudRate    = 9600 baud
     - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance = USARTx;

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
        Error_Handler();
    }

    srand(time(NULL));

    r = task_create(NULL, task1func, NULL, task_getmiddlepriority(), 0, "task1");
    if (0 != r) {
        logme("fail at task_create\r\n");
    }

    r = task_create(NULL, task2func, NULL, task_getmiddlepriority(), 0, "task2");
    if (0 != r) {
        logme("fail at task_create\r\n");
    }

    /*##-2- Start the transmission process #####################################*/
    /* User start transmission data through "TxBuffer" buffer */
    if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*) aTxStartMessage, TXSTARTMESSAGESIZE) != HAL_OK)
    {
        /* Transfer error in transmission process */
        Error_Handler();
    }

    /*##-3- Put UART peripheral in reception process ###########################*/
    /* Any data received will be stored in "RxBuffer" buffer : the number max of
     data received is 10 */
    if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t*) aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
        /* Transfer error in reception process */
        Error_Handler();
    }

    /*##-4- Wait for the end of the transfer ###################################*/
    while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
    {
    }

    /*##-5- Send the received Buffer ###########################################*/
    if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*) aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
        /* Transfer error in transmission process */
        Error_Handler();
    }

    /*##-6- Wait for the end of the transfer ###################################*/
    while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
    {
    }

    /*##-7- Send the End Message ###############################################*/
    if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*) aTxEndMessage, TXENDMESSAGESIZE) != HAL_OK)
    {
        /* Turn LED3 on: Transfer error in transmission process */
        BSP_LED_On(LED3);
        while (1)
        {
        }
    }

    /*##-8- Wait for the end of the transfer ###################################*/
    while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
    {
    }

    /* Infinite loop */
    while (1)
    {
    }
}


static void task1func(void *arg) {
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        delayms = (rand() % 10 + 1) * 200;
        BSP_LED_Toggle(LED2);
        task_sleepms(delayms);
    }
}

static void task2func(void *arg) {
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++) {
        delayms = (rand() % 10 + 1) * 200;
        BSP_LED_Toggle(LED3);
        task_sleepms(delayms);
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while (1)
    {
    }
}

/**
 * @brief  Tx Transfer completed callback
 * @param  huart: UART handle.
 * @note   This example shows a simple way to report end of DMA Tx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Turn LED1 on: Transfer in transmission process is correct */
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
    /* Turn LED2 on: Transfer in reception process is correct */
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
    /* Turn LED3 on: Transfer error in reception/transmission process */
    BSP_LED_On(LED3);
    while (1)
    {
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

