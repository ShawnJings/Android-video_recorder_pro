# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/ffmpeg/include 
            )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3")

add_library(liblivecore STATIC common/live_thread.cpp
            common/publisher_statistics.cpp
            common/publisher_rate_feed_back.cpp
            common/live_packet_pool.cpp
            common/live_audio_packet_pool.cpp
            common/live_video_packet_queue.cpp
            common/live_audio_packet_queue.cpp
            audio_encoder/audio_encoder_adapter.cpp
            audio_encoder/audio_encoder.cpp
            publisher/recording_h264_publisher.cpp
            publisher/recording_publisher.cpp
            consumer/video_consumer_thread.cpp
            common/live_video_quality_agent.cpp )

target_link_libraries(liblivecore

                libcommontool)