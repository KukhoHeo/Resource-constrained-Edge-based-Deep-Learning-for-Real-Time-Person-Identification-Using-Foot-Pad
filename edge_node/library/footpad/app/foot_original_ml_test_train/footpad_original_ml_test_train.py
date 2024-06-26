import numpy as np
from Csv_Formatter import Search, Concate
from Check_Feet import *
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras.callbacks import EarlyStopping
from sklearn.preprocessing import OneHotEncoder
from sklearn.model_selection import train_test_split

####

foot,target=Concate(Search("./data/"))

left,right=preprocess(foot, show=False)

foot=[]
target=[]
for i in range(len(left)):
    foot.append(left[i]/255)
    target.append([0])
for i in range(len(right)):
    foot.append(right[i]/255)
    target.append([1])

onehot_encoder = OneHotEncoder()
target_onehot=onehot_encoder.fit_transform(target).toarray()

foot=np.array(foot).astype(np.float32)
target_onehot=np.array(target_onehot)

####

import os
MODELS_DIR = 'testmodels/'
if not os.path.exists(MODELS_DIR):
    os.mkdir(MODELS_DIR)
MODEL_TF = MODELS_DIR + 'model'
MODEL_NO_QUANT_TFLITE = MODELS_DIR + 'model_no_quant.tflite'
MODEL_QUANT_TFLITE = MODELS_DIR + 'model_quant.tflite'
MODEL_TFLITE = MODELS_DIR + 'model.tflite'
MODEL_TFLITE_MICRO = MODELS_DIR + 'model.cc'

##example model
x_train, x_test, t_train, t_test = train_test_split(foot, target_onehot, test_size=0.2, shuffle=True, stratify=target)

TEST_DATA_HDR_FILE_NAME = "test_data.h"
TEST_DATA_HDR_PATH = MODELS_DIR + TEST_DATA_HDR_FILE_NAME
TEST_DATA_SRC_PATH = MODELS_DIR + "test_data.cc"

with open(TEST_DATA_HDR_PATH, "w") as file_object:
    line = "#define X_TEST_DATA_COUNT {}\n".format(len(x_test))
    file_object.write(line)
    line = "#define X_TEST_DATA_SIZE {}\n".format(len(x_test[0]) * len(x_test[0][0]))
    file_object.write(line)
    line = "\n"
    file_object.write(line)

    line = "extern const float g_x_test_data[X_TEST_DATA_COUNT][X_TEST_DATA_SIZE];\n"
    file_object.write(line)
    line = "\n"
    file_object.write(line)

    line = "#define X_TEST_TARGET_SIZE {}\n".format(len(t_test[0]))
    file_object.write(line)
    line = "\n"
    file_object.write(line)

    line = "extern const float g_x_test_target[X_TEST_DATA_COUNT][X_TEST_TARGET_SIZE];\n"
    file_object.write(line)
    line = "\n"
    file_object.write(line)

with open(TEST_DATA_SRC_PATH, "w") as file_object:
    line = "#include \"{}\"\n".format(TEST_DATA_HDR_FILE_NAME)
    file_object.write(line)
    line = "\n"
    file_object.write(line)

    line ="alignas(8) const float g_x_test_data[X_TEST_DATA_COUNT][X_TEST_DATA_SIZE] = {\n"
    file_object.write(line)
    for i in range(len(x_test)):
        line = "{{ // {}: ({}, {})\n".format(i, t_test[i][0], t_test[i][1])
        file_object.write(line)
        for y in range(len(x_test[i])):
            for x in range(len(x_test[i][y])):
                # if x_test[i][y][x][0] == 0.:
                #     line = " , "
                # else:
                #     line = "1, "
                line = ("{}, ".format(x_test[i][y][x][0]))
                file_object.write(line)
            line = "\n"
            file_object.write(line)
        line = "},\n"
        file_object.write(line)
    line = "};\n\n"
    file_object.write(line)

    line ="alignas(8) const float g_x_test_target[X_TEST_DATA_COUNT][X_TEST_TARGET_SIZE] = {\n"
    file_object.write(line)
    for i in range(len(t_test)):
        line = "{"
        file_object.write(line)
        for y in range(len(t_test[i])):
            line = ("{}, ".format(t_test[i][y]))
            file_object.write(line)
        line = "},\n"
        file_object.write(line)
    line = "};\n\n"
    file_object.write(line)

