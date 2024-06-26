# +
from copy import deepcopy
import numpy as np
from math import sqrt,fabs
import matplotlib.pyplot as plt
class COG:
    def __init__(self, first_x,first_y, second_x,second_y):
        self.first_x: int = first_x
        self.first_y: int = first_y
        self.second_x: int = second_x
        self.second_y: int = second_y

class TOE:
    def __init__(self, first_x,first_y,first_sum, second_x,second_y,second_sum):
        self.first_x: int = first_x
        self.first_y: int = first_y
        self.first_sum: int = first_sum
        self.second_x: int = second_x
        self.second_y: int = second_y
        self.second_sum: int = second_sum

class TWO_COG:
    def __init__(self, first_x,first_y, second_x,second_y,third_x,third_y,fourth_x,fourth_y,foot_count):
        self.first_x: int = first_x
        self.first_y: int = first_y
        self.second_x: int = second_x
        self.second_y: int = second_y
        self.third_x: int = third_x
        self.third_y: int = third_y
        self.fourth_x: int = fourth_x
        self.fourth_y: int = fourth_y
        self.foot_count: int = foot_count

class FOOT:
    def __init__(self, whole_width,cog,toe_x,toe_y):
        self.whole_width: int = 0
        self.cog = cog
        self.toe_x: int = 0
        self.toe_y: int = 0
        self.data_x=[]
        self.data_y=[]
        self.image= [[0]*96 for i in range(48)]

class TWO_FOOT:
    def __init__(self, foot):
        self.right = deepcopy(foot)
        self.left = deepcopy(foot)
width_hope = 3
near_hope = 7
min_value=3
delta=1e-4


# -

