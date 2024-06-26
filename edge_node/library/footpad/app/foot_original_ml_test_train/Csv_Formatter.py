import csv
import numpy as np
import os

def UserCheck(name):
    f=open("./data/user.txt",'r+',encoding='utf-8',newline='')
    user_list={}
    lines = f.readlines()
    for line in lines:
        user_list[str(line[line.find(',')+1:-1])]=int(line[:line.find(',')])
    if name in user_list:
        f.close()
        return user_list[name]
    else:
        f.write(str(len(user_list))+","+name+"\n")
        f.close()
        return len(user_list)

def WriteCsv(image,name):
    f=open("./data/"+str(UserCheck(name))+".csv",'a',encoding='utf-8',newline='')
    write=csv.writer(f)
    #print(UserCheck(name))
    for i in range(48):
        write.writerow(image[i])
    write.writerow(str(UserCheck(name)))
    f.close()

def ReadCsv(filename):
    f=open(filename,'r',encoding='utf-8')
    read=csv.reader(f)
    foot=[]
    target=[]
    list_cnt=0
    data=[]
    for line in read:
        if list_cnt==48:
            foot.append(data)
            target.append([int(line[0])])
            data=[]
            list_cnt=0
            continue
        line=[[int(x, 16)] for x in line]
        for i in range(len(line)):
            if line[i][0]<3:
                line[i][0]=0
        line=np.array(line,dtype=np.int32)
        data.append(line)
        list_cnt+=1
    f.close()
    foot=np.array(foot)
    target=np.array(target)
    return foot,target

def Search(dirname):
    full_filename=[]
    filenames = os.listdir(dirname)
    for filename in filenames:
        if ".csv" in filename:
            full_filename.append(os.path.join(dirname, filename))
    return full_filename

def Concate(filelist):
    for i in range(len(filelist)):
        temp_foot, temp_target=ReadCsv(filelist[i])
        if i ==0:
            foot=temp_foot
            target=temp_target
        else:
            foot=np.concatenate((foot,temp_foot),axis=0)
            target=np.concatenate((target,temp_target),axis=0)
    return foot, target
