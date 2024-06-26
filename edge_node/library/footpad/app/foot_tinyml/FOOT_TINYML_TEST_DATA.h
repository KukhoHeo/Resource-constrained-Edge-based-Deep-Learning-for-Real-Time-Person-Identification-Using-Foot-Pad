#include <ubinos.h>
#define X_TEST_TARGET_SIZE 25
#define X_TEST_DATA_COUNT 5
#define X_TEST_DATA_SIZE 2304
#if (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 1)
extern const float g_x_test_data[X_TEST_DATA_COUNT][X_TEST_DATA_SIZE];
extern const float g_x_test_target[X_TEST_DATA_COUNT][X_TEST_TARGET_SIZE];
#endif /* (FOOT_ORIGINAL__USE_FOOT_TINYML_TEST == 1) */