def separate_each_foot(image,cog,toe,two_foot):
    if (cog.foot_count == 1):
        #4cog
        if (cog.fourth_x != 0 and cog.fourth_y != 0 and cog.second_x != 0 and cog.second_y != 0):
                #line between toe 1 and cog_third
                a = (toe.first_x - cog.third_x) / (toe.first_y - cog.third_y+delta)
                b = 1
                x = (toe.first_x + cog.third_x) / 2
                y = (toe.first_y + cog.third_y) / 2
                c = -(a * x + b * y)
        #3cog
        elif (cog.third_x != 0 and cog.third_y != 0):

            #0 degree near toe or 180 degree near anywhere
            if (cog.fourth_x == 0 and cog.fourth_y == 0):
                #near toe
                if (toe.second_x == 0 and toe.second_y == 0):
                    #lint between toe1 and cog_third
                    a = (toe.first_x - cog.third_x) / (toe.first_y - cog.third_y+delta)
                    b = 1
                    x = (toe.first_x + cog.third_x) / 2
                    y = (toe.first_y + cog.third_y) / 2
                    c = -(a * x + b * y)
                #180 degree near heal
                else:
                    #lint between toe2 and cog_second
                    a = (toe.second_x - cog.second_x) / (toe.second_y - cog.second_y+delta)
                    b = 1
                    x = (toe.second_x + cog.second_x) / 2
                    y = (toe.second_y + cog.second_y) / 2
                    c = -(a * x + b * y)
            #0 degree near heal
            elif (cog.second_x == 0 and cog.second_y == 0):
                #lint between toe1 and cog_fourth
                a = (toe.first_x - cog.fourth_x) / (toe.first_y - cog.fourth_y+delta)
                b = 1
                x = (toe.first_x + cog.fourth_x) / 2
                y = (toe.first_y + cog.fourth_y) / 2
                c = -(a * x + b * y)
        #2cog
        else:
            two_foot.left.toe_x = toe.first_x
            two_foot.left.toe_y = toe.first_y
            two_foot.left.cog.first_x = cog.first_x
            two_foot.left.cog.first_y = cog.first_y
            two_foot.left.cog.second_x = cog.second_x
            two_foot.left.cog.second_y = cog.second_y
            if (toe.first_x > cog.second_x):
                if (toe.first_y > cog.second_y):
                    height = toe.first_x + foot_bound
                    low = cog.second_x - foot_bound
                    left = cog.second_y - foot_bound
                    right = toe.first_y + foot_bound
                else:
                    height = toe.first_x + foot_bound
                    low = cog.second_x - foot_bound
                    left = toe.first_y - foot_bound
                    right = cog.second_y + foot_bound
            else:
                if (toe.first_y > cog.second_y):
                    height = cog.second_x + foot_bound
                    low = toe.first_x - foot_bound
                    left = cog.second_y - foot_bound
                    right = toe.first_y + foot_bound
                else:
                    height = cog.second_x + foot_bound
                    low = toe.first_x - foot_bound
                    left = toe.first_y - foot_bound
                    right = cog.second_y + foot_bound
            for i in range(48):
                for j in range(96):
                    if image[i][j]>min_value:
                        two_foot.left.data_x.extend([j])
                        two_foot.left.data_y.extend([i])
                        two_foot.left.image[i][j] = image[i][j]
                        two_foot.left.whole_width+=1
            return two_foot;
        if (a * cog.first_x + b * cog.first_y + c < 0):
                two_foot.left.toe_x = toe.first_x
                two_foot.left.toe_y = toe.first_y
                two_foot.left.cog.first_x = cog.first_x
                two_foot.left.cog.first_y = cog.first_y
                two_foot.left.cog.second_x = cog.second_x
                two_foot.left.cog.second_y = cog.second_y
        else:
            two_foot.right.toe_x = toe.first_x
            two_foot.right.toe_y = toe.first_y
            two_foot.right.cog.first_x = cog.first_x
            two_foot.right.cog.first_y = cog.first_y
            two_foot.right.cog.second_x = cog.second_x
            two_foot.right.cog.second_y = cog.second_y


        if (a * cog.third_x + b * cog.third_y + c < 0):
            two_foot.left.toe_x = toe.second_x
            two_foot.left.toe_y = toe.second_y
            two_foot.left.cog.first_x = cog.third_x
            two_foot.left.cog.first_y = cog.third_y
            two_foot.left.cog.second_x = cog.fourth_x
            two_foot.left.cog.second_y = cog.fourth_y

        else:
            two_foot.right.toe_x = toe.second_x
            two_foot.right.toe_y = toe.second_y
            two_foot.right.cog.first_x = cog.third_x
            two_foot.right.cog.first_y = cog.third_y
            two_foot.right.cog.second_x = cog.fourth_x
            two_foot.right.cog.second_y = cog.fourth_y

        for i in range(48):
            for j in range(96):
                if image[i][j]>min_value:
                    if (a * i + b * j + c < 0):
                        two_foot.left.data_x.extend([j])
                        two_foot.left.data_y.extend([i])
                        two_foot.left.image[i][j] = image[i][j]
                        two_foot.left.whole_width+=1
                    else:
                        two_foot.right.data_x.extend([j])
                        two_foot.right.data_y.extend([i])
                        two_foot.right.image[i][j] = image[i][j]
                        two_foot.right.whole_width+=1
    #2foot
    elif (cog.foot_count == 2):
        #lint between toes
        a = (toe.first_x - toe.second_x) / (toe.first_y - toe.second_y+delta)
        b = 1
        x = (cog.second_x + cog.fourth_x) / 2
        y = (cog.second_y + cog.fourth_y) / 2
        c = -(a * x + b * y)

        if (a * cog.first_x + b * cog.first_y + c < 0):
            two_foot.left.toe_x = toe.first_x
            two_foot.left.toe_y = toe.first_y
            two_foot.left.cog.first_x = cog.first_x
            two_foot.left.cog.first_y = cog.first_y
            two_foot.left.cog.second_x = cog.second_x
            two_foot.left.cog.second_y = cog.second_y
        else:
            two_foot.right.toe_x = toe.first_x
            two_foot.right.toe_y = toe.first_y
            two_foot.right.cog.first_x = cog.first_x
            two_foot.right.cog.first_y = cog.first_y
            two_foot.right.cog.second_x = cog.second_x
            two_foot.right.cog.second_y = cog.second_y

        if (a * cog.third_x + b * cog.third_y + c < 0):
            two_foot.left.toe_x = toe.second_x
            two_foot.left.toe_y = toe.second_y
            two_foot.left.cog.first_x = cog.third_x
            two_foot.left.cog.first_y = cog.third_y
            two_foot.left.cog.second_x = cog.fourth_x
            two_foot.left.cog.second_y = cog.fourth_y
        else:
            two_foot.right.toe_x = toe.second_x
            two_foot.right.toe_y = toe.second_y
            two_foot.right.cog.first_x = cog.third_x
            two_foot.right.cog.first_y = cog.third_y
            two_foot.right.cog.second_x = cog.fourth_x
            two_foot.right.cog.second_y = cog.fourth_y
        for i in range(48):
            for j in range(96):
                if image[i][j]>min_value:
                    if (a * i + b * j + c < 0):
                        two_foot.left.data_x.extend([j])
                        two_foot.left.data_y.extend([i])
                        two_foot.left.image[i][j] = image[i][j]
                        two_foot.left.whole_width+=1
                    else:
                        two_foot.right.data_x.extend([j])
                        two_foot.right.data_y.extend([i])
                        two_foot.right.image[i][j] = image[i][j]
                        two_foot.right.whole_width+=1
    else:
        return two_foot
    return two_foot


