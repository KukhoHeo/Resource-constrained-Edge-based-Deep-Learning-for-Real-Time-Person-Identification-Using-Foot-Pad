/*
 * FOOT_POWER.h
 *
 *  Created on: 2020. 6. 5.
 *      Author: Heo
 */

#ifndef APP_FOOT_POWER_H_
#define APP_FOOT_POWER_H_

#include <FOOT_HARD.h>

static GPIO_InitTypeDef GPIO_InitStruct;
static void Power_Init() {
	//Power Pin Init
	WIFI_PW_EN_CLK_ENABLE();
	Logic_PW_EN_CLK_ENABLE();
	RF_T_EN_CLK_ENABLE();
	RF_R_EN_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = WIFI_PW_EN_PIN;
	HAL_GPIO_Init(WIFI_PW_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Logic_PW_EN_PIN;
	HAL_GPIO_Init(Logic_PW_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RF_T_EN_PIN;
	HAL_GPIO_Init(RF_T_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RF_R_EN_PIN;
	HAL_GPIO_Init(RF_R_EN_PORT, &GPIO_InitStruct);
}

//static void WIFI_Enable() {
//	HAL_GPIO_WritePin(WIFI_PW_EN_PORT, WIFI_PW_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
//	task_sleepms(1);
//	HAL_GPIO_WritePin(WIFI_PW_EN_PORT, WIFI_PW_EN_PIN, GPIO_PIN_SET);
//}


static void Logic_Enable() {
	HAL_GPIO_WritePin(Logic_PW_EN_PORT, Logic_PW_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(Logic_PW_EN_PORT, Logic_PW_EN_PIN, GPIO_PIN_SET);
}

static void RF_T_Enable() {
	HAL_GPIO_WritePin(RF_T_EN_PORT, RF_T_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(RF_T_EN_PORT, RF_T_EN_PIN, GPIO_PIN_SET);
}

static void RF_R_Enable() {
	HAL_GPIO_WritePin(RF_R_EN_PORT, RF_R_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(RF_R_EN_PORT, RF_R_EN_PIN, GPIO_PIN_SET);
}
#endif /* APP_FOOT_POWER_H_ */
