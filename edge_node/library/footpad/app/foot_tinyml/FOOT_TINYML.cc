/*
The tinyml model using lenet5 which classify left or right foot.
The mdoe of micro_mutable_op_resolver not all op resolver.
I dont know what is the perfect method to get the small size model between all resolver and micro resolver.
So, it needs to try to determinate perfect model.
And, it is a "FULL" QUANTIZATION MODEL.
create by Heo
date 2022-01-19
*/
#include <ubinos.h>
#include "FOOT_TINYML_MAIN_FUNCTIONS.h"
#include "FOOT_TINYML_TEST_DATA.h"
#include "FOOT_TINYML_MODEL_DATA.h"
#include "FOOT_HARD_ll.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace
{
  tflite::ErrorReporter *error_reporter = nullptr;
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *input = nullptr;
  TfLiteTensor *output = nullptr;
  int inference_count = 0;


  // Create an area of memory to use for input, output, and intermediate arrays.
  // The size of this will depend on the model you're using, and may need to be
  // determined by experimentation.
  constexpr int kTensorArenaSize = 60 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
} // namespace

// The name of this function is important for Arduino compatibility.
void setup()
{
  tflite::InitializeTarget();

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter; // NOLINT
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_foot_tinyml_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION)
  {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  static tflite::MicroMutableOpResolver<7> micro_op_resolver; // NOLINT
  micro_op_resolver.AddAveragePool2D();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddDequantize();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddQuantize();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddSoftmax();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk)
  {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  if ((input->dims->size != 4) || (input->dims->data[0] != 1) ||
      (input->dims->data[1] != 48) ||
      (input->dims->data[2] != 48) ||
      (input->type != kTfLiteInt8))
  {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");
    return;
  }

  output = interpreter->output(0);
}

#if (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 1)
void loop()
{
  int8_t y_quantized[X_TEST_TARGET_SIZE];
  float y[X_TEST_TARGET_SIZE];
   for (unsigned int i = 0; i < X_TEST_DATA_SIZE; i++)
  {
    float x = g_x_test_data[inference_count % X_TEST_DATA_COUNT][i];
    int8_t x_quantized = x / input->params.scale + input->params.zero_point;
    input->data.int8[i] = x_quantized;
  }

  // Run inference, and report any error.
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk)
  {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on index: \n");
    return;
  }

  for(unsigned int i = 0; i < X_TEST_TARGET_SIZE; i++){
    y_quantized[i]=output->data.int8[i];
    y[i] = (y_quantized[i] - output->params.zero_point) * output->params.scale;
  }
  // int8_t y_quantized_0 = output->data.int8[0]; 
  // int8_t y_quantized_1 = output->data.int8[1];

  // float y_0 = (y_quantized_0 - output->params.zero_point) * output->params.scale;
  // float y_1 = (y_quantized_1 - output->params.zero_point) * output->params.scale;
  // int ti = t_0 > t_1 ? 0 : 1;
  // int yi = y_0 > y_1 ? 0 : 1;
  printf("[%4d]  prediction ",inference_count);
  for(unsigned int i = 0; i < X_TEST_TARGET_SIZE; i++){
    if(i==X_TEST_TARGET_SIZE-1)
    printf("%f\n",y[i]);
    else
    printf("%f,",y[i]);
  }
  inference_count += 1;
}
#endif /* (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 1) */
#if (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 0)
void loop(char side,int target)
{
  int8_t y_quantized[X_TEST_TARGET_SIZE];
  float y[X_TEST_TARGET_SIZE];
  float max_num=0;
  int max_cnt=0;
  TfLiteStatus invoke_status;
   int miss_count=0;
  switch(side){
    case 'l':
    for(int i=0;i<48;i++){
      for(int j=0;j<48;j++){
        float x = (float)left_image[i][j]/255.0;
        int8_t x_quantized = x / input->params.scale + input->params.zero_point;
        input->data.int8[(i*48)+j] = x_quantized;
      }
    }
    // Run inference, and report any error.
  invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk)
  {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on index: \n");
    return;
  }
  for(unsigned int i = 0; i < X_TEST_TARGET_SIZE; i++){
    y_quantized[i]=output->data.int8[i];
    y[i] = (y_quantized[i] - output->params.zero_point) * output->params.scale;
    if(max_num<y[i]){
      max_num=y[i];
      max_cnt=i;
    }
  }
   printf("[%4d]  prediction ",inference_count);
  for(unsigned int i = 0; i < X_TEST_TARGET_SIZE; i++){
    if(i==X_TEST_TARGET_SIZE-1)
    printf("%f\n predict to %d ==>%f",y[i],max_cnt,max_num);
    else
    printf("%f,",y[i]);
  }
  inference_count += 1;
    break;

    case 'r':
    for(int i=0;i<48;i++){
      for(int j=0;j<48;j++){
        float x = (float)right_image[i][j]/255.0;
        int8_t x_quantized = x / input->params.scale + input->params.zero_point;
        input->data.int8[(i*48)+j] = x_quantized;
      }
    }
// Run inference, and report any error.
  invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk)
  {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on index: \n");
    return;
  }
  for(unsigned int i = 0; i < X_TEST_TARGET_SIZE; i++){
    y_quantized[i]=output->data.int8[i];
    y[i] = (y_quantized[i] - output->params.zero_point) * output->params.scale;
    if(max_num<y[i]){
      max_num=y[i];
      max_cnt=i;
    }
  }
  if (max_cnt != target){
    miss_count++;
  }
   printf("[%4d]  prediction ",inference_count);
  for(unsigned int i = 0; i < X_TEST_TARGET_SIZE; i++){
    if(i==X_TEST_TARGET_SIZE-1)
    printf("%f\n predict to %d ==>%f",y[i],max_cnt,max_num);
    else
    printf("%f,",y[i]);
  }
  inference_count += 1;
    break;
  }
}
#endif /* (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 0) */