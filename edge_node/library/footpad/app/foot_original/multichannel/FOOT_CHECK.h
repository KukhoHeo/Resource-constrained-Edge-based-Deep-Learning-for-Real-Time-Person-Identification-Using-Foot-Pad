/*
 * FOOT_CHECK.h
 *
 *  Created on: 2021. 7. 2.
 *      Author: Heo
 */
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <FOOT_HARD_ll.h>
struct COG{
	int first_x;
	int first_y;
	int second_x;
	int second_y;
};

struct TOE{
	int first_x;
	int first_y;
	int first_sum;
	int second_x;
	int second_y;
	int second_sum;
};

struct TWO_COG{
	int first_x;
	int first_y;
	int second_x;
	int second_y;
	int third_x;
	int third_y;
	int fourth_x;
	int fourth_y;
	int foot_count;
};
struct FOOT{
	int whole_width;
	struct COG cog;
	int toe_x;
	int toe_y;
	float data_x_avg;
	float data_y_avg;
};
struct TWO_FOOT{
	struct FOOT right;
	struct FOOT left;
	bool overlap;
};
int Add_3x3(int x, int y);
struct TOE Is_TOE(int current_x, int current_y, struct TOE toe);
int Check_Width(int x, int y, int hope);
int Check_Weight(int x, int y, int hope);
int Find_Near_TOE(int current_x,int current_y, struct TOE toe,int hope);
int Find_Near_COG2(int current_x,int current_y, struct TWO_COG cog,int hope);
struct TWO_COG Is_COG_2(int current_x, int current_y,struct TWO_COG cog);
char Check_Foot_2();
struct TWO_FOOT Separate_Each_Foot(struct TWO_COG cog, struct TOE toe, struct TWO_FOOT two_foot);
struct TWO_COG Assemble_COG_and_TOE(struct TWO_COG, struct TOE);
double Calculate_Distance(int fitst_x,int first_y, int second_x, int second_y);
int Check_Angle(struct COG cog,struct TOE toe);
void Swap(int* a, int* b);
void Init_Foot_Pad(int);
void Print_Foot_Pad(char[48][96]);
void Print_Foot_Pad_s(char[48][48]);
int Line_Rotal_Least_Squares(char *, char *, struct FOOT);
int Get_Degree(float a, float b);
bool Rotate_Foot(int angle,struct FOOT foot,char side);
bool Interpolation(int angle,struct FOOT foot, char side);

