
#pragma once

#define FILENAME_ONLY (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define LOG_I(formatString, ...) \
    do { \
        printf("INFO %s:%d: ", FILENAME_ONLY, __LINE__); \
        printf(formatString, ##__VA_ARGS__); \
        printf("\n"); \
    } while(0)

#define LOG_F(formatString, ...) \
    do { \
        printf("FATAL %s:%d: ", FILENAME_ONLY, __LINE__); \
        printf(formatString, ##__VA_ARGS__); \
        printf("\n"); \
    } while(0)