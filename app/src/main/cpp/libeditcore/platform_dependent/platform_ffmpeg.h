#ifndef PLATFORM_FFMPEG
#define PLATFORM_FFMPEG

// ffmpeg
#ifdef __ANDROID__
extern "C" {
#include "./../../3rdparty/ffmpeg/include/libavutil/imgutils.h"
#include "./../../3rdparty/ffmpeg/include/libswscale/swscale.h"
#include "./../../3rdparty/ffmpeg/include/libswresample/swresample.h"
#include "./../../3rdparty/ffmpeg/include/libavutil/samplefmt.h"
#include "./../../3rdparty/ffmpeg/include/libavutil/timestamp.h"
#include "./../../3rdparty/ffmpeg/include/libavformat/avformat.h"
#include "./../../3rdparty/ffmpeg/include/libswscale/swscale.h"
}

#elif defined(__APPLE__)	// IOS or OSX
extern "C" {
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#endif

#endif	// PLATFORM_FFMPEG
