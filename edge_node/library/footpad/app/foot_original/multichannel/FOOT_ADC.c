/*
 * FOOT_ADC.c
 *
 *  Created on: 2020. 9. 1.
 *      Author: Heo
 */

#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL == 1)

#include <FOOT_ADC.h>

ADC_HandleTypeDef AdcHandle;
uint16_t adcVal[2];
short adc_col,adc_row;

void Mux_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

	//LEFT MUX Init
	Mux_Left_S0_ENABLE();
	Mux_Left_S1_ENABLE();
	Mux_Left_S2_ENABLE();
	Mux_Left_S3_ENABLE();
	Mux_Left_Mux_1_EN_ENABLE();
	Mux_Left_Mux_2_EN_ENABLE();
	Mux_Left_Mux_3_EN_ENABLE();
	//RIGHT MUX Init
	Mux_Right_S0_ENABLE();
	Mux_Right_S1_ENABLE();
	Mux_Right_S2_ENABLE();
	Mux_Right_S3_ENABLE();
	Mux_Right_Mux_1_EN_ENABLE();
	Mux_Right_Mux_2_EN_ENABLE();
	Mux_Right_Mux_3_EN_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	//left
	GPIO_InitStruct.Pin = Mux_Left_S0_PIN;
	HAL_GPIO_Init(Mux_Left_S0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_S1_PIN;
	HAL_GPIO_Init(Mux_Left_S1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_S2_PIN;
	HAL_GPIO_Init(Mux_Left_S2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_S3_PIN;
	HAL_GPIO_Init(Mux_Left_S3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_Mux_1_EN_PIN;
	HAL_GPIO_Init(Mux_Left_Mux_1_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_Mux_2_EN_PIN;
	HAL_GPIO_Init(Mux_Left_Mux_2_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_Mux_3_EN_PIN;
	HAL_GPIO_Init(Mux_Left_Mux_3_EN_PORT, &GPIO_InitStruct);

	//right
	GPIO_InitStruct.Pin = Mux_Right_S0_PIN;
	HAL_GPIO_Init(Mux_Right_S0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_S1_PIN;
	HAL_GPIO_Init(Mux_Right_S1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_S2_PIN;
	HAL_GPIO_Init(Mux_Right_S2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_S3_PIN;
	HAL_GPIO_Init(Mux_Right_S3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_Mux_1_EN_PIN;
	HAL_GPIO_Init(Mux_Right_Mux_1_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_Mux_2_EN_PIN;
	HAL_GPIO_Init(Mux_Right_Mux_2_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_Mux_3_EN_PIN;
	HAL_GPIO_Init(Mux_Right_Mux_3_EN_PORT, &GPIO_InitStruct);

	//Init all Left pin
	HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);

	//Init all Left pin
	HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
	task_sleepms(1);
}

void Mux_s0(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_RESET);
	}
}

void Mux_s1(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_RESET);
	}
}

void Mux_s2(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_RESET);
	}
}

void Mux_s3(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_RESET);
	}
}

void Mux_Open_by_Pin(int num) {
	short binary[4] = { 0, };
	for (short i = 0; i < 4; i++) {
		binary[i] = num % 2;
		num /= 2;
	}
	Mux_s0(binary[0]);
	Mux_s1(binary[1]);
	Mux_s2(binary[2]);
	Mux_s3(binary[3]);
	bsp_busywait(UBINOS__BSP__BUSYWAITCOUNT_PER_MS/1000); //busy_wait 1us
}
void Enable_Mux(short port){
	if (port == 1) {
							//left
							HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
							//right
							HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
							bsp_busywait(UBINOS__BSP__BUSYWAITCOUNT_PER_MS/1000); //busy_wait 1us
						}
	else if (port == 2) {
							//left
							HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
							//right
							HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
							bsp_busywait(UBINOS__BSP__BUSYWAITCOUNT_PER_MS/1000); //busy_wait 1us
						}
	else if (port == 3) {
							//left
							HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);
							//right
							HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
							HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
							bsp_busywait(UBINOS__BSP__BUSYWAITCOUNT_PER_MS/1000); //busy_wait 1us
						}
}
void Read_Mux_Data(short Column) {
	for (short Row = 0; Row < 48; Row++) {
		printf("%d\n", Row);
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);
			adc_col=Column,adc_row=Row;
		} else if (Row / 16 == 1) {
			//First Mux Operate
			if (Row == 16) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);
			adc_col=Column,adc_row=Row;
		} else if (Row / 16 == 2) {
			//First Mux Operate
			if (Row == 32) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);
			adc_col=Column,adc_row=Row;
		} else {
			printf("The mux number is too high\n");
		}
	}
}

#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL == 1) */
