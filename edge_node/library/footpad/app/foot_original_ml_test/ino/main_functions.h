#ifndef FOOT_ORIGINAL_ML_TEST_MAIN_FUNCTIONS_H_
#define FOOT_ORIGINAL_ML_TEST_MAIN_FUNCTIONS_H_

// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

// Initializes all data needed for the example. The name is important, and needs
// to be setup() for Arduino compatibility.
void setup();

// Runs one iteration of data gathering and inference. This should be called
// repeatedly from the application code. The name needs to be loop() for Arduino
// compatibility.
void loop();

#ifdef __cplusplus
}
#endif

#endif  // FOOT_ORIGINAL_ML_TEST_MAIN_FUNCTIONS_H_
