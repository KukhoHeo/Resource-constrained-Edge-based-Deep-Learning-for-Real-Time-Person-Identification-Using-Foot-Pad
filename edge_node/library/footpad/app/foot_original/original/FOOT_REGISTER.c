/*
 * FOOT_REGISTER.c
 *
 *  Created on: 2020. 10. 4.
 *      Author: Heo
 */

#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL != 1)

#include <FOOT_REGISTER.h>

/**************************************************/
/*************** SHIFT REGISTER *******************/
/**************************************************/

void Shift_Register_Init() {
	//Right side pin enable
	Shift_Register_RIGHT_DATA_CLK_ENABLE();
	Shift_Register_RIGHT_CLK_CLK_ENABLE();
	Shift_Register_RIGHT_CE_CLK_ENABLE();

	//left side pin enable
	Shift_Register_LEFT_DATA_CLK_ENABLE();
	Shift_Register_LEFT_CLK_CLK_ENABLE();
	Shift_Register_LEFT_CE_CLK_ENABLE();

	//gpio setting
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	//Right side gpio init
	GPIO_InitStruct.Pin = Shift_Register_RIGHT_DATA_PIN;
	HAL_GPIO_Init(Shift_Register_RIGHT_DATA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_RIGHT_CLK_PIN;
	HAL_GPIO_Init(Shift_Register_RIGHT_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_RIGHT_CE_PIN;
	HAL_GPIO_Init(Shift_Register_RIGHT_CE_PORT, &GPIO_InitStruct);

	//Left side gpio init
	GPIO_InitStruct.Pin = Shift_Register_LEFT_DATA_PIN;
	HAL_GPIO_Init(Shift_Register_LEFT_DATA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_LEFT_CLK_PIN;
	HAL_GPIO_Init(Shift_Register_LEFT_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Shift_Register_LEFT_CE_PIN;
	HAL_GPIO_Init(Shift_Register_LEFT_CE_PORT, &GPIO_InitStruct);

	//Init right side Data, CLK, CE pin
	HAL_GPIO_WritePin(Shift_Register_RIGHT_DATA_PORT, Shift_Register_RIGHT_DATA_PIN, GPIO_PIN_RESET); //Data pin init to 0
	HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_RESET); //CLK pin init to 0
	HAL_GPIO_WritePin(Shift_Register_RIGHT_CE_PORT, Shift_Register_RIGHT_CE_PIN, GPIO_PIN_RESET); //CE pin init to 0

	//Init left side Data, CLK, CE pin
	HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin init to 0
	HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin init to 0
	HAL_GPIO_WritePin(Shift_Register_LEFT_CE_PORT, Shift_Register_LEFT_CE_PIN, GPIO_PIN_RESET); //CE pin init to 0

	//high purse
	task_sleepms(1);
}

void Read_Data_to_Shift_Register(int Data) {
	/*Data Set Sequence
	 * ==> 1. Data pin Set
	 * ==> 2. CLK pin to set high to save Data to Register*/
	if (Data == 1) {
		//data pin set
		HAL_GPIO_WritePin(Shift_Register_RIGHT_DATA_PORT, Shift_Register_RIGHT_DATA_PIN, GPIO_PIN_SET); //Data pin set to 1
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_SET); //Data pin set to 1
		task_sleepms(1); //Sleep Task (min 25ns)

		//clk pin reset
		HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		task_sleepms(1); //Sleep Task (min 20ns)

		//clk pin set
		HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_SET); //CLK pin set to 1
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_SET); //CLK pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)

		//clk pin reset & data pin reset
		HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_RIGHT_DATA_PORT, Shift_Register_RIGHT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 1
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)
		//left side move

	} else if (Data == 0) {
		//data pin reset
		HAL_GPIO_WritePin(Shift_Register_RIGHT_DATA_PORT, Shift_Register_RIGHT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 0
		task_sleepms(1); //Sleep Task (min 25ns)

		//clk pin reset
		HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		task_sleepms(1); //Sleep Task (min 20ns)

		//clk pin set
		HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_SET); //CLK pin set to 1
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_SET); //CLK pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)

		//clk pin reset & data pin reset
		HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_RIGHT_DATA_PORT, Shift_Register_RIGHT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 1
		HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
		HAL_GPIO_WritePin(Shift_Register_LEFT_DATA_PORT, Shift_Register_LEFT_DATA_PIN, GPIO_PIN_RESET); //Data pin set to 1
		task_sleepms(1); //Sleep Task (min 20ns)
	} else {
		printf("Please Select Data 0 or 1\n");
	}
}
void Shift_to_Shift_Register() {
	/*Register is shifted by CE pin
	 * if CE has Rising and falling edge, the register is shift.
	 * this function is doing parallel shift data and save 0 data*/

	//clk reset & ce set
	HAL_GPIO_WritePin(Shift_Register_RIGHT_CE_PORT, Shift_Register_RIGHT_CE_PIN, GPIO_PIN_SET); //CE pin set to 1
	HAL_GPIO_WritePin(Shift_Register_RIGHT_CLK_PORT, Shift_Register_RIGHT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
	HAL_GPIO_WritePin(Shift_Register_LEFT_CE_PORT, Shift_Register_LEFT_CE_PIN, GPIO_PIN_SET); //CE pin set to 1
	HAL_GPIO_WritePin(Shift_Register_LEFT_CLK_PORT, Shift_Register_LEFT_CLK_PIN, GPIO_PIN_RESET); //CLK pin set to 0
	task_sleepms(1); //Sleep Task (min 20ns)

	//ce reset
	HAL_GPIO_WritePin(Shift_Register_RIGHT_CE_PORT, Shift_Register_RIGHT_CE_PIN, GPIO_PIN_RESET); //CE pin set to 0
	HAL_GPIO_WritePin(Shift_Register_LEFT_CE_PORT, Shift_Register_LEFT_CE_PIN, GPIO_PIN_RESET); //CE pin set to 0
	task_sleepms(1); //Sleep Task (min 20ns)
}

#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL != 1) */
