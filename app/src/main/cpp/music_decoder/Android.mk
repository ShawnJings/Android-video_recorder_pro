LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_STATIC_LIBRARIES := libresampler libaudioeffect

LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../libcommon \
	$(LOCAL_PATH)/../audio_effect \
	$(LOCAL_PATH)/../video_consumer \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 
    
LOCAL_SRC_FILES := \
./libmusic_decoder/aac_accompany_decoder.cpp \
./libmusic_decoder/song_decoder_controller.cpp 

LOCAL_MODULE := libmusicdecoder
include $(BUILD_STATIC_LIBRARY)