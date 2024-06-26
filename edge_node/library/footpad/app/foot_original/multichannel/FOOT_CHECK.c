/*
 * FOOT_CHECK.c
 *
 *  Created on: 2021. 7. 2.
 *      Author: Heo
 */

#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL == 1)

#include <FOOT_CHECK.h>
#define WIDTH_HOPE 3
#define NEAR_HOPE 7
#define MIN_VALUE 3
#define DELTA 1e-4
#define FOOT_BOUND 5
#define MIN_VALUE_COG 30
#define RATIO_COG_DISTANCE 5

#define DATA_SIZE 700

static char data_x[2][DATA_SIZE];
static char data_y[2][DATA_SIZE];
static float temp_data_x[DATA_SIZE];
static float temp_data_y[DATA_SIZE];

int Add_3x3(int x, int y)
{
	int number = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if ((-1 < x + i && x + i < 48) && (-1 < y + j && y + j < 96))
			{
				number += foot_array[x + i][y + j];
			}
		}
	}
	return number;
}

struct TOE Is_TOE(int current_x, int current_y, struct TOE toe)
{
	int sum = 0;
	int temp_add = 0, zero_cnt = 0;
	int near = 0;
	for (int i = -2; i < 3; i += 2)
	{
		for (int j = -2; j < 3; j += 2)
		{
			temp_add = Add_3x3(current_x + i, current_y + j);
			if (temp_add <= 10)
				return toe;
			else
				sum += temp_add;
		}
	}
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (foot_array[current_x + i][current_y + j] == 0)
				zero_cnt++;
		}
	}
	if (zero_cnt < 2)
		return toe;
	if (toe.first_x == 0 && toe.first_y == 0)
	{
		toe.first_x = current_x;
		toe.first_y = current_y;
		toe.first_sum = sum;
		return toe;
	}
	near = Find_Near_TOE(current_x, current_y, toe, NEAR_HOPE);
	switch (near)
	{
	case 3:
		toe.second_x = current_x;
		toe.second_y = current_y;
		toe.second_sum = sum;
		break;
	case 2:
		if (toe.second_sum < sum)
		{
			toe.second_x = current_x;
			toe.second_y = current_y;
			toe.second_sum = sum;
		}
		break;
	case 1:
		if (toe.first_sum < sum)
		{
			toe.first_x = current_x;
			toe.first_y = current_y;
			toe.first_sum = sum;
		}
		break;
	}
	return toe;
}

int Check_Width(int x, int y, int hope)
{
	int width = 0;
	for (int i = x - hope; i <= x + hope; i++)
	{
		for (int j = y - hope; j <= y + hope; j++)
		{
			if ((0 > i || i > 47) || (0 > j || j > 95))
			{
				continue;
			}
			if (foot_array[i][j] != 0)
				width += 1;
		}
	}
	return width;
}

int Check_Weight(int x, int y, int hope)
{
	int weight = 0;
	for (int i = x - hope; i <= x + hope; i++)
	{
		for (int j = y - hope; j <= y + hope; j++)
		{
			if ((0 > i || i > 47) || (0 > j || j > 95))
			{
				continue;
			}
			if (foot_array[i][j] != 0)
				weight += foot_array[i][j];
		}
	}
	return weight;
}

int Find_Near_TOE(int current_x, int current_y, struct TOE toe, int hope)
{
	if (toe.first_x - hope <= current_x && current_x <= toe.first_x + hope && toe.first_y - hope <= current_y && current_y <= toe.first_y + hope)
	{
		return 1;
	}
	else if (toe.second_x - hope <= current_x && current_x <= toe.second_x + hope && toe.second_y - hope <= current_y && current_y <= toe.second_y + hope)
	{
		return 2;
	}
	return 3;
}

int Find_Near_COG_2(int current_x, int current_y, struct TWO_COG cog, int hope)
{
	if (cog.first_x - hope <= current_x && current_x <= cog.first_x + hope && cog.first_y - hope <= current_y && current_y <= cog.first_y + hope)
	{
		return 1;
	}
	else if (cog.second_x - hope <= current_x && current_x <= cog.second_x + hope && cog.second_y - hope <= current_y && current_y <= cog.second_y + hope)
	{
		return 2;
	}
	else if (cog.third_x - hope <= current_x && current_x <= cog.third_x + hope && cog.third_y - hope <= current_y && current_y <= cog.third_y + hope)
	{
		return 3;
	}
	else if (cog.fourth_x - hope <= current_x && current_x <= cog.fourth_x + hope && cog.fourth_y - hope <= current_y && current_y <= cog.fourth_y + hope)
	{
		return 4;
	}
	return 5;
}