def calculate_distance(first_x,first_y,second_x,second_y):
    if second_x==0 and second_y==0:
        return 9999
    return sqrt(pow(first_x - second_x, 2) + pow(first_y - second_y, 2))


def assemble_cog_and_toe(cog,toe):
    #first
    if (toe.first_x != 0 and toe.first_y != 0 and cog.first_x != 0 and cog.first_y != 0 and cog.second_x != 0 and cog.second_y != 0):
        min = calculate_distance(toe.first_x, toe.first_y, cog.first_x, cog.first_y)
        if (min > calculate_distance(toe.first_x, toe.first_y, cog.second_x, cog.second_y)):
            cog.first_x, cog.second_x = cog.second_x, cog.first_x
            cog.first_y, cog.second_y = cog.second_y, cog.first_y
        min = calculate_distance(toe.first_x, toe.first_y, cog.first_x, cog.first_y)
        if (min > calculate_distance(toe.first_x, toe.first_y, cog.third_x, cog.third_y)):
            cog.first_x, cog.third_x = cog.third_x, cog.first_x
            cog.first_y, cog.third_y = cog.third_y, cog.first_y
        min = calculate_distance(toe.first_x, toe.first_y, cog.first_x, cog.first_y)
        if (min > calculate_distance(toe.first_x, toe.first_y, cog.fourth_x, cog.fourth_y)):
            cog.first_x, cog.fourth_x = cog.fourth_x, cog.first_x
            cog.first_y, cog.fourth_y = cog.fourth_y, cog.first_y
    else:
        return cog
#     print(cog.first_x,cog.first_y,cog.second_x,cog.second_y,cog.third_x,cog.third_y,cog.fourth_x,cog.fourth_y)
#     print(toe.first_x,toe.first_y,toe.second_x,toe.second_y)
    #second
    #if it has 4 cog
    if (toe.second_x != 0 and toe.second_y != 0 and cog.third_x != 0 and cog.third_y != 0 and cog.fourth_x != 0 and cog.fourth_y != 0):
        min = calculate_distance(toe.second_x, toe.second_y, cog.third_x, cog.third_y)
        if (min > calculate_distance(toe.second_x, toe.second_y, cog.second_x, cog.second_y)):
            cog.third_x, cog.second_x = cog.second_x, cog.third_x
            cog.third_y, cog.second_y = cog.second_y, cog.third_y
        min = calculate_distance(toe.second_x, toe.second_y, cog.third_x, cog.third_y)
        if (min > calculate_distance(toe.second_x, toe.second_y, cog.fourth_x, cog.fourth_y)):
            cog.third_x, cog.fourth_x = cog.fourth_x, cog.third_x
            cog.third_y, cog.fourth_y = cog.fourth_y, cog.third_y

        cog.foot_count = 2
