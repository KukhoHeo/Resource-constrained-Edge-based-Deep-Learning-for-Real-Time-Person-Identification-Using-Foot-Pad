/*
 * FOOT_REGISTER.h
 *
 *  Created on: 2020. 10. 4.
 *      Author: Heo
 */

#ifndef APP_FOOT_REGISTER_H_
#define APP_FOOT_REGISTER_H_


#include <FOOT_HARD_ll.h>
#include <ubinos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void Shift_Register_Init();
void Read_Data_to_Shift_Register(int );
void Shift_to_Shift_Register();
void Read_Data_to_Shift_Register_1();
void Read_Data_to_Shift_Register_0();

#endif /* APP_FOOT_REGISTER_H_ */
