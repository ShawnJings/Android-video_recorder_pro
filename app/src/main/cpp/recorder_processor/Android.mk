LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3
LOCAL_STATIC_LIBRARIES := libcommontool
    
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../libcommon \
    $(LOCAL_PATH)/../video_consumer \
	$(LOCAL_PATH)/../audio_effect \
	$(LOCAL_PATH)/../liblivecore \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 
    
LOCAL_SRC_FILES = \
./libvocal_processor/record_corrector.cpp \
./libvocal_processor/record_processor.cpp 

LOCAL_MODULE := librecordprocessor
include $(BUILD_STATIC_LIBRARY)