#         print(cog.first_x,cog.first_y,cog.second_x,cog.second_y,cog.third_x,cog.third_y,cog.fourth_x,cog.fourth_y)
#         print(toe.first_x,toe.first_y,toe.second_x,toe.second_y)
    #if it has 3 cog
    elif (cog.third_x != 0 and cog.third_y != 0):
        #if it has 3cog and 2toe
        #compare second third
        if (toe.second_x != 0 and toe.second_y != 0):
            min = calculate_distance(toe.second_x, toe.second_y, cog.third_x, cog.third_y)
            if (min > calculate_distance(toe.second_x, toe.second_y, cog.second_x, cog.second_y)):
                cog.third_x, cog.second_x = cog.second_x, cog.third_x
                cog.third_y, cog.second_y = cog.second_y, cog.third_y

            #find second cog owner
            a = (toe.first_y - cog.first_y) / (toe.first_x - cog.first_x+delta)
            b = -1
            c = toe.first_y - (a * toe.first_x)
            min = fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b)

            a = (toe.second_y - cog.third_y) / (toe.second_x - cog.third_x+delta)
            b = -1
            c = toe.second_y - (a * toe.second_x)
            if ((min > (fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b)))
                    and calculate_distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y)
                            > calculate_distance(cog.third_x, cog.third_y, cog.second_x, cog.second_y)):
                cog.second_x, cog.fourth_x = cog.fourth_x, cog.second_x
                cog.second_y, cog.fourth_y = cog.fourth_y, cog.second_y

        a = (toe.first_y - cog.first_y) / (toe.first_x - cog.first_x+delta)
        b = -1
        c = toe.first_y - (a * toe.first_x)
        min = fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b)
        if ((min > (fabs(a * cog.third_x + b * cog.third_y + c) / sqrt(a * a + b * b)))
                and calculate_distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y)
                        > calculate_distance(cog.first_x, cog.first_y, cog.third_x, cog.third_y)):
            cog.second_x, cog.third_x = cog.third_x, cog.second_x
            cog.second_y, cog.third_y = cog.third_y, cog.second_y

        cog.foot_count = 1
        return cog
    #if it has only 2 cog
    else:
        cog.foot_count = 1
        return cog

    #compare second fourth
    a = (toe.first_y - cog.first_y) / (toe.first_x - cog.first_x+delta)
    b = -1
    c = toe.first_y - (a * toe.first_x)
    min = fabs(a * cog.second_x + b * cog.second_y + c) / sqrt(a * a + b * b)
    if ((min > (fabs(a * cog.fourth_x + b * cog.fourth_y + c) / sqrt(a * a + b * b)))
            and calculate_distance(cog.first_x, cog.first_y, cog.second_x, cog.second_y)
                    > calculate_distance(cog.first_x, cog.first_y, cog.fourth_x, cog.fourth_y)):
        cog.second_x, cog.fourth_x = cog.fourth_x, cog.second_x
        cog.second_y, cog.fourth_y = cog.fourth_y, cog.second_y
#     print(cog.first_x,cog.first_y,cog.second_x,cog.second_y,cog.third_x,cog.third_y,cog.fourth_x,cog.fourth_y)
#     print(toe.first_x,toe.first_y,toe.second_x,toe.second_y)
    return cog


def find_near_toe(current_x,current_y,toe,hope):
    if (toe.first_x - hope <= current_x and current_x <= toe.first_x + hope and toe.first_y - hope <= current_y and current_y <= toe.first_y + hope):
        return 1
    elif (toe.second_x - hope <= current_x and current_x <= toe.second_x + hope and toe.second_y - hope <= current_y and current_y <= toe.second_y + hope):
        return 2
    return 3


def add_3x3(x,y,image):
    number=0
    for i in range(-1,2):
        for j in range(-1,2):
            if (-1<x+i and x+i<48) and (-1<y+j and y+j<96):
                number=number+image[x+i][y+j]
    return number


def is_toe(current_x,current_y,toe,image):
    temp_add=int(0)
    sum=int(0)
    for i in range(-2,3,2):
        for j in range(-2,3,2):
            temp_add=add_3x3(current_x+i,current_y+j,image)
            if temp_add <=10:
                return toe
            else:
                sum+=temp_add

    if (toe.first_x == 0 and toe.first_y == 0):
        toe.first_x = current_x
        toe.first_y = current_y
        toe.first_sum = sum
        return toe

    near=find_near_toe(current_x,current_y,toe,near_hope)

    if near==3:
        toe.second_x = current_x
        toe.second_y = current_y
        toe.second_sum = sum
    elif near ==2:
        if (toe.second_sum < sum):
            toe.second_x = current_x
            toe.second_y = current_y
            toe.second_sum = sum
    elif near==1:
        if (toe.first_sum < sum):
            toe.first_x = current_x
            toe.first_y = current_y
            toe.first_sum = sum
    return toe