struct TWO_COG Is_COG_2(int current_x, int current_y, struct TWO_COG cog)
{
	int near;
	if (Check_Width(current_x, current_y, WIDTH_HOPE) >= MIN_VALUE_COG)
	{
		if (cog.first_x == 0 && cog.first_y == 0)
		{
			cog.first_x = current_x;
			cog.first_y = current_y;
			return cog;
		}
		near = Find_Near_COG_2(current_x, current_y, cog, NEAR_HOPE);
		switch (near)
		{
		case 5:
			if (cog.second_x == 0 && cog.second_y == 0)
			{
				cog.second_x = current_x;
				cog.second_y = current_y;
			}
			else if (cog.third_x == 0 && cog.third_y == 0)
			{
				cog.third_x = current_x;
				cog.third_y = current_y;
			}
			else if (cog.fourth_x == 0 && cog.fourth_y == 0)
			{
				cog.fourth_x = current_x;
				cog.fourth_y = current_y;
			}
			break;

		case 4:
			if (Check_Width(cog.fourth_x, cog.fourth_y, WIDTH_HOPE) < Check_Width(current_x, current_y, WIDTH_HOPE))
			{
				cog.fourth_x = current_x;
				cog.fourth_y = current_y;
			}
			else if ((Check_Width(cog.fourth_x, cog.fourth_y, WIDTH_HOPE) == Check_Width(current_x, current_y, WIDTH_HOPE)) && (Check_Weight(cog.fourth_x, cog.fourth_y, WIDTH_HOPE) < Check_Weight(current_x, current_y, WIDTH_HOPE)))
			{
				cog.fourth_x = current_x;
				cog.fourth_y = current_y;
			}
			break;

		case 3:
			if (Check_Width(cog.third_x, cog.third_y, WIDTH_HOPE) < Check_Width(current_x, current_y, WIDTH_HOPE))
			{
				cog.third_x = current_x;
				cog.third_y = current_y;
			}
			else if ((Check_Width(cog.third_x, cog.third_y, WIDTH_HOPE) == Check_Width(current_x, current_y, WIDTH_HOPE)) && (Check_Weight(cog.third_x, cog.third_y, WIDTH_HOPE) < Check_Weight(current_x, current_y, WIDTH_HOPE)))
			{
				cog.third_x = current_x;
				cog.third_y = current_y;
			}
			break;

		case 2:
			if (Check_Width(cog.second_x, cog.second_y, WIDTH_HOPE) < Check_Width(current_x, current_y, WIDTH_HOPE))
			{
				cog.second_x = current_x;
				cog.second_y = current_y;
			}
			else if ((Check_Width(cog.second_x, cog.second_y, WIDTH_HOPE) == Check_Width(current_x, current_y, WIDTH_HOPE)) && (Check_Weight(cog.second_x, cog.second_y, WIDTH_HOPE) < Check_Weight(current_x, current_y, WIDTH_HOPE)))
			{
				cog.second_x = current_x;
				cog.second_y = current_y;
			}
			break;
		case 1:
			if (Check_Width(cog.first_x, cog.first_y, WIDTH_HOPE) < Check_Width(current_x, current_y, WIDTH_HOPE))
			{
				cog.first_x = current_x;
				cog.first_y = current_y;
			}
			else if ((Check_Width(cog.first_x, cog.first_y, WIDTH_HOPE) == Check_Width(current_x, current_y, WIDTH_HOPE)) && (Check_Weight(cog.first_x, cog.first_y, WIDTH_HOPE) < Check_Weight(current_x, current_y, WIDTH_HOPE)))
			{
				cog.first_x = current_x;
				cog.first_y = current_y;
			}
			break;
		}
	}
	return cog;
}

