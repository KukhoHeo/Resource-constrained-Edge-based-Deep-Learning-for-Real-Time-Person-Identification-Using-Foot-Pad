/*
 * FOOT_REGISTER.h
 *
 *  Created on: 2020. 6. 4.
 *      Author: Heo
 */

#ifndef APP_FOOT_REGISTER_LEFT_H_
#define APP_FOOT_REGISTER_LEFT_H_

#include <FOOT_HARD.h>

static GPIO_InitTypeDef GPIO_InitStruct;
static void Shift_Register_Left_Init() {
	//Left side init
	Shift_Register_LEFT_DATA_CLK_ENABLE();
	Shift_Register_LEFT_CLK_CLK_ENABLE();
	Shift_Register_LEFT_CE_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = Shift_Register_LEFT_DATA_PIN;
	HAL_GPIO_Init(Shift_Register_LEFT_DATA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_LEFT_CLK_PIN;
	HAL_GPIO_Init(Shift_Register_LEFT_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_LEFT_CE_PIN;
	HAL_GPIO_Init(Shift_Register_LEFT_CE_PORT, &GPIO_InitStruct);

	//Right side init
	Shift_Register_RIGHT_DATA_CLK_ENABLE();
	Shift_Register_RIGHT_CLK_CLK_ENABLE();
	Shift_Register_RIGHT_CE_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = Shift_Register_RIGHT_DATA_PIN;
	HAL_GPIO_Init(Shift_Register_RIGHT_DATA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_RIGHT_CLK_PIN;
	HAL_GPIO_Init(Shift_Register_RIGHT_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_RIGHT_CE_PIN;
	HAL_GPIO_Init(Shift_Register_RIGHT_CE_PORT, &GPIO_InitStruct);
}
static void Read_Data_to_Left_Shift_Register(int Data) {
	//Init Data, CLK, CE pin
	HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin init to 0
	HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin init to 0
	HAL_GPIO_WritePin(Shift_Register_LEFT_CE_PORT, Shift_Register_LEFT_CE_PIN, GPIO_PIN_RESET); //CE pin init to 0

	/*Data Set Sequence
	 * ==> 1. Data pin Set
	 * ==> 2. CLK pin to set high to save Data to Register*/
	if (Data == 1) {
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_SET); //Data pin set to 1
		task_sleepms(1); //Sleep Task (min 25ns)
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		task_sleepms(1); //Sleep Task (min 20ns)
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_SET); //CLK pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)
	} else if (Data == 0) {
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 0
		task_sleepms(1); //Sleep Task (min 25ns)
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		task_sleepms(1); //Sleep Task (min 20ns)
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_SET); //CLK pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)
	}
	else{
		printf("Please Select Data 0 or 1\n");
	}
}
static void Shift_to_Left_Shift_Register() {
	/*Register is shifted by CE pin
	 * if CE has Rising and falling edge, the register is shift.
	 * this function is doing parallel shift data and save 0 data*/

	HAL_GPIO_WritePin(Shift_Register_LEFT_CE_PORT, Shift_Register_LEFT_CE_PIN, GPIO_PIN_SET); //CE pin set to 1
	HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
	task_sleepms(1); //Sleep Task (min 20ns)
	HAL_GPIO_WritePin(Shift_Register_LEFT_CE_PORT, Shift_Register_LEFT_CE_PIN, GPIO_PIN_RESET); //CE pin set to 0
	task_sleepms(1); //Sleep Task (min 20ns)
}

#endif /* APP_FOOT_REGISTER_LEFT_H_ */
