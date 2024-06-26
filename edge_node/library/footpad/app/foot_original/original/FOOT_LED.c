/*
 * FOOT_LED.c
 *
 *  Created on: 2020. 10. 4.
 *      Author: Heo
 */

#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL != 1)

#include <FOOT_LED.h>

void Print_Foot_Pad() {
	for (int row = 0; row < 48; row++) {
		for (int col = 0; col < 96; col++) {
			printf("%5d", Foot_Array[row][col]);
		}
		printf("\n");
	}
}

void Init_Foot_Pad(){
	for (int row = 0; row < 48; row++) {
			for (int col = 0; col < 96; col++) {
				Foot_Array[row][col]=0;
			}
		}
}
void Led_Init() {
	LED1_GPIO_CLK_ENABLE();
	LED2_GPIO_CLK_ENABLE();
	LED3_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = LED1_PIN;
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED3_PIN;
	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);
}
void Led_Turn_on_1s(char *LED_Name) {
	if (!strcmp("RED", LED_Name)) {
		//		HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN); //toggle code
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET); //pin init to 0
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET); //pin to 1 to turn on
		task_sleep(1000);
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET); //pin to 0 to turn off
	} else if (!strcmp("GREEN", LED_Name)) {
//		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET); //pin init to 0
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET); //pin to 1 to turn on
		task_sleep(1000);
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET); //pin to 0 to turn off
	} else if (!strcmp("BLUE", LED_Name)) {

//		HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET); //pin init to 0
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET); //pin to 1 to turn on
		task_sleep(1000);
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET); //pin init to 0 to turn off
	} else {
		printf("Please Select LED Name RED or GREEN or BLUE\n");
	}
}
void Led_Turn_on(char *LED_Name) {
	if (!strcmp("RED", LED_Name)) {
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET); //pin init to 0
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET); //pin to 1 to turn on
	} else if (!strcmp("GREEN", LED_Name)) {
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET); //pin init to 0
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET); //pin to 1 to turn on
	} else if (!strcmp("BLUE", LED_Name)) {
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET); //pin init to 0
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET); //pin to 1 to turn on
	} else {
		printf("Please Select LED Name RED or GREEN or BLUE\n");
	}
}
void Led_Turn_off(char *LED_Name) {
	if (!strcmp("RED", LED_Name)) {
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET); //pin to 0 to turn off
	} else if (!strcmp("GREEN", LED_Name)) {
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET); //pin to 0 to turn off
	} else if (!strcmp("BLUE", LED_Name)) {
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET); //pin to 0 to turn off
	} else {
		printf("Please Select LED Name RED or GREEN or BLUE\n");
	}
}

#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL != 1) */