char Check_Foot_2()
{
	struct TWO_COG cog;
	struct TOE toe;
	struct TWO_FOOT two_foot;
	double distance1, distance2;

	// int whole_width = 0;

	memset(&cog, 0, sizeof(struct TWO_COG));
	memset(&toe, 0, sizeof(struct TOE));
	memset(&two_foot, 0, sizeof(struct TWO_FOOT));
	memset(temp_left_image, 0, sizeof(temp_left_image));
	memset(temp_right_image, 0, sizeof(temp_right_image));
	memset(left_image, 0, sizeof(left_image));
	memset(right_image, 0, sizeof(right_image));

	// find candidate
	for (int i = 0; i < 48; i++)
	{
		for (int j = 0; j < 96; j++)
		{
			if (foot_array[i][j] != 0)
			{
				cog = Is_COG_2(i, j, cog);
			}
			else if (foot_array[i][j] == 0)
			{
				toe = Is_TOE(i, j, toe);
			}
		}
	}

	// match candidate
	cog = Assemble_COG_and_TOE(cog, toe);
	printf("assemble\n");
	// foot_count 3 is error signal
	if (cog.foot_count == 3)
		return 'n';
	printf("cogcount is not 3\n");
	//	//test code
	//	if(cog.foot_count ==1){
	//
	//		distance1 = Calculate_Distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y);
	//		if(100>distance1 * 5){
	//			printf("Foot\n");
	//			return true;
	//		}
	//		else{
	//			return false;
	//		}
	//	}
	//	else if(cog.foot_count==2){
	//
	//		distance1 = Calculate_Distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y);
	//		distance2 = Calculate_Distance(cog.third_x, cog.third_y, cog.fourth_x, cog.fourth_y);
	//		if(100>distance1 * 5&&100>distance2 * 5){
	//			printf("Feet\n");
	//			return true;
	//		}
	//		else{
	//			return false;
	//		}
	//	}
	//	//////

	// separate
	two_foot = Separate_Each_Foot(cog, toe, two_foot);
	printf("Separate_Each_Foot\n");
	// two_foot.overlap = false is error signal
	if (two_foot.overlap == false)
		return 'n';
	printf("two_foot is not overlap\n");
	printf("fitst foot width %d, second foot width %d sum %d\n", two_foot.left.whole_width, two_foot.right.whole_width, two_foot.left.whole_width + two_foot.right.whole_width);
	distance1 = Calculate_Distance(two_foot.left.cog.first_x, two_foot.left.cog.first_y, two_foot.left.cog.second_x, two_foot.left.cog.second_y);
	distance2 = Calculate_Distance(two_foot.right.cog.first_x, two_foot.right.cog.first_y, two_foot.right.cog.second_x, two_foot.right.cog.second_y);
	printf("distance1 %f, distance2 %f\n", distance1, distance2);
	printf("two_foot.left.whole_width %d two_foot.right.whole_width %d distance1 * RATIO_COG_DISTANCE %f distance2 * RATIO_COG_DISTANCE %f\n", two_foot.left.whole_width, two_foot.right.whole_width, distance1 * RATIO_COG_DISTANCE, distance2 * RATIO_COG_DISTANCE);
	if (distance1 < 10 || distance2 < 10)
	{
		printf("distance too short\n");
		return 'n';
	}
	if (cog.foot_count == 1)
	{
		if (two_foot.left.whole_width > distance1 * RATIO_COG_DISTANCE)
		{
			printf("left foot ok\n");
			int angle = Line_Rotal_Least_Squares(data_x[0], data_y[0], two_foot.left);
			printf("left foot angle ok\n");
#if (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1)
			printf("======================\n");
			printf("angle %d\n", angle);
			printf("======================\n");
#endif /* (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1) */

			if (Rotate_Foot(angle, two_foot.left, 'l'))
			{
				printf("left foot rotate ok\n");
			}
			else
			{
				printf("left foot rotate no\n");
				return 'n';
			}
			if (Interpolation(angle, two_foot.left, 'l'))
			{
				printf("left foot interpolation ok\n");
			}
			else
			{
				printf("left foot interpolation no\n");
				return 'n';
			}
			return 'l';
		}
		else if (two_foot.right.whole_width > distance2 * RATIO_COG_DISTANCE)
		{
			printf("right foot ok\n");
			int angle = Line_Rotal_Least_Squares(data_x[1], data_y[1], two_foot.right);
			printf("right foot angle ok\n");
#if (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1)
			printf("======================\n");
			printf("angle %d\n", angle);
			printf("======================\n");
#endif /* (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1) */
			if (Rotate_Foot(angle, two_foot.right, 'r'))
			{
				printf("right foot rotate ok\n");
			}
			else
			{
				printf("right foot rotate no\n");
				return 'n';
			}
			if (Interpolation(angle, two_foot.right, 'r'))
			{
				printf("right foot interpolation ok\n");
			}
			else
			{
				printf("right foot interpolation no\n");
				return 'n';
			}
			return 'r';
		}
	}
	else if (cog.foot_count == 2)
	{
		char side;
		if (two_foot.left.whole_width > distance1 * RATIO_COG_DISTANCE)
		{
			printf("left foot ok\n");
			int angle = Line_Rotal_Least_Squares(data_x[0], data_y[0], two_foot.left);
			printf("left foot angle ok\n");
#if (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1)
			printf("======================\n");
			printf("angle %d\n", angle);
			printf("======================\n");
			Print_Foot_Pad(temp_left_image);
#endif /* (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1) */
			if (Rotate_Foot(angle, two_foot.left, 'l'))
			{
				printf("left foot rotate ok\n");
			}
			else
			{
				printf("left foot rotate no\n");
				return false;
			}
			Print_Foot_Pad_s(left_image);
			if (Interpolation(angle, two_foot.left, 'l'))
			{
				printf("left foot interpolation ok\n");
			}
			else
			{
				printf("left foot interpolation no\n");
				return false;
			}
			Print_Foot_Pad_s(left_image);
			side = 'l';
		}
		if (two_foot.right.whole_width > distance2 * RATIO_COG_DISTANCE)
		{
			printf("right foot ok\n");
			int angle = Line_Rotal_Least_Squares(data_x[1], data_y[1], two_foot.right);
			printf("right foot angle ok\n");
#if (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1)
			printf("======================\n");
			printf("angle %d\n", angle);
			printf("======================\n");
#endif /* (FOOT_ORIGINAL__USE_PRINT_IMAGE == 1) */
			if (Rotate_Foot(angle, two_foot.right, 'r'))
			{
				printf("right foot rotate ok\n");
			}
			else
			{
				printf("right foot rotate no\n");
				return false;
			}
			if (Interpolation(angle, two_foot.right, 'r'))
			{
				printf("right foot interpolation ok\n");
			}
			else
			{
				printf("right foot interpolation no\n");
				return false;
			}
			if (side == 'l')
			{
				side = 'b';
			}
			else
			{
				side = 'r';
			}
		}
		return side;
	}
	else
	{
		return 'n';
	}
	return 'n';
}
struct TWO_FOOT Separate_Each_Foot(struct TWO_COG cog, struct TOE toe, struct TWO_FOOT two_foot)
{
	double a, b, c;
	//	double aa, bb, cc;
	int x, y;
	int height, low, left, right;
	int left_sum_data_x = 0, right_sum_data_x = 0;
	int left_sum_data_y = 0, right_sum_data_y = 0;
	if (cog.foot_count == 1)
	{
		// 4cog
		if (cog.fourth_x != 0 && cog.fourth_y != 0 && cog.second_x != 0 && cog.second_y != 0)
		{
			// printf("all cog lint between toe1 and cog_third\n");
			// lint between toe1 and cog_third
			a = (toe.first_x - cog.third_x) / (toe.first_y - cog.third_y + DELTA);
			b = 1;
			x = (toe.first_x + cog.third_x) / 2;
			y = (toe.first_y + cog.third_y) / 2;
			c = -(a * x + b * y);
		}

		// 3cog
		else if (cog.third_x != 0 && cog.third_y != 0)
		{
			// 0 degree near toe or 180 degree near anywhere
			if (cog.fourth_x == 0 && cog.fourth_y == 0)
			{
				// near toe
				if (toe.second_x == 0 && toe.second_y == 0)
				{
					// printf("near toe line between toe1 and cog_third\n");
					// lint between toe1 and cog_third
					a = (toe.first_x - cog.third_x) / (toe.first_y - cog.third_y + DELTA);
					b = 1;
					x = (toe.first_x + cog.third_x) / 2;
					y = (toe.first_y + cog.third_y) / 2;
					c = -(a * x + b * y);
				}
				// 180 degree near heal
				else
				{
					// printf("near heal line between toe2 and cog_second\n");
					// lint between toe2 and cog_second
					a = (toe.second_x - cog.second_x) / (toe.second_y - cog.second_y + DELTA);
					b = 1;
					x = (toe.second_x + cog.second_x) / 2;
					y = (toe.second_y + cog.second_y) / 2;
					c = -(a * x + b * y);
				}
			}
			// 0 degree near heal
			else if (cog.second_x == 0 && cog.second_y == 0)
			{
				// printf("near heal line between toe1 and cog_fourth\n");
				// lint between toe1 and cog_fourth
				a = (toe.first_x - cog.fourth_x) / (toe.first_y - cog.fourth_y + DELTA);
				b = 1;
				x = (toe.first_x + cog.fourth_x) / 2;
				y = (toe.first_y + cog.fourth_y) / 2;
				c = -(a * x + b * y);
			}
		}
		// 2cog
		else
		{
			two_foot.left.toe_x = toe.first_x;
			two_foot.left.toe_y = toe.first_y;
			two_foot.left.cog.first_x = cog.first_x;
			two_foot.left.cog.first_y = cog.first_y;
			two_foot.left.cog.second_x = cog.second_x;
			two_foot.left.cog.second_y = cog.second_y;
			if (toe.first_x > cog.second_x)
			{
				if (toe.first_y > cog.second_y)
				{
					height = toe.first_x + FOOT_BOUND;
					right = toe.first_y + FOOT_BOUND;
					low = cog.second_x - FOOT_BOUND;
					left = cog.second_y - FOOT_BOUND;
					if (height > 47)
					{
						height = 47;
					}
					if (right > 95)
					{
						right = 95;
					}
					if (low < 0)
					{
						low = 0;
					}
					if (left < 0)
					{
						left = 0;
					}
				}
				else
				{
					height = toe.first_x + FOOT_BOUND;
					left = toe.first_y - FOOT_BOUND;
					low = cog.second_x - FOOT_BOUND;
					right = cog.second_y + FOOT_BOUND;
					if (height > 47)
					{
						height = 47;
					}
					if (right > 95)
					{
						right = 95;
					}
					if (low < 0)
					{
						low = 0;
					}
					if (left < 0)
					{
						left = 0;
					}
				}
			}
			else
			{
				if (toe.first_y > cog.second_y)
				{
					low = toe.first_x - FOOT_BOUND;
					right = toe.first_y + FOOT_BOUND;
					height = cog.second_x + FOOT_BOUND;
					left = cog.second_y - FOOT_BOUND;
					if (height > 47)
					{
						height = 47;
					}
					if (right > 95)
					{
						right = 95;
					}
					if (low < 0)
					{
						low = 0;
					}
					if (left < 0)
					{
						left = 0;
					}
				}
				else
				{
					low = toe.first_x - FOOT_BOUND;
					left = toe.first_y - FOOT_BOUND;
					height = cog.second_x + FOOT_BOUND;
					right = cog.second_y + FOOT_BOUND;
					if (height > 47)
					{
						height = 47;
					}
					if (right > 95)
					{
						right = 95;
					}
					if (low < 0)
					{
						low = 0;
					}
					if (left < 0)
					{
						left = 0;
					}
				}
			}
			for (int i = low; i < height; i++)
			{
				for (int j = left; j < right; j++)
				{
					if (foot_array[i][j] > MIN_VALUE)
					{
						temp_left_image[i][j] = foot_array[i][j];
						data_x[0][two_foot.left.whole_width] = j;
						data_y[0][two_foot.left.whole_width] = i;
						left_sum_data_x += j;
						left_sum_data_y += i;
						two_foot.left.whole_width++;
					}
				}
			}
			two_foot.left.data_x_avg = ((float)left_sum_data_x / (float)two_foot.left.whole_width);
			two_foot.left.data_y_avg = ((float)left_sum_data_y / (float)two_foot.left.whole_width);
			two_foot.overlap = true;
			return two_foot;
		}
		if (a * cog.first_x + b * cog.first_y + c < 0)
		{
			two_foot.left.toe_x = toe.first_x;
			two_foot.left.toe_y = toe.first_y;
			two_foot.left.cog.first_x = cog.first_x;
			two_foot.left.cog.first_y = cog.first_y;
			two_foot.left.cog.second_x = cog.second_x;
			two_foot.left.cog.second_y = cog.second_y;
		}
		else
		{
			two_foot.right.toe_x = toe.first_x;
			two_foot.right.toe_y = toe.first_y;
			two_foot.right.cog.first_x = cog.first_x;
			two_foot.right.cog.first_y = cog.first_y;
			two_foot.right.cog.second_x = cog.second_x;
			two_foot.right.cog.second_y = cog.second_y;
		}

		if (a * cog.third_x + b * cog.third_y + c < 0)
		{
			two_foot.left.toe_x = toe.second_x;
			two_foot.left.toe_y = toe.second_y;
			two_foot.left.cog.first_x = cog.third_x;
			two_foot.left.cog.first_y = cog.third_y;
			two_foot.left.cog.second_x = cog.fourth_x;
			two_foot.left.cog.second_y = cog.fourth_y;
		}
		else
		{
			two_foot.right.toe_x = toe.second_x;
			two_foot.right.toe_y = toe.second_y;
			two_foot.right.cog.first_x = cog.third_x;
			two_foot.right.cog.first_y = cog.third_y;
			two_foot.right.cog.second_x = cog.fourth_x;
			two_foot.right.cog.second_y = cog.fourth_y;
		}
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 96; j++)
			{
				if (foot_array[i][j] > MIN_VALUE)
				{
					if (-1 < a * i + b * j + c && a * i + b * j + c < 1)
					{
						two_foot.overlap = false;
						return two_foot;
					}
					else if (a * i + b * j + c < 0)
					{
						temp_left_image[i][j] = foot_array[i][j];
						data_x[0][two_foot.left.whole_width] = j;
						data_y[0][two_foot.left.whole_width] = i;
						left_sum_data_x += j;
						left_sum_data_y += i;
						two_foot.left.whole_width++;
					}
					else
					{
						temp_right_image[i][j] = foot_array[i][j];
						data_x[1][two_foot.right.whole_width] = j;
						data_y[1][two_foot.right.whole_width] = i;
						right_sum_data_x += j;
						right_sum_data_y += i;
						two_foot.right.whole_width++;
					}
				}
			}
		}
		two_foot.left.data_x_avg = ((float)left_sum_data_x / (float)two_foot.left.whole_width);
		two_foot.left.data_y_avg = ((float)left_sum_data_y / (float)two_foot.left.whole_width);
		two_foot.right.data_x_avg = ((float)right_sum_data_x / (float)two_foot.right.whole_width);
		two_foot.right.data_y_avg = ((float)right_sum_data_y / (float)two_foot.right.whole_width);
	}
	// 2foot
	else if (cog.foot_count == 2)
	{
		// lint between toes
		a = (toe.first_x - toe.second_x) / (toe.first_y - toe.second_y + DELTA);
		b = 1;
		x = (cog.second_x + cog.fourth_x + cog.first_x + cog.third_x) / 4;
		y = (cog.second_y + cog.fourth_y + cog.first_y + cog.third_y) / 4;
		c = -(a * x + b * y);

		if (a * cog.first_x + b * cog.first_y + c < 0)
		{
			two_foot.left.toe_x = toe.first_x;
			two_foot.left.toe_y = toe.first_y;
			two_foot.left.cog.first_x = cog.first_x;
			two_foot.left.cog.first_y = cog.first_y;
			two_foot.left.cog.second_x = cog.second_x;
			two_foot.left.cog.second_y = cog.second_y;
		}
		else
		{
			two_foot.right.toe_x = toe.first_x;
			two_foot.right.toe_y = toe.first_y;
			two_foot.right.cog.first_x = cog.first_x;
			two_foot.right.cog.first_y = cog.first_y;
			two_foot.right.cog.second_x = cog.second_x;
			two_foot.right.cog.second_y = cog.second_y;
		}
		if (a * cog.third_x + b * cog.third_y + c < 0)
		{
			two_foot.left.toe_x = toe.second_x;
			two_foot.left.toe_y = toe.second_y;
			two_foot.left.cog.first_x = cog.third_x;
			two_foot.left.cog.first_y = cog.third_y;
			two_foot.left.cog.second_x = cog.fourth_x;
			two_foot.left.cog.second_y = cog.fourth_y;
		}
		else
		{
			two_foot.right.toe_x = toe.second_x;
			two_foot.right.toe_y = toe.second_y;
			two_foot.right.cog.first_x = cog.third_x;
			two_foot.right.cog.first_y = cog.third_y;
			two_foot.right.cog.second_x = cog.fourth_x;
			two_foot.right.cog.second_y = cog.fourth_y;
		}

		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 96; j++)
			{
				if (foot_array[i][j] > MIN_VALUE)
				{
					if (-1 < a * i + b * j + c && a * i + b * j + c < 1)
					{
						two_foot.overlap = false;
						return two_foot;
					}
					else if (a * i + b * j + c < 0)
					{
						temp_left_image[i][j] = foot_array[i][j];
						data_x[0][two_foot.left.whole_width] = j;
						data_y[0][two_foot.left.whole_width] = i;
						left_sum_data_x += j;
						left_sum_data_y += i;
						two_foot.left.whole_width++;
					}
					else
					{
						temp_right_image[i][j] = foot_array[i][j];
						data_x[1][two_foot.right.whole_width] = j;
						data_y[1][two_foot.right.whole_width] = i;
						right_sum_data_x += j;
						right_sum_data_y += i;
						two_foot.right.whole_width++;
					}
				}
			}
		}
		two_foot.left.data_x_avg = ((float)left_sum_data_x / (float)two_foot.left.whole_width);
		two_foot.left.data_y_avg = ((float)left_sum_data_y / (float)two_foot.left.whole_width);
		two_foot.right.data_x_avg = ((float)right_sum_data_x / (float)two_foot.right.whole_width);
		two_foot.right.data_y_avg = ((float)right_sum_data_y / (float)two_foot.right.whole_width);
	}
	else
	{
		two_foot.overlap = false;
		return two_foot;
	}
	two_foot.overlap = true;
	return two_foot;
}
struct TWO_COG Assemble_COG_and_TOE(struct TWO_COG cog, struct TOE toe)
{
	double min;
	double a, b, c;
	// first
	if (toe.first_x != 0 && toe.first_y != 0 && cog.first_x != 0 && cog.first_y != 0 && cog.second_x != 0 && cog.second_y != 0)
	{
		min = Calculate_Distance(toe.first_x, toe.first_y, cog.first_x, cog.first_y);
		if (min > Calculate_Distance(toe.first_x, toe.first_y, cog.second_x, cog.second_y))
		{
			Swap(&cog.first_x, &cog.second_x);
			Swap(&cog.first_y, &cog.second_y);
		}
		min = Calculate_Distance(toe.first_x, toe.first_y, cog.first_x, cog.first_y);
		if (min > Calculate_Distance(toe.first_x, toe.first_y, cog.third_x, cog.third_y))
		{
			Swap(&cog.first_x, &cog.third_x);
			Swap(&cog.first_y, &cog.third_y);
		}
		min = Calculate_Distance(toe.first_x, toe.first_y, cog.first_x, cog.first_y);
		if (min > Calculate_Distance(toe.first_x, toe.first_y, cog.fourth_x, cog.fourth_y))
		{
			Swap(&cog.first_x, &cog.fourth_x);
			Swap(&cog.first_y, &cog.fourth_y);
		}
	}
	else
		return cog;

