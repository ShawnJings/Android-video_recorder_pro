LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3
LOCAL_STATIC_LIBRARIES := liblivecore libcolorconversion libmusicmerger libresampler

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../libcommon \
	$(LOCAL_PATH)/../liblivecore \
	$(LOCAL_PATH)/../audio_effect \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 
    
LOCAL_SRC_FILES = \
./libmusic_merger/music_merger.cpp \
./libvideo_consumer/live_common_packet_pool.cpp \
./libaudio_encoder/audio_process_encoder_adapter.cpp \
./libvideo_consumer/video_packet_consumer.cpp 

LOCAL_MODULE := libvideoconsumer
include $(BUILD_STATIC_LIBRARY)
