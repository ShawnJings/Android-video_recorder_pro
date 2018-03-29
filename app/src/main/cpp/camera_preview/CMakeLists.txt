# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/ffmpeg/include
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/senseme/include
            ${CMAKE_SOURCE_DIR}/src/main/cpp/libcommon
            ${CMAKE_SOURCE_DIR}/src/main/cpp/video_consumer
            ${CMAKE_SOURCE_DIR}/src/main/cpp/liblivecore
            ${CMAKE_SOURCE_DIR}/src/main/cpp/video_encoder
            ${CMAKE_SOURCE_DIR}/src/main/cpp/libeditcore)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES")

add_library(libcamerapreview STATIC libcamera_preview/recording_preview_renderer.cpp
            	libcamera_preview/mv_recording_preview_controller.cpp)

target_link_libraries(libcamerapreview

            libcommontool
            libvideoencoder
            libeditcore)