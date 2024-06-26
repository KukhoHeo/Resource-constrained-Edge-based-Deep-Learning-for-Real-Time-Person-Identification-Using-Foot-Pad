/*
 * FOOT_POWER.c
 *
 *  Created on: 2021. 1. 26.
 *      Author: Heo
 */


#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL == 1)

#include <FOOT_POWER.h>

void Power_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

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

void WIFI_Enable() {
	HAL_GPIO_WritePin(WIFI_PW_EN_PORT, WIFI_PW_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(WIFI_PW_EN_PORT, WIFI_PW_EN_PIN, GPIO_PIN_SET);
}


void Logic_Enable() {
	HAL_GPIO_WritePin(Logic_PW_EN_PORT, Logic_PW_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(Logic_PW_EN_PORT, Logic_PW_EN_PIN, GPIO_PIN_SET);
}

void RF_T_Enable() {
	HAL_GPIO_WritePin(RF_T_EN_PORT, RF_T_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(RF_T_EN_PORT, RF_T_EN_PIN, GPIO_PIN_SET);
}

void RF_R_Enable() {
	HAL_GPIO_WritePin(RF_R_EN_PORT, RF_R_EN_PIN, GPIO_PIN_RESET); //WIFI pin init 0
	task_sleepms(1);
	HAL_GPIO_WritePin(RF_R_EN_PORT, RF_R_EN_PIN, GPIO_PIN_SET);
}

#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL == 1) */