def find_near_cog_2(current_x,current_y,cog,hope):
    if (cog.first_x - hope <= current_x and current_x <= cog.first_x + hope and cog.first_y - hope <= current_y and current_y <= cog.first_y + hope):
        return 1
    elif (cog.second_x - hope <= current_x and current_x <= cog.second_x + hope and cog.second_y - hope <= current_y and current_y <= cog.second_y + hope):
        return 2
    elif (cog.third_x - hope <= current_x and current_x <= cog.third_x + hope and cog.third_y - hope <= current_y and current_y <= cog.third_y + hope):
        return 3
    elif (cog.fourth_x - hope <= current_x and current_x <= cog.fourth_x + hope and cog.fourth_y - hope <= current_y and current_y <= cog.fourth_y + hope):
        return 4
    return 5


def check_width(x,y,hope,image):
    width=int(0)
    for i in range(x-hope,x+hope+1):
        for j in range(y-hope,y+hope+1):
            if ((0 > i or i > 47) or (0 > j or j > 95)):
                continue
            if (image[i][j] != 0):
                width += 1
    return width


def check_weight(x,y,hope,image):
    weight=int(0)
    for i in range(x-hope,x+hope+1):
        for j in range(y-hope,y+hope+1):
            if ((0 > i or i > 47) or (0 > j or j > 95)):
                continue
            if (image[i][j] != 0):
                weight += image[i][j]
    return weight


def is_cog_2(current_x,current_y,cog,image):
    if check_width(current_x,current_y,width_hope,image)>30:
        if (cog.first_x == 0 and cog.first_y == 0):
            cog.first_x = current_x
            cog.first_y = current_y
            return cog
        near = find_near_cog_2(current_x,current_y,cog,near_hope)
        if near ==5:
            if (cog.second_x == 0 and cog.second_y == 0):
                cog.second_x = current_x
                cog.second_y = current_y
            elif (cog.third_x == 0 and cog.third_y == 0):
                cog.third_x = current_x
                cog.third_y = current_y
            elif (cog.fourth_x == 0 and cog.fourth_y == 0):
                cog.fourth_x = current_x
                cog.fourth_y = current_y
        elif near ==4:
            if (check_width(cog.fourth_x, cog.fourth_y, 3, image) < check_width(current_x, current_y, 3, image)):
                cog.fourth_x = current_x
                cog.fourth_y = current_y
            elif ((check_width(cog.fourth_x, cog.fourth_y, 3, image) == check_width(current_x, current_y, 3, image))
                    and (check_weight(cog.fourth_x, cog.fourth_y, 3, image) < check_weight(current_x, current_y, 3, image))):
                cog.fourth_x = current_x
                cog.fourth_y = current_y
        elif near ==3:
            if (check_width(cog.third_x, cog.third_y, 3, image) < check_width(current_x, current_y, 3, image)):
                cog.third_x = current_x
                cog.third_y = current_y
            elif ((check_width(cog.third_x, cog.third_y, 3, image) == check_width(current_x, current_y, 3, image))
                    and (check_weight(cog.third_x, cog.third_y, 3, image) < check_weight(current_x, current_y, 3, image))):
                cog.third_x = current_x
                cog.third_y = current_y
        elif near ==2:
            if (check_width(cog.second_x, cog.second_y, 3, image) < check_width(current_x, current_y, 3, image)):
                cog.second_x = current_x
                cog.second_y = current_y
            elif ((check_width(cog.second_x, cog.second_y, 3, image) == check_width(current_x, current_y, 3, image))
                    and (check_weight(cog.second_x, cog.second_y, 3, image) < check_weight(current_x, current_y, 3, image))):
                cog.second_x = current_x
                cog.second_y = current_y
        elif near ==1:
            if (check_width(cog.first_x, cog.first_y, 3, image) < check_width(current_x, current_y, 3, image)):
                cog.first_x = current_x
                cog.first_y = current_y
            elif ((check_width(cog.first_x, cog.first_y, 3, image) == check_width(current_x, current_y, 3, image))
                    and (check_weight(cog.first_x, cog.first_y, 3, image) < check_weight(current_x, current_y, 3, image))):
                cog.first_x = current_x
                cog.first_y = current_y
    return cog


def print_array(image,mode="grey"):
    for i in range(len(image)):
        for j in range(len(image[0])):
            if mode == "grey":
                print(image[47-i][j][0],end=" ")
            elif mode == "none":
                print(image[47-i][j],end=" ")
        print()