model = keras.Sequential()
model.add(keras.layers.Conv2D(filters=6, kernel_size=(5, 5), activation='relu', input_shape=(48,48,1)))
#model.add(keras.layers.Conv2D(filters=12, kernel_size=(5, 5), activation='relu'))
model.add(keras.layers.AveragePooling2D())
model.add(keras.layers.Conv2D(filters=16, kernel_size=(5, 5), activation='relu'))
#model.add(keras.layers.Conv2D(filters=24, kernel_size=(3, 3), activation='relu'))
model.add(keras.layers.AveragePooling2D())
model.add(keras.layers.Flatten())
model.add(keras.layers.Dense(units=120, activation='relu'))
model.add(keras.layers.Dense(units=84, activation='relu'))
model.add(keras.layers.Dense(units=2, activation = 'softmax'))
model.compile(loss=keras.losses.categorical_crossentropy, optimizer=keras.optimizers.Adam(), metrics=['accuracy'])
print("tensorflow version = ",tf.__version__)
print("keras version = ",keras.__version__)
model.summary()
EPOCHS = 100
BATCH_SIZE = 16
early_stopping = EarlyStopping(monitor='val_loss',verbose=1,min_delta=0,patience=2,restore_best_weights=True)
history =model.fit(x_train,t_train,epochs=EPOCHS, batch_size=BATCH_SIZE,validation_data=(x_test, t_test),callbacks=[early_stopping])

###

model.save(MODEL_TF)

# Convert the model to the TensorFlow Lite format without quantization
converter = tf.lite.TFLiteConverter.from_saved_model(MODEL_TF)
model_no_quant_tflite = converter.convert()

# Save the model to disk
open(MODEL_NO_QUANT_TFLITE, "wb").write(model_no_quant_tflite)

# Convert the model to the TensorFlow Lite format with quantization
def representative_dataset_gen():
    for i in range(len(x_train)):
        data = x_train[i]
        yield [data.reshape(1, 48, 48, 1)]

converter = tf.lite.TFLiteConverter.from_saved_model(MODEL_TF)
# Set the optimization flag.
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8
# Provide a representative dataset to ensure we quantize correctly.
converter.representative_dataset = representative_dataset_gen
model_tflite = converter.convert()

# Save the model to disk
open(MODEL_TFLITE, "wb").write(model_tflite)

def predict_tflite(tflite_model, x_test):
    # Prepare the test data
    x_test_ = x_test.copy()

    # Initialize the TFLite interpreter
    interpreter = tf.lite.Interpreter(model_content=tflite_model)
    interpreter.allocate_tensors()
    input_details = interpreter.get_input_details()[0]
    output_details = interpreter.get_output_details()[0]

    print(input_details)
    print(output_details)
    # If required, quantize the input layer (from float to integer)
    input_scale, input_zero_point = input_details["quantization"]
    if (input_scale, input_zero_point) != (0.0, 0):
        x_test_ = x_test_ / input_scale + input_zero_point
        x_test_ = x_test_.astype(input_details["dtype"])

    # Invoke the interpreter
    y_pred = np.empty([x_test.shape[0],2], dtype=output_details["dtype"])

    for i in range(len(x_test_)):
        interpreter.set_tensor(input_details["index"], [x_test_[i]])
        interpreter.invoke()
        y_pred[i] = interpreter.get_tensor(output_details["index"])[0]

    # If required, dequantized the output layer (from integer to float)
    output_scale, output_zero_point = output_details["quantization"]
    if (output_scale, output_zero_point) != (0.0, 0):
        y_pred = y_pred.astype(np.float32)
        y_pred = (y_pred - output_zero_point) * output_scale
    return y_pred

def evaluate_tflite(tflite_model, x_test, y_true):
    global model
    y_pred = predict_tflite(tflite_model, x_test)
    loss_function = tf.keras.losses.get(model.loss)
    loss = loss_function(y_true, y_pred).numpy()
    return loss

# Calculate predictions
y_test_pred_tf = model.predict(x_test)
y_test_pred_no_quant_tflite = predict_tflite(model_no_quant_tflite, x_test)
y_test_pred_tflite = predict_tflite(model_tflite, x_test)

print(t_test)
print(y_test_pred_tflite)

def make_one_hot(predict):
    for i in range(len(predict)):
        if predict[i][0]>predict[i][1]:
            predict[i][0]=1
            predict[i][1]=0
        else:
            predict[i][0]=0
            predict[i][1]=1
    return predict

onehot_y_test_pred_tf=make_one_hot(y_test_pred_tf)
onehot_y_test_pred_no_quant_tflite=make_one_hot(y_test_pred_no_quant_tflite)
onehot_y_test_pred_tflite=make_one_hot(y_test_pred_tflite)

def accuracy(predict,target):
    acc_count=int(0)
    for i in range(len(predict)):
        if np.array_equal(predict[i],target[i]):
            acc_count+=1
    print("total count %d correct count %d"%(len(predict),acc_count))

accuracy(onehot_y_test_pred_tf,t_test)
accuracy(onehot_y_test_pred_no_quant_tflite,t_test)
accuracy(onehot_y_test_pred_tflite,t_test)

# xxd -i model.tflite > model_data.cc