	// second
	// if it has 4 cog
	if (toe.second_x != 0 && toe.second_y != 0 && cog.third_x != 0 && cog.third_y != 0 && cog.fourth_x != 0 && cog.fourth_y != 0)
	{
		min = Calculate_Distance(toe.second_x, toe.second_y, cog.third_x, cog.third_y);
		if (min > Calculate_Distance(toe.second_x, toe.second_y, cog.second_x, cog.second_y))
		{
			Swap(&cog.third_x, &cog.second_x);
			Swap(&cog.third_y, &cog.second_y);
		}
		min = Calculate_Distance(toe.second_x, toe.second_y, cog.third_x, cog.third_y);
		if (min > Calculate_Distance(toe.second_x, toe.second_y, cog.fourth_x, cog.fourth_y))
		{
			Swap(&cog.third_x, &cog.fourth_x);
			Swap(&cog.third_y, &cog.fourth_y);
		}
		cog.foot_count = 2;
	}
	////if it has 4 cog except 2 toe
	else if (toe.second_x == 0 && toe.second_y == 0 && cog.third_x != 0 && cog.third_y != 0 && cog.fourth_x != 0 && cog.fourth_y != 0)
	{
		cog.foot_count = 3;
		return cog;
	}
	// if it has 3 cog
	else if (cog.third_x != 0 && cog.third_y != 0)
	{
		// if it has 3cog and 2toe
		// compare second third
		if (toe.second_x != 0 && toe.second_y != 0)
		{
			min = Calculate_Distance(toe.second_x, toe.second_y, cog.third_x, cog.third_y);
			if (min > Calculate_Distance(toe.second_x, toe.second_y, cog.second_x, cog.second_y))
			{
				Swap(&cog.third_x, &cog.second_x);
				Swap(&cog.third_y, &cog.second_y);
			}
			// find second cog owner
			a = (toe.first_y - cog.first_y) / (toe.first_x - cog.first_x + DELTA);
			b = -1;
			c = toe.first_y - (a * toe.first_x);
			min = fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b);

			a = (toe.second_y - cog.third_y) / (toe.second_x - cog.third_x + DELTA);
			b = -1;
			c = toe.second_y - (a * toe.second_x);
			if ((min > (fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b))) && Calculate_Distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y) > Calculate_Distance(cog.third_x, cog.third_y, cog.second_x, cog.second_y))
			{
				Swap(&cog.second_x, &cog.fourth_x);
				Swap(&cog.second_y, &cog.fourth_y);
			}
		}
		a = (toe.first_y - cog.first_y) / (toe.first_x - cog.first_x + DELTA);
		b = -1;
		c = toe.first_y - (a * toe.first_x);
		min = fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b);
		if ((min > (fabs(a * cog.third_x + b * cog.third_y + c) / sqrt(a * a + b * b))) && Calculate_Distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y) > Calculate_Distance(cog.first_x, cog.first_y, cog.third_x, cog.third_y))
		{
			Swap(&cog.second_x, &cog.third_x);
			Swap(&cog.second_y, &cog.third_y);
		}
		cog.foot_count = 1;
		return cog;
	}
	// if it has only 2 cog
	else
	{
		cog.foot_count = 1;
		return cog;
	}
	// compare second fourth
	a = (toe.first_y - cog.first_y) / (toe.first_x - cog.first_x + DELTA);
	b = -1;
	c = toe.first_y - (a * toe.first_x);
	min = fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b);
	if ((min > (fabs(a * cog.fourth_x + b * cog.fourth_y + c) / sqrt(a * a + b * b))) && Calculate_Distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y) > Calculate_Distance(cog.first_x, cog.first_y, cog.fourth_x, cog.fourth_y))
	{
		Swap(&cog.second_x, &cog.fourth_x);
		Swap(&cog.second_y, &cog.fourth_y);
	}
	return cog;
}

