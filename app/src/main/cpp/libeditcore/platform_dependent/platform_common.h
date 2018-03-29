/*
 * code here will be compiled according to platforms, for android and IOS, please make your own implement
 */

#ifndef PLATFORM_COMMON
#define PLATFORM_COMMON

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

typedef unsigned char byte;

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

namespace platform {
static inline long getCurrentTime()
{
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

}

// log
#ifdef __ANDROID__
#include <android/log.h>
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#elif defined(__APPLE__)	// IOS or OSX
#define LOGI(...)  printf("  ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#define LOGE(...)  printf(" Error: ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#endif



#endif	// PLATFORM_COMMON