def preprocess(original_image, show=True):
    left=[]
    right=[]
    for img_cnt in range(len(original_image)):
#         print_image([original_image[img_cnt]], show)
        image=original_image[img_cnt]
        one_cog=COG(0,0,0,0)
        toe=TOE(0,0,0,0,0,0)
        cog=TWO_COG(0,0,0,0,0,0,0,0,0)
        foot=FOOT(0,one_cog,0,0)
        two_foot=TWO_FOOT(foot)
        two_foot.right.image=np.array(two_foot.right.image).reshape(48,96,1)
        two_foot.left.image=np.array(two_foot.left.image).reshape(48,96,1)
        for i in range(48):
            for j in range(96):
                if image[i][j] !=0:
                    cog=is_cog_2(i,j,cog,image)
                elif image[i][j]==0:
                    toe=is_toe(i,j,toe,image)
#         print(cog.first_x,cog.first_y,cog.second_x,cog.second_y,cog.third_x,cog.third_y,cog.fourth_x,cog.fourth_y)
#         print(toe.first_x,toe.first_y,toe.second_x,toe.second_y)
        cog=assemble_cog_and_toe(cog,toe)
        two_foot = separate_each_foot(image, cog, toe, two_foot)
#         print_image([two_foot.left.image], show)
#         print_image([two_foot.right.image], show)
        distance1 = calculate_distance(two_foot.left.cog.first_x, two_foot.left.cog.first_y, two_foot.left.cog.second_x, two_foot.left.cog.second_y);
        distance2 = calculate_distance(two_foot.right.cog.first_x, two_foot.right.cog.first_y, two_foot.right.cog.second_x, two_foot.right.cog.second_y);
        if (cog.foot_count == 1):
#             print("1")
            if(two_foot.left.whole_width > distance1 * 5):
                temp_left=np.array(two_foot.left.image)
                #data,angle=check_angle(temp_left)
                angle=line_total_least_squares(two_foot.left.data_x,two_foot.left.data_y,two_foot.left.toe_y,two_foot.left.toe_x)
#                 print(angle)
                print_image([temp_left], show)
                img=rotate_foot(temp_left,angle)
                print_image([img], show)
                re=find_whole(temp_left,img,angle)
                if classify_each_foot(re) == "left":
                    left.append(re)
                    print_image([re], show)
                else:
                    right.append(re)
                    print_image([re], show)

            elif(two_foot.right.whole_width > distance2 * 5):
                temp_right=np.array(two_foot.right.image)
                #data,angle=check_angle(temp_right)
                angle=line_total_least_squares(two_foot.right.data_x,two_foot.right.data_y,two_foot.right.toe_y,two_foot.right.toe_x)
#                 print(angle)
                print_image([temp_right], show)
                img=rotate_foot(temp_right,angle)
                print_image([img], show)
                re=find_whole(temp_right,img,angle)
                if classify_each_foot(re) == "left":
                    left.append(re)
                    print_image([re], show)
                else:
                    right.append(re)
                    print_image([re], show)
        elif(cog.foot_count==2):
#             print("2")
#             print(cog.first_x,cog.first_y,cog.second_x,cog.second_y,cog.third_x,cog.third_y,cog.fourth_x,cog.fourth_y)
#             print(toe.first_x,toe.first_y,toe.second_x,toe.second_y)
            if(two_foot.left.whole_width > distance1 * 5):
                temp_left=np.array(two_foot.left.image)
                #data,angle=check_angle(temp_left)
                angle=line_total_least_squares(two_foot.left.data_x,two_foot.left.data_y,two_foot.left.toe_y,two_foot.left.toe_x)
#                 print(angle)
                print_image([temp_left], show)
                img=rotate_foot(temp_left,angle)
                print_image([img], show)
                re=find_whole(temp_left,img,angle)
                if classify_each_foot(re) == "left":
                    left.append(re)
                    print_image([re], show)
                else:
                    right.append(re)
                    print_image([re], show)

            if(two_foot.right.whole_width > distance1 * 5):
                temp_right=np.array(two_foot.right.image)
                #data,angle=check_angle(temp_right)
                angle=line_total_least_squares(two_foot.right.data_x,two_foot.right.data_y,two_foot.right.toe_y,two_foot.right.toe_x)