double Calculate_Distance(int fitst_x, int first_y, int second_x, int second_y)
{
	if (second_x == 0 && second_y == 0)
	{
		return 9999;
	}
	return sqrt(pow(fitst_x - second_x, 2) + pow(first_y - second_y, 2));
}

int Check_Angle(struct COG cog, struct TOE toe)
{
	int angle = 0;
	double distance_first = 0, distance_second = 0;
	double delta_x = 0, delta_y = 0;
	distance_first = sqrt(pow(cog.first_x - toe.first_x, 2) + pow(cog.first_y - toe.first_y, 2));
	distance_second = sqrt(pow(cog.second_x - toe.first_x, 2) + pow(cog.second_y - toe.first_y, 2));
	if (distance_first > distance_second)
	{
		// first가 발가락과 더 멀다.
		delta_x = abs(cog.second_x - cog.first_x);
		delta_y = abs(cog.second_y - cog.first_y);
	}
	else
	{
		// second가 더 멀다
		delta_x = abs(cog.first_x - cog.second_x);
		delta_y = abs(cog.first_y - cog.second_y);
	}
	angle = atan2(delta_y, delta_x);
	return angle;
}
void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void Init_Foot_Pad(int mode)
{
	for (int row = 0; row < 48; row++)
	{
		for (int col = 0; col < 96; col++)
		{
			if (mode == 1)
			{
				if (col == 0)
					foot_array[row][col] = row;
				else
					foot_array[row][col] = col;
			}
			else if (mode == 2)
			{
				foot_array[row][col] = 0;
			}
			else if (mode == 3)
			{
				foot_array[10][10] = 1;
				foot_array[10][11] = 1;
				foot_array[10][12] = 1;
				foot_array[10][13] = 1;
				foot_array[10][14] = 1;

				foot_array[11][11] = 1;
				foot_array[11][12] = 1;
				foot_array[11][13] = 1;
				foot_array[11][14] = 1;

				foot_array[12][12] = 1;
				foot_array[12][13] = 1;
				foot_array[12][14] = 1;

				foot_array[13][13] = 1;
				foot_array[13][14] = 1;

				foot_array[14][14] = 1;
			}
		}
	}
}

