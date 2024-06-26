#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL != 1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <FOOT_LED.h>
//#include "FOOT_REGISTER.h"
#include <FOOT_REGISTER_LEFT.h>
#include <FOOT_REGISTER_RIGHT.h>
#include <FOOT_POWER.h>
#include <FOOT_ADC.h>
//#include "FOOT_ADC_LEFT.h"
//#include "FOOT_ADC_RIGHT.h"
#include <FOOT_WIFI.h>

static void rootfunc(void *arg);

static void helloworld_mt_task1func(void *arg);
static void helloworld_mt_task2func(void *arg);
static void helloworld_mt_task3func(void *arg);



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

	printf("\n\n\n\r");
	printf("================================================================================\n\r");
	printf("Food Pad (build time: %s %s)\n\r", __TIME__, __DATE__);
	printf("================================================================================\n\r");
	printf("\n\r");

	HAL_Init();

	Led_Init();
	Power_Init();
	Mux_Init();
//	Shift_Register_Init();
	Shift_Register_Left_Init();
	Shift_Register_Right_Init();
	Logic_Enable();
	RF_T_Enable();
	RF_R_Enable();




	srand(time(NULL));

	r = task_create(NULL, helloworld_mt_task1func, NULL, task_getmiddlepriority(), 0, "task1");
	if (0 != r) {
		logme("fail at task_create func1\r\n");
	}

	r = task_create(NULL, helloworld_mt_task2func, NULL, task_getmiddlepriority(), 0, "task2");
	if (0 != r) {
		logme("fail at task_create func2\r\n");
	}

	r = task_create(NULL, helloworld_mt_task3func, NULL, task_gethighestpriority(), 0, "task3");
	if (0 != r) {
		logme("fail at task_create func3\r\n");
	}


}


static void helloworld_mt_task1func(void *arg) {
	int r;
	unsigned int delayms;

	(void) r;

	printf("\n\r");

	for (unsigned int i = 0;; i++) {
		delayms = (rand() % 10 + 1) * 200;
		printf("1: hello world ! (%u) (delay = %4d ms)\n\r", i, delayms);
//		Led_Turn_on_1s("RED");
		task_sleepms(1);
		//LED_Turn_On("RED");
		//HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
	}
}

static void helloworld_mt_task2func(void *arg) {
	printf("\n\r");
	//WIFI_Enable();
	Wifi_Init();
	uint8_t test[2];
	if (HAL_UART_Transmit(&Uart_WIFI, (uint8_t*) 65, 1, 1000) != HAL_OK) {
		printf("error");
		/* Transfer error in transmission process */
		//Error_Handler();
	}
	if (HAL_UART_Receive(&Uart_WIFI, (uint8_t*) test, 2, 10000) != HAL_OK) {
		printf("error");
		/* Transfer error in transmission process */
		//  Error_Handler();
	}
}
static void helloworld_mt_task3func(void *arg) {
	Led_Turn_on("BLUE");
	Logic_Enable();
	printf("\n\r");
	//all Shift_Register Init to 0
	HAL_ADC_Start_IT(&AdcHandle);
	HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)adcVal, 2);

	for (unsigned int i = 0; i < 48; i++) {
		Read_Data_to_Left_Shift_Register(0);
		Read_Data_to_Right_Shift_Register(0);
		Shift_to_Left_Shift_Register();
		Shift_to_Right_Shift_Register();
	}
	for (int count = 0; count < 100; count++) {
		Init_Foot_Pad();
		Read_Data_to_Left_Shift_Register(1);
		Read_Data_to_Right_Shift_Register(1);
		printf("Shift Register Data Come in\n");
		for (unsigned int i = 0; i < 48; i++) {
			//left side read
			Shift_to_Left_Shift_Register();
			Shift_to_Right_Shift_Register();
			Read_Mux_Data(i);
			Read_Data_to_Left_Shift_Register(0);
			Read_Data_to_Right_Shift_Register(0);
			//printf("Shift Register Save 0\n");
		}
		Print_Foot_Pad();
	}
	Led_Turn_off("BLUE");
}

//static void helloworld_mt_task1func(void *arg) {
//	int r, r2, r3;
//	task_pt mux_task[3] = { NULL, NULL, NULL };
//
//	Led_Turn_on("BLUE");
//	Logic_Enable();
//	printf("\n\r");
//	//all Shift_Register Init to 0
//	for (unsigned int i = 0; i < 48; i++) {
//		Read_Data_to_Shift_Register(0);
//		Shift_to_Shift_Register();
//	}
//	Init_Foot_Pad();
//	for (int count = 0; count < 100; count++) {
//		Read_Data_to_Shift_Register(1);
//		printf("Shift Register Data Come in\n");
//		for (unsigned int i = 0; i < 48; i++) {
//			//left side read
//			Shift_to_Shift_Register();
//
//			r = task_create(&mux_task[0], Read_Mux_Data_task1, (void*) i, task_getmiddlepriority() + 1, 0, "muxtask1");
//			if (0 != r) {
//				Led_Turn_on("LED");
//			}
//			r = task_create(&mux_task[1], Read_Mux_Data_task2, (void*) i, task_getmiddlepriority() + 1, 0, "muxtask2");
//			if (0 != r) {
//				Led_Turn_on("LED");
//			}
//			r = task_create(&mux_task[2], Read_Mux_Data_task3, (void*) i, task_getmiddlepriority() + 1, 0, "muxtask3");
//			if (0 != r) {
//				Led_Turn_on("LED");
//			}
//
//			r2 = task_join(mux_task, NULL, 3);
//			if (0 != r2) {
//				Led_Turn_on("LED");
//			}
//			task_sleep(1);
//			Read_Data_to_Shift_Register(0);
//			//printf("Shift Register Save 0\n");
//		}
//		Print_Foot_Pad();
//	}
//	Led_Turn_off("BLUE");
//}

#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL != 1) */