#                 print(angle)
                print_image([temp_right], show)
                img=rotate_foot(temp_right,angle)
                print_image([img], show)
                re=find_whole(temp_right,img,angle)
                if classify_each_foot(re) == "left":
                    left.append(re)
                    print_image([re], show)
                else:
                    right.append(re)
                    print_image([re], show)
    left=np.array(left)
    right=np.array(right)
    return left,right


def classify_each_foot(image):
    #image=np.array(image).reshape(48,96)[:,0:48]
    np.set_printoptions(threshold=np.inf,linewidth=np.inf)
    left_filter=[[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]]]
    #print(np.array(left_filter).reshape(48,48,1))

    right_filter=[[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[1],[1],[1],[1],[1],[1],[1],[1],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0],[0]]]
    #print(np.array(right_filter).reshape(48,48,1))
    left_sum=np.sum(np.multiply(left_filter,image))
    right_sum=np.sum(np.multiply(right_filter,image))
    if left_sum>right_sum:
        return "left"
    else:
        return "right"


def print_image(image, show=True):
    if show:
        for i in range(len(image)):
            plt.imshow(image[i])
            plt.gca().invert_yaxis()
            plt.show()


# +
# Total Least Squares:
def line_total_least_squares(x,y,toe_y,toe_x):
    n = len(x)
    x_=np.sum(x)/n
    y_=np.sum(y)/n
    # Calculate the x~ and y~
    x1 = x - x_
    y1 = y - y_
#     plt.plot(x1,y1,'ro')
    toe_x=toe_x-y_
    toe_y=toe_y-x_
#     plt.plot(toe_y,toe_x,'yo')

    # Create the matrix array
    X = np.vstack((y1, x1))
    X_t = np.transpose(X)


    # Finding A_T_A and it's Find smallest eigenvalue::
    prd = np.dot(X,X_t)
    W,V = np.linalg.eig(prd)
    small_eig_index = W.argmin()
    a,b = V[:,small_eig_index]
    # Compute C:
    c = (-1*a*x_) + (-1*b*y_)
    toe_angle=get_degree(toe_y,toe_x)
#     plt.plot(a*10,-b*10,'bo')
#     plt.plot(-a*10,b*10,'bo')
#     plt.show()
    tls_angle_1=get_degree(a,-b)
    tls_angle_2=get_degree(-a,b)

    if np.abs(360-tls_angle_1+toe_angle)>np.abs(360+tls_angle_1-toe_angle):
        if np.abs(360+tls_angle_1-toe_angle)>np.abs(tls_angle_1-toe_angle):
            abs_angle_1=np.abs(tls_angle_1-toe_angle)
        else:
            abs_angle_1=np.abs(360+tls_angle_1-toe_angle)
    else:
        if np.abs(360-tls_angle_1+toe_angle)>np.abs(tls_angle_1-toe_angle):
            abs_angle_1=np.abs(tls_angle_1-toe_angle)
        else:
            abs_angle_1=np.abs(360-tls_angle_1+toe_angle)


    if np.abs(360-tls_angle_2+toe_angle)>np.abs(360+tls_angle_2-toe_angle):
        if np.abs(360+tls_angle_2-toe_angle)>np.abs(tls_angle_2-toe_angle):
            abs_angle_2=np.abs(tls_angle_2-toe_angle)
        else:
            abs_angle_2=np.abs(360+tls_angle_2-toe_angle)
    else:
        if np.abs(360-tls_angle_2+toe_angle)>np.abs(tls_angle_2-toe_angle):
            abs_angle_2=np.abs(tls_angle_2-toe_angle)
        else:
            abs_angle_2=np.abs(360-tls_angle_2+toe_angle)


#     print(toe_angle,tls_angle_1,tls_angle_2)
    #print(toe_angle,tls_angle_1,tls_angle_2)
    if abs_angle_1<abs_angle_2:
#         print(toe_angle,tls_angle_1)
        return [tls_angle_1,x_,y_]
    else:
#         print(toe_angle,tls_angle_2)
        return [tls_angle_2,x_,y_]


# -

