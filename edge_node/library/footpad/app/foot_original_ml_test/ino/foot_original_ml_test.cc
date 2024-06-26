#include "main_functions.h"

#include "model_data.h"
#include "test_data.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"


// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int input_length;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 60 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  tflite::InitializeTarget();

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_foot_original_ml_test_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
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
  static tflite::MicroMutableOpResolver<7> micro_op_resolver;  // NOLINT
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
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  if ((input->dims->size != 4) || (input->dims->data[0] != 1) ||
      (input->dims->data[1] != 48) ||
      (input->dims->data[2] != 48) ||
      (input->type != kTfLiteInt8)) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");
    return;
  }

  input_length = input->bytes / sizeof(int8_t);

  output = interpreter->output(0);
}

// The name of this function is important for Arduino compatibility.
void loop() {
  for (unsigned int i = 0; i < X_TEST_DATA_SIZE; i++) {
    float x = g_x_test_data[inference_count % X_TEST_DATA_COUNT][i];
    int8_t x_quantized = x / input->params.scale + input->params.zero_point;
    input->data.int8[i] = x_quantized;
  }

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n", inference_count);
    return;
  }

  float t_0 = g_x_test_target[inference_count % X_TEST_DATA_COUNT][0];
  float t_1 = g_x_test_target[inference_count % X_TEST_DATA_COUNT][1];
  int8_t y_quantized_0 = output->data.int8[0];
  int8_t y_quantized_1 = output->data.int8[1];
  float y_0 = (y_quantized_0 - output->params.zero_point) * output->params.scale;
  float y_1 = (y_quantized_1 - output->params.zero_point) * output->params.scale;

  int ti = t_0 > t_1 ? 0 : 1;
  int yi = y_0 > y_1 ? 0 : 1;
  printf("[%4d] target (%f, %f), prediction (%f, %f) : ", inference_count, t_0, t_1, y_0, y_1);
  if (ti == yi) {
    printf("correct\n");
  }
  else {
    printf("wrong\n");
  }

  inference_count += 1;
}
