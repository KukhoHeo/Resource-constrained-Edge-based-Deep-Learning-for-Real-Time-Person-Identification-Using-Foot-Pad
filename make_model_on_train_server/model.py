# +
import numpy as np
import random
from sklearn.model_selection import train_test_split
from sklearn.utils import shuffle
def make_dataset(foot, target,target_cnt_e,random_state):
    x_train=[]
    x_test=[]
    t_train=[]
    t_test=[]
    x_validation=[]
    t_validation=[]
    x_train_all=[]
    t_train_all=[]
    x_validation_all=[]
    t_validation_all=[]
    target_cnt=len(np.unique(target))
    #target_cnt=10
    data_cnt=1000
    validation_ratio=10
    test_cnt_ratio=5
    random.seed(random_state)

    for i in range (5):
        x_train.append([])
        t_train.append([])
        x_validation.append([])
        t_validation.append([])

#     for cnt_cnt in range (len(target_cnt_e)):
#         i=target_cnt_e[cnt_cnt]
    for i in range(target_cnt_e):
        index=np.where(target==i)
        temp_foot=foot[index[0]]
        random.shuffle(temp_foot)
        temp_target=target[index[0]]

        #20% of foot data(1000)
        x_test.extend(temp_foot[int(data_cnt-data_cnt/test_cnt_ratio):int(data_cnt)])
        t_test.extend(temp_target[int(data_cnt-data_cnt/test_cnt_ratio):int(data_cnt)])
        x_train_all.extend(temp_foot[:int(data_cnt-data_cnt/test_cnt_ratio)-int(data_cnt/validation_ratio)])
        t_train_all.extend(temp_target[:int(data_cnt-data_cnt/test_cnt_ratio)-int(data_cnt/validation_ratio)])
        x_validation_all.extend(temp_foot[int(data_cnt-data_cnt/test_cnt_ratio)-int(data_cnt/validation_ratio):int(data_cnt-data_cnt/test_cnt_ratio)])
        t_validation_all.extend(temp_target[int(data_cnt-data_cnt/test_cnt_ratio)-int(data_cnt/validation_ratio):int(data_cnt-data_cnt/test_cnt_ratio)])
        for v in range(5):
            # 20% of validation data in train data
            x_validation[v].extend(temp_foot[int(v*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio):int((v+1)*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio)])
            t_validation[v].extend(temp_target[int(v*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio):int((v+1)*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio)])
            for j in range (validation_ratio):
                if v != j:
                    x_train[v].extend(temp_foot[int((j%validation_ratio)*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio):int((j+1%validation_ratio)*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio)])
                    t_train[v].extend(temp_target[int((j%validation_ratio)*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio):int((j+1%validation_ratio)*(data_cnt-data_cnt/test_cnt_ratio)/validation_ratio)])

    x_train,t_train=shuffle(x_train,t_train, random_state=random_state)
    x_test,t_test=shuffle(x_test,t_test, random_state=random_state)
    x_validation,t_validation=shuffle(x_validation,t_validation, random_state=random_state)
    x_train_all,t_train_all=shuffle(x_train_all,t_train_all, random_state=random_state)
    x_validation_all,t_validation_all=shuffle(x_validation_all,t_validation_all, random_state=random_state)


    x_train=np.array(x_train).astype(np.float32)
    t_train=np.array(t_train)


    x_test=np.array(x_test).astype(np.float32)
    t_test=np.array(t_test)


    x_validation=np.array(x_validation).astype(np.float32)
    t_validation=np.array(t_validation)

    x_train_all=np.array(x_train_all).astype(np.float32)
    t_train_all=np.array(t_train_all)

    x_validation_all=np.array(x_validation_all).astype(np.float32)
    t_validation_all=np.array(t_validation_all)
    return x_train_all, t_train_all, x_validation_all, t_validation_all, x_validation,t_validation,x_train, t_train, x_test, t_test

# +
import os

def createDirectory(directory):
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print("Error: Failed to create the directory.")
# +
import tensorflow as tf
from tensorflow import keras

