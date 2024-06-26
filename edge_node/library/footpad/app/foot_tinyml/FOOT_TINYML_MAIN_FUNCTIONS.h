/*
create by Heo
date 2022-01-19
*/
#ifndef FOOT_TINYML_FOOT_MAIN_FUNCTIONS_H_
#define FOOT_TINYML_FOOT_MAIN_FUNCTIONS_H_

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
// void loop(char);
#if (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 1)
void loop();
#endif /* (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 1) */
#if (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 0)
void loop(char side,int target);
#endif /* (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 0) */
#ifdef __cplusplus
}
#endif

#endif  // FOOT_TINYML_FOOT_MAIN_FUNCTIONS_H_