void Print_Foot_Pad(char image[48][96])
{
	for (int row = 47; row >= 0; row--)
	{
		for (int col = 0; col < 96; col++)
		{
			printf("%5d", image[row][col]);
		}
		printf("\n");
	}
}

void Print_Foot_Pad_s(char image[48][48])
{
	for (int row = 47; row >= 0; row--)
	{
		for (int col = 0; col < 48; col++)
		{
			printf("%5d", image[row][col]);
		}
		printf("\n");
	}
}

int Line_Rotal_Least_Squares(char *data_x, char *data_y, struct FOOT foot)
{
	float temp_toe_x, temp_toe_y;
	float eigenvalues_1, eigenvalues_2;
	float eigenvectors_1[2], eigenvectors_2[2];
	/*(a, b)
	  (c, d)
	*/
	float a = 0, b = 0, c = 0, d = 0;
	// a_x+b_y+c_=0
	float a_ = 0, b_ = 0;
	// float c_=0;
	// angle
	int toe_angle, tls_angle_1, tls_angle_2;
	// temp angle
	int abs_angle_1, abs_angle_2;
	// move to center(0,0)
	for (int i = 0; i < foot.whole_width; i++)
	{
		temp_data_x[i] = data_x[i] - foot.data_x_avg;
		temp_data_y[i] = data_y[i] - foot.data_y_avg;
	}
	temp_toe_x = foot.toe_x - foot.data_y_avg;
	temp_toe_y = foot.toe_y - foot.data_x_avg;

	// Matrix * Matrix_T
	for (int i = 0; i < foot.whole_width; i++)
	{
		a += (temp_data_y[i] * temp_data_y[i]);
		b += (temp_data_x[i] * temp_data_y[i]);
		c += (temp_data_y[i] * temp_data_x[i]);
		d += (temp_data_x[i] * temp_data_x[i]);
	}

	// eigenvalues
	eigenvalues_1 = ((a + d) + sqrt(pow(-a - d, 2) - 4 * (a * d - b * c))) / 2;
	eigenvalues_2 = ((a + d) - sqrt(pow(-a - d, 2) - 4 * (a * d - b * c))) / 2;

	// eigenvectors
	eigenvectors_1[0] = b;
	eigenvectors_1[1] = eigenvalues_1 - a;
	eigenvectors_2[0] = b;
	eigenvectors_2[1] = eigenvalues_2 - a;

	// a_x+b_y+c_=0
	if (eigenvalues_1 <= eigenvalues_2)
	{
		a_ = eigenvectors_1[0];
		b_ = eigenvectors_1[1];
	}
	else
	{
		a_ = eigenvectors_2[0];
		b_ = eigenvectors_2[1];
	}
	// c_ = (-1 * a_*foot.data_x_avg) + (-1 * b_*foot.data_y_avg);
	toe_angle = Get_Degree(temp_toe_y, temp_toe_x);

	tls_angle_1 = Get_Degree(a_, -b_);
	tls_angle_2 = Get_Degree(-a_, b_);

	if (abs(360 - tls_angle_1 + toe_angle) > abs(360 + tls_angle_1 - toe_angle))
	{
		if (abs(360 + tls_angle_1 - toe_angle) > abs(tls_angle_1 - toe_angle))
			abs_angle_1 = abs(tls_angle_1 - toe_angle);
		else
			abs_angle_1 = abs(360 + tls_angle_1 - toe_angle);
	}
	else
	{
		if (abs(360 - tls_angle_1 + toe_angle) > abs(tls_angle_1 - toe_angle))
			abs_angle_1 = abs(tls_angle_1 - toe_angle);
		else
			abs_angle_1 = abs(360 - tls_angle_1 + toe_angle);
	}

	if (abs(360 - tls_angle_2 + toe_angle) > abs(360 + tls_angle_2 - toe_angle))
	{
		if (abs(360 + tls_angle_2 - toe_angle) > abs(tls_angle_2 - toe_angle))
			abs_angle_2 = abs(tls_angle_2 - toe_angle);
		else
			abs_angle_2 = abs(360 + tls_angle_2 - toe_angle);
	}
	else
	{
		if (abs(360 - tls_angle_2 + toe_angle) > abs(tls_angle_2 - toe_angle))
			abs_angle_2 = abs(tls_angle_2 - toe_angle);
		else
			abs_angle_2 = abs(360 - tls_angle_2 + toe_angle);
	}
	if (abs_angle_1 < abs_angle_2)
		return tls_angle_1;
	else
		return tls_angle_2;
	// printf("%dm%dm%d\n",toe_angle,tls_angle_1,tls_angle_2);
	// printf("%lfm%lfm%lf\n",a_,b_,c_);
	// printf("%lfm%lfm%lfm%lf\n",eigenvectors_1[0],eigenvectors_1[1],eigenvectors_2[0],eigenvectors_2[1]);
	// printf("%lfm%lf\n",eigenvalues_1,eigenvalues_2);
	// printf("%lfm%lf\n",temp_toe_x,temp_toe_y);
}
int Get_Degree(float a, float b)
{
	float degree;
	degree = atan2f(a, b) * 180 / M_PI;
	if (degree < 0)
	{
		degree = 360 + degree;
	}
	return (int)(round(degree));
}
bool Rotate_Foot(int angle, struct FOOT foot, char side)
{
	char ratio[48][48];
	// target angle
	int rotate_angle = 360 - angle;
	// target axis
	float rotation_x_i, rotation_y_j;
	// target value
	float rotation_x, rotation_y;
	// temp ratio for high probability axis
	float tmp_ratio;
	// round axis
	int round_rotation_x, round_rotation_y;
	// set to initial ratio
	memset(ratio, 254, sizeof(char) * 48 * 48);
	if (side == 'l')
	{
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 96; j++)
			{
				if (temp_left_image[i][j] > 0)
				{
					rotation_x_i = i - foot.data_y_avg;
					rotation_y_j = j - foot.data_x_avg;
					rotation_x = (cos((rotate_angle)*M_PI / 180) * (rotation_x_i)) - (sin((rotate_angle)*M_PI / 180) * (rotation_y_j));
					rotation_y = (sin((rotate_angle)*M_PI / 180) * (rotation_x_i)) + (cos((rotate_angle)*M_PI / 180) * (rotation_y_j));
					rotation_x = 24 + rotation_x;
					rotation_y = 24 + rotation_y;
					round_rotation_x = (lround(rotation_x));
					round_rotation_y = (lround(rotation_y));
					tmp_ratio = pow((pow((rotation_x - (int)rotation_x), 2) + pow((rotation_y - (int)rotation_y), 2)), 0.5) * 100;
					if (tmp_ratio < ratio[round_rotation_x][round_rotation_y])
					{
						left_image[round_rotation_x][round_rotation_y] = temp_left_image[i][j];
						ratio[round_rotation_x][round_rotation_y] = tmp_ratio;
					}
				}
			}
		}
	}
	else if (side == 'r')
	{
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 96; j++)
			{
				if (temp_right_image[i][j] > 0)
				{
					rotation_x_i = i - foot.data_y_avg;
					rotation_y_j = j - foot.data_x_avg;
					rotation_x = (cos((rotate_angle)*M_PI / 180) * (rotation_x_i)) - (sin((rotate_angle)*M_PI / 180) * (rotation_y_j));
					rotation_y = (sin((rotate_angle)*M_PI / 180) * (rotation_x_i)) + (cos((rotate_angle)*M_PI / 180) * (rotation_y_j));
					rotation_x = 24 + rotation_x;
					rotation_y = 24 + rotation_y;
					round_rotation_x = (lround(rotation_x));
					round_rotation_y = (lround(rotation_y));

					tmp_ratio = pow((pow((rotation_x - (int)rotation_x), 2) + pow((rotation_y - (int)rotation_y), 2)), 0.5) * 100;
					if (tmp_ratio < ratio[round_rotation_x][round_rotation_y])
					{
						right_image[round_rotation_x][round_rotation_y] = temp_right_image[i][j];
						ratio[round_rotation_x][round_rotation_y] = tmp_ratio;
					}
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool Interpolation(int angle, struct FOOT foot, char side)
{
	// whole
	short whole[2][200];
	int whole_cnt = 0;
	int rotate_angle = -angle;
	int near_number, near_cnt;
	// interpolation
	int rotation_x_j, rotation_y_i;
	float rotation_x, rotation_y;
	int p1, p2, p3, p4;
	float fy1, fy2, fx1, fx2;
	if (side == 'l')
	{
		// find whole
		// center is 0 and it should have at least 3 point
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 48; j++)
			{
				if (left_image[i][j] == 0)
				{
					near_number = 0;
					near_cnt = 0;
					if (left_image[i - 1][j] != 0)
					{
						near_number += 1;
						near_cnt += left_image[i - 1][j];
					}
					if (left_image[i + 1][j] != 0)
					{
						near_number += 1;
						near_cnt += left_image[i + 1][j];
					}
					if (left_image[i][j - 1] != 0)
					{
						near_number += 1;
						near_cnt += left_image[i][j - 1];
					}
					if (left_image[i][j + 1] != 0)
					{
						near_number += 1;
						near_cnt += left_image[i][j + 1];
					}
					if (near_number >= 3 && near_cnt >= 50)
					{
						whole[0][whole_cnt] = i;
						whole[1][whole_cnt] = j;
						whole_cnt++;
					}
				}
			}
		}
		// Bilinear Interpolation
		for (int i = 0; i < whole_cnt; i++)
		{
			rotation_x_j = whole[1][i] - 24;
			rotation_y_i = whole[0][i] - 24;
			rotation_x = (cos((rotate_angle)*M_PI / 180) * rotation_x_j) - (sin((rotate_angle)*M_PI / 180) * rotation_y_i);
			rotation_y = (sin((rotate_angle)*M_PI / 180) * rotation_x_j) + (cos((rotate_angle)*M_PI / 180) * rotation_y_i);
			rotation_x += foot.data_x_avg;
			rotation_y += foot.data_y_avg;
			p1 = temp_left_image[(int)(rotation_y) + 1][(int)(rotation_x)];
			p2 = temp_left_image[(int)(rotation_y) + 1][(int)(rotation_x) + 1];
			p3 = temp_left_image[(int)(rotation_y)][(int)(rotation_x)];
			p4 = temp_left_image[(int)(rotation_y)][(int)(rotation_x) + 1];
			fy1 = rotation_x - (int)(rotation_x);
			fy2 = 1 - fy1;
			fx2 = rotation_y - (int)(rotation_y);
			fx1 = 1 - fx2;
			left_image[whole[0][i]][whole[1][i]] = (lround((fy2 * fx2 * p1) + (fy1 * fx2 * p2) + (fy2 * fx1 * p3) + (fy1 * fx1 * p4)));
		}
		return true;
	}
	else if (side == 'r')
	{
		// find whole
		// center is 0 and it should have at least 3 point
		for (int i = 0; i < 48; i++)
		{
			for (int j = 0; j < 48; j++)
			{
				if (right_image[i][j] == 0)
				{
					near_number = 0;
					near_cnt = 0;
					if (right_image[i - 1][j] != 0)
					{
						near_number += 1;
						near_cnt += right_image[i - 1][j];
					}
					if (right_image[i + 1][j] != 0)
					{
						near_number += 1;
						near_cnt += right_image[i + 1][j];
					}
					if (right_image[i][j - 1] != 0)
					{
						near_number += 1;
						near_cnt += right_image[i][j - 1];
					}
					if (right_image[i][j + 1] != 0)
					{
						near_number += 1;
						near_cnt += right_image[i][j + 1];
					}
					if (near_number > 3 && near_cnt > 50)
					{
						whole[0][whole_cnt] = i;
						whole[1][whole_cnt] = j;
						whole_cnt++;
					}
				}
			}
		}
		// Bilinear Interpolation
		for (int i = 0; i < whole_cnt; i++)
		{
			rotation_x_j = whole[1][i] - 24;
			rotation_y_i = whole[0][i] - 24;
			rotation_x = (cos((rotate_angle)*M_PI / 180) * rotation_x_j) - (sin((rotate_angle)*M_PI / 180) * rotation_y_i);
			rotation_y = (sin((rotate_angle)*M_PI / 180) * rotation_x_j) + (cos((rotate_angle)*M_PI / 180) * rotation_y_i);
			rotation_x += foot.data_x_avg;
			rotation_y += foot.data_y_avg;
			p1 = temp_right_image[(int)(rotation_y) + 1][(int)(rotation_x)];
			p2 = temp_right_image[(int)(rotation_y) + 1][(int)(rotation_x) + 1];
			p3 = temp_right_image[(int)(rotation_y)][(int)(rotation_x)];
			p4 = temp_right_image[(int)(rotation_y)][(int)(rotation_x) + 1];
			fy1 = rotation_x - (int)(rotation_x);
			fy2 = 1 - fy1;
			fx2 = rotation_y - (int)(rotation_y);
			fx1 = 1 - fx2;
			right_image[whole[0][i]][whole[1][i]] = (lround((fy2 * fx2 * p1) + (fy1 * fx2 * p2) + (fy2 * fx1 * p3) + (fy1 * fx1 * p4)));
		}
		return true;
	}
	else
	{
		return false;
	}
}
#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL == 1) */
