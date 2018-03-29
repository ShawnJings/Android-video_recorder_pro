# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/ffmpeg/include
            ${CMAKE_SOURCE_DIR}/src/main/cpp/libcommon
            ${CMAKE_SOURCE_DIR}/src/main/cpp/video_consumer
            ${CMAKE_SOURCE_DIR}/src/main/cpp/liblivecore)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES")


SET(ARCH_SRC_FILES libvideo_encoder/soft_encoder/color_conversion/color_conversion_default.cpp)

if(${ANDROID_ABI} STREQUAL "x86")

SET(ARCH_SRC_FILES libvideo_encoder/soft_encoder/color_conversion/color_conversion_x86.cpp)

endif(${ANDROID_ABI} STREQUAL "x86")

if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
SET(CMAKE_C_FLAGS "-DHAVE_CONFIG_H -mfloat-abi=softfp -mfpu=neon -march=armv7-a")
SET(CMAKE_CPP_FLAGS "-DHAVE_CONFIG_H -mfloat-abi=softfp -mfpu=neon -march=armv7-a")
SET(ARCH_SRC_FILES libvideo_encoder/soft_encoder/color_conversion/color_conversion_armv7a.cpp)
SET(ENABLE_NEON ON)
endif(${ANDROID_ABI} STREQUAL "armeabi-v7a")

add_library(libvideoencoder STATIC libvideo_encoder/video_encoder_adapter.cpp
            libvideo_encoder/soft_encoder/soft_encoder_adapter.cpp
            libvideo_encoder/soft_encoder/host_gpu_copier.cpp
            libvideo_encoder/soft_encoder/live_yuy2_packet_pool.cpp
            libvideo_encoder/soft_encoder/video_x264_encoder.cpp
            libvideo_encoder/hw_encoder/hw_encoder_adapter.cpp

            ${ARCH_SRC_FILES}
            )

target_link_libraries(libvideoencoder

                libcommontool)