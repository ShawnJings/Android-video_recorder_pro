# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
            ${CMAKE_SOURCE_DIR}/src/main/cpp/libcommon
            ${CMAKE_SOURCE_DIR}/src/main/cpp/video_consumer
            ${CMAKE_SOURCE_DIR}/src/main/cpp/audio_effect
            ${CMAKE_SOURCE_DIR}/src/main/cpp/liblivecore
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/ffmpeg/include
            )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3")

add_library(librecordprocessor STATIC libvocal_processor/record_corrector.cpp
            libvocal_processor/record_processor.cpp
            )

target_link_libraries(librecordprocessor

                libcommontool)