def lenet5(random_state):
    random.seed(random_state)
    np.random.seed(random_state)
    tf.random.set_seed(random_state)
    model = keras.Sequential()
    model.add(keras.layers.Conv2D(filters=6, kernel_size=(5, 5), activation='relu', padding='same',input_shape=(48,48,1)))
    model.add(keras.layers.AveragePooling2D())
    model.add(keras.layers.Conv2D(filters=16, kernel_size=(5, 5), activation='relu',padding='valid'))
    model.add(keras.layers.AveragePooling2D())
    model.add(keras.layers.Flatten())
    model.add(keras.layers.Dense(units=120, activation='relu',name="test_dense"))
    model.add(keras.layers.Dense(units=84, activation='relu'))
    model.add(keras.layers.Dense(units=25, activation = 'softmax'))
    model.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(learning_rate= 0.0001), metrics=['accuracy'])
    print("tensorflow version = ",tf.__version__)
    print("keras version = ",keras.__version__)
    model.summary()
    return model

def alexnet(random_state):
    random.seed(random_state)
    np.random.seed(random_state)
    tf.random.set_seed(random_state)
    # modeling(functional API)
    
    model = keras.Sequential()
    # for original Alexnet
    model.add(keras.layers.Conv2D(12, 11, strides=(1,1), activation='relu', padding='same', input_shape=(48,48,1,)))
    model.add(keras.layers.MaxPooling2D(pool_size=(3,3), strides=(2,2)))
    # Local Response normalization for Original Alexnet
    model.add(keras.layers.BatchNormalization())

    model.add(keras.layers.Conv2D(32, (5,5), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D(pool_size=(3,3), strides=(2,2)))
    # Local Response normalization for Original Alexnet
    model.add(keras.layers.BatchNormalization())

    model.add(keras.layers.Conv2D(48, (3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(48, (3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(32, (3,3), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D(pool_size=(3,3), strides=(2,2)))
    # Local Response normalization for Original Alexnet
    model.add(keras.layers.BatchNormalization())

    model.add(keras.layers.Flatten())
    model.add(keras.layers.Dense(256, activation='tanh'))
    model.add(keras.layers.Dropout(0.5))
    model.add(keras.layers.Dense(128, activation='tanh'))
    model.add(keras.layers.Dropout(0.5))
    model.add(keras.layers.Dense(25, activation='softmax'))
    model.compile(loss=keras.losses.categorical_crossentropy,
              optimizer=keras.optimizers.Adam(),
              metrics=['accuracy'])
    model.summary()
    return model


# +
from keras.callbacks import EarlyStopping
from keras.layers import Dense, Conv2D,  MaxPool2D, Flatten, GlobalAveragePooling2D,  BatchNormalization, Layer, Add
from keras.models import Sequential
from keras.models import Model
import tensorflow as tf


class ResnetBlock(Model):
    """
    A standard resnet block.
    """

    def __init__(self, channels: int, down_sample=False):
        """
        channels: same as number of convolution kernels
        """
        super().__init__()

        self.__channels = channels
        self.__down_sample = down_sample
        self.__strides = [2, 1] if down_sample else [1, 1]
        print(self.__channels,self.__strides)
        KERNEL_SIZE = (3, 3)
        # use He initialization, instead of Xavier (a.k.a 'glorot_uniform' in Keras), as suggested in [2]
        INIT_SCHEME = "he_normal"

        self.conv_1 = Conv2D(self.__channels, strides=self.__strides[0],
                             kernel_size=KERNEL_SIZE, padding="same", kernel_initializer=INIT_SCHEME)
        self.bn_1 = BatchNormalization()
        self.conv_2 = Conv2D(self.__channels, strides=self.__strides[1],
                             kernel_size=KERNEL_SIZE, padding="same", kernel_initializer=INIT_SCHEME)
        self.bn_2 = BatchNormalization()
        self.merge = Add()

        if self.__down_sample:
            # perform down sampling using stride of 2, according to [1].
            self.res_conv = Conv2D(
                self.__channels, strides=2, kernel_size=(1, 1), kernel_initializer=INIT_SCHEME, padding="same")
            self.res_bn = BatchNormalization()

    def call(self, inputs):
        res = inputs
        x = self.conv_1(inputs)
        x = self.bn_1(x)
        x = tf.nn.relu(x)
        x = self.conv_2(x)
        x = self.bn_2(x)
        
        if self.__down_sample:
            res = self.res_conv(res)
            res = self.res_bn(res)

        # if not perform down sample, then add a shortcut directly
        x = self.merge([x, res])
        out = tf.nn.relu(x)
        return out


class ResNet18(Model):

    def __init__(self, num_classes, **kwargs):
        """
            num_classes: number of classes in specific classification task.
        """
        super().__init__(**kwargs)
        self.conv_1 = Conv2D(16, (7,7), strides=2,
                             padding="same", kernel_initializer="he_normal")
        self.init_bn = BatchNormalization()
        self.pool_2 = MaxPool2D(pool_size=(3,3), strides=2, padding="same")
        self.res_1_1 = ResnetBlock(16)
        self.res_1_2 = ResnetBlock(16)
        self.res_2_1 = ResnetBlock(32, down_sample=True)
        self.res_2_2 = ResnetBlock(32)
        self.res_3_1 = ResnetBlock(64, down_sample=True)
        self.res_3_2 = ResnetBlock(64)
        self.res_4_1 = ResnetBlock(128, down_sample=True)
        self.res_4_2 = ResnetBlock(128)
        self.avg_pool = GlobalAveragePooling2D()
        self.flat = Flatten()
        self.fc = Dense(num_classes, activation="softmax")
    def call(self, inputs):
        out = self.conv_1(inputs)
        out = self.init_bn(out)
        out = tf.nn.relu(out)
        out = self.pool_2(out)
        for res_block in [self.res_1_1, self.res_1_2, self.res_2_1, self.res_2_2, self.res_3_1, self.res_3_2, self.res_4_1, self.res_4_2]:
            out = res_block(out)
        out = self.avg_pool(out)
        out = self.flat(out)
        out = self.fc(out)
        return out
        


# -

import tensorflow as tf
from tensorflow import keras
def VGG16(random_state):
    random.seed(random_state)
    np.random.seed(random_state)
    tf.random.set_seed(random_state)
    model = keras.Sequential()
    model.add(keras.layers.Conv2D(filters=8, kernel_size=(3,3), activation='relu', padding='same',input_shape=(48,48,1)))
    model.add(keras.layers.Conv2D(filters=8, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D())
    model.add(keras.layers.Conv2D(filters=16, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=16, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D())
    model.add(keras.layers.Conv2D(filters=32, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=32, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=32, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D())
    model.add(keras.layers.Conv2D(filters=64, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=64, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=64, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D())
    model.add(keras.layers.Conv2D(filters=64, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=64, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.Conv2D(filters=64, kernel_size=(3,3), activation='relu', padding='same'))
    model.add(keras.layers.MaxPooling2D())
    model.add(keras.layers.Flatten())
    model.add(keras.layers.Dense(units=256, activation='relu'))
    model.add(keras.layers.Dense(units=128, activation='relu'))
    model.add(keras.layers.Dense(units=25, activation = 'softmax'))
    model.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(learning_rate= 0.0001), metrics=['accuracy'])
    print("tensorflow version = ",tf.__version__)
    print("keras version = ",keras.__version__)
    model.summary()
    return model


# +
from keras.models import Model
from keras.layers import Input, Conv2D, MaxPooling2D, AveragePooling2D, Flatten, GlobalAveragePooling2D, Dense, Dropout,Concatenate

def Inception_block(input_layer, f1, f2_conv1, f2_conv3, f3_conv1, f3_conv5, f4): 
    # Input: 
    # - f1: number of filters of the 1x1 convolutional layer in the first path
    # - f2_conv1, f2_conv3 are number of filters corresponding to the 1x1 and 3x3 convolutional layers in the second path
    # - f3_conv1, f3_conv5 are the number of filters corresponding to the 1x1 and 5x5  convolutional layer in the third path
    # - f4: number of filters of the 1x1 convolutional layer in the fourth path

    # 1st path:
    path1 = Conv2D(filters=f1, kernel_size = (1,1), padding = 'same', activation = 'relu')(input_layer)

    # 2nd path
    path2 = Conv2D(filters = f2_conv1, kernel_size = (1,1), padding = 'same', activation = 'relu')(input_layer)
    path2 = Conv2D(filters = f2_conv3, kernel_size = (3,3), padding = 'same', activation = 'relu')(path2)

    # 3rd path
    path3 = Conv2D(filters = f3_conv1, kernel_size = (1,1), padding = 'same', activation = 'relu')(input_layer)
    path3 = Conv2D(filters = f3_conv5, kernel_size = (5,5), padding = 'same', activation = 'relu')(path3)

    # 4th path
    path4 = MaxPooling2D((3,3), strides= (1,1), padding = 'same')(input_layer)
    path4 = Conv2D(filters = f4, kernel_size = (1,1), padding = 'same', activation = 'relu')(path4)

    output_layer = Concatenate(axis = -1)([path1, path2, path3, path4])

    return output_layer
def GoogLeNet(random_state):
    random.seed(random_state)
    np.random.seed(random_state)
    tf.random.set_seed(random_state)
    # input layer 
    input_layer = Input(shape = (48,48,1))

    # convolutional layer: filters = 64, kernel_size = (7,7), strides = 2
    X = Conv2D(filters = 8, kernel_size = (4,4), strides = 1, padding = 'valid', activation = 'relu')(input_layer)

    # max-pooling layer: pool_size = (3,3), strides = 2
    X = MaxPooling2D(pool_size = (2,2), strides = 1)(X)

    # convolutional layer: filters = 64, strides = 1
    X = Conv2D(filters = 8, kernel_size = (1,1), strides = 1, padding = 'same', activation = 'relu')(X)

    # convolutional layer: filters = 192, kernel_size = (3,3)
    X = Conv2D(filters = 24, kernel_size = (2,2), padding = 'same', activation = 'relu')(X)

    # max-pooling layer: pool_size = (3,3), strides = 2
    X = MaxPooling2D(pool_size= (2,2), strides = 1)(X)

    # 1st Inception block
    X = Inception_block(X, f1 = 8, f2_conv1 = 12, f2_conv3 = 16, f3_conv1 = 2, f3_conv5 = 4, f4 = 4)

    # 2nd Inception block
    X = Inception_block(X, f1 = 16, f2_conv1 = 16, f2_conv3 = 24, f3_conv1 = 4, f3_conv5 = 12, f4 = 8)

    # max-pooling layer: pool_size = (3,3), strides = 2
    X = MaxPooling2D(pool_size= (2,2), strides = 1)(X)

    # 3rd Inception block
    X = Inception_block(X, f1 = 24, f2_conv1 = 12, f2_conv3 = 26, f3_conv1 = 2, f3_conv5 = 6, f4 = 8)

    # Extra network 1:
    X1 = AveragePooling2D(pool_size = (3,3), strides = 2)(X)
    X1 = Conv2D(filters = 16, kernel_size = (1,1), padding = 'same', activation = 'relu')(X1)
    X1 = Flatten()(X1)
    X1 = Dense(128, activation = 'relu')(X1)
    X1 = Dropout(0.7)(X1)
    X1 = Dense(25, activation = 'softmax')(X1)


    # 4th Inception block
    X = Inception_block(X, f1 = 20, f2_conv1 = 14, f2_conv3 = 28, f3_conv1 = 3, f3_conv5 = 8, f4 = 8)

    # 5th Inception block
    X = Inception_block(X, f1 = 16, f2_conv1 = 16, f2_conv3 = 32, f3_conv1 = 3, f3_conv5 = 8, f4 = 8)

    # 6th Inception block
    X = Inception_block(X, f1 = 14, f2_conv1 = 18, f2_conv3 = 36, f3_conv1 = 4, f3_conv5 = 8, f4 = 8)

    # Extra network 2:
    X2 = AveragePooling2D(pool_size = (3,3), strides = 2)(X)
    X2 = Conv2D(filters = 16, kernel_size = (1,1), padding = 'same', activation = 'relu')(X2)
    X2 = Flatten()(X2)
    X2 = Dense(128, activation = 'relu')(X2)
    X2 = Dropout(0.7)(X2)
    X2 = Dense(25, activation = 'softmax')(X2)


    # 7th Inception block
    X = Inception_block(X, f1 = 32, f2_conv1 = 20, f2_conv3 = 40, f3_conv1 = 4, 
                      f3_conv5 = 16, f4 = 16)

    # max-pooling layer: pool_size = (3,3), strides = 2
    X = MaxPooling2D(pool_size = (2,2), strides = 1)(X)

    # 8th Inception block
    X = Inception_block(X, f1 = 32, f2_conv1 = 20, f2_conv3 = 40, f3_conv1 = 4, f3_conv5 = 16, f4 = 16)

    # 9th Inception block
    X = Inception_block(X, f1 = 43, f2_conv1 = 24, f2_conv3 = 43, f3_conv1 = 6, f3_conv5 = 16, f4 = 16)

    # Global Average pooling layer 
    X = GlobalAveragePooling2D(name = 'GAPL')(X)

    # Dropoutlayer 
    X = Dropout(0.4)(X)

    # output layer 
    X = Dense(25, activation = 'softmax')(X)

    # model
    model = Model(input_layer, [X, X1, X2], name = 'GoogLeNet')
    model.compile(loss=keras.losses.categorical_crossentropy,
                  optimizer=keras.optimizers.Adam(),
                  metrics=['accuracy'])
    model.summary()
    return model
# -