def rotate_foot(image,angle):
    data=[[[0] for col in range(96)] for row in range(48)]
    ratio=[[100 for col in range(96)] for row in range(48)]
    rotate_angle=360-angle[0]
    for i in range(48):
        for j in range(96):
            if image[i][j]>0:
                rotation_x_i=i-angle[2]
                rotation_y_j=j-angle[1]
                rotation_x=(np.cos((rotate_angle)*np.pi/180)*(rotation_x_i))-(np.sin((rotate_angle)*np.pi/180)*(rotation_y_j))
                rotation_y=(np.sin((rotate_angle)*np.pi/180)*(rotation_x_i))+(np.cos((rotate_angle)*np.pi/180)*(rotation_y_j))
                rotation_x=24+rotation_x
                rotation_y=24+rotation_y
                tmp_ratio=((rotation_x-int(rotation_x))**2+(rotation_y-int(rotation_y))**2)**0.5
                if tmp_ratio < ratio[int(round(rotation_x,0))][int(round(rotation_y,0))]:
                    data[int(round(rotation_x,0))][int(round(rotation_y,0))]=image[i][j]
                    ratio[int(round(rotation_x,0))][int(round(rotation_y,0))]=tmp_ratio
    return data


def find_whole(original,img,angle):
    image=deepcopy(img)
    whole=[]
#     data_x=[]
#     data_y=[]
# #     rotated_x=[]
# #     rotated_y=[]
    rotate_angle=-angle[0]
    for i in range(1,47):
        for j in range(1,47):
            #if image[i][j]==0 and image[i-1][j]!=0 and image[i+1][j]!=0 and image[i][j-1]!=0 and image[i][j+1]!=0:
            near_number=int(0)
            near_number_cnt=int(0)
            if image[i-1][j][0]!=0:
                near_number+=1
                near_number_cnt+=image[i-1][j][0]
            if image[i+1][j][0]!=0:
                near_number+=1
                near_number_cnt+=image[i+1][j][0]
            if image[i][j-1][0]!=0:
                near_number+=1
                near_number_cnt+=image[i][j-1][0]
            if image[i][j+1][0]!=0:
                near_number+=1
                near_number_cnt+=image[i][j+1][0]
            if image[i][j][0]==0 and  near_number>=3 and near_number_cnt>50:
                whole.append([i,j])
#     for i in range(1,47):
#         for j in range(1,47):
#             if image[i][j][0]>0:
#                 data_x.append([j])
#                 data_y.append([i])
#                 rotated_x.append([(np.cos((rotate_angle)*np.pi/180)*(j-24))-(np.sin((rotate_angle)*np.pi/180)*(i-24))])
#                 rotated_y.append([(np.sin((rotate_angle)*np.pi/180)*(j-24))+(np.cos((rotate_angle)*np.pi/180)*(i-24))])
#     plt.plot(data_x,data_y,'ro')
#     plt.show()
#     plt.plot(rotated_x,rotated_y,'ro')
#     plt.show()

    for i in range(len(whole)):
                    #print(data_num)
        rotation_x_i=whole[i][1]-24 #i
        rotation_y_j=whole[i][0]-24 #j
        rotation_x=(np.cos((rotate_angle)*np.pi/180)*rotation_x_i)-(np.sin((rotate_angle)*np.pi/180)*rotation_y_j)
        rotation_y=(np.sin((rotate_angle)*np.pi/180)*rotation_x_i)+(np.cos((rotate_angle)*np.pi/180)*rotation_y_j)
#         plt.plot(rotation_x,rotation_y,'bo')
        rotation_x+=angle[1]
        rotation_y+=angle[2]
        p1=original[int(rotation_y)+1][int(rotation_x)][0]
        p2=original[int(rotation_y)+1][int(rotation_x)+1][0]
        p3=original[int(rotation_y)][int(rotation_x)][0]
        p4=original[int(rotation_y)][int(rotation_x)+1][0]
        fy1=rotation_x-int(rotation_x)
        fy2=1-fy1
        fx2=rotation_y-int(rotation_y)
        fx1=1-fx2
        image[whole[i][0]][whole[i][1]]=[int(round((fy2*fx2*p1)+(fy1*fx2*p2)+(fy2*fx1*p3)+(fy1*fx1*p4),0))]
#     plt.show()
    image=np.array(image).reshape(48,96,1)[:,0:48]
    return image


def get_degree(a,b):
    degree=np.arctan2(a,b)*180/np.pi
    degree=int(np.round(degree))
    if degree<0:
        degree=360+degree
    return degree
