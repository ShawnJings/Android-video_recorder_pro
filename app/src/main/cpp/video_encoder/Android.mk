LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := libcommontool

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../libcommon \
    $(LOCAL_PATH)/../video_consumer \
	$(LOCAL_PATH)/../liblivecore \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 
    
LOCAL_CFLAGS +=  \
	-DGL_GLEXT_PROTOTYPES\
	-DEGL_EGLEXT_PROTOTYPES
    
LOCAL_SRC_FILES = \
	./libvideo_encoder/video_encoder_adapter.cpp \
	./libvideo_encoder/soft_encoder/soft_encoder_adapter.cpp \
	./libvideo_encoder/soft_encoder/host_gpu_copier.cpp \
	./libvideo_encoder/soft_encoder/live_yuy2_packet_pool.cpp \
	./libvideo_encoder/soft_encoder/video_x264_encoder.cpp \
	./libvideo_encoder/hw_encoder/hw_encoder_adapter.cpp 

# include the right version cpp that matches the CPU architecture 
ARCH_SRC_FILES := ./libvideo_encoder/soft_encoder/color_conversion/color_conversion_default.cpp	# mips and armeabi 

ifeq ($(TARGET_ARCH_ABI), x86)	# x86
ARCH_SRC_FILES := ./libvideo_encoder/soft_encoder/color_conversion/color_conversion_x86.cpp
endif

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)	# armeabi-v7a
LOCAL_CFLAGS   := -DHAVE_CONFIG_H -mfloat-abi=softfp -mfpu=neon -march=armv7-a
ARCH_SRC_FILES := ./libvideo_encoder/soft_encoder/color_conversion/color_conversion_armv7a.cpp
LOCAL_ARM_NEON := true
endif

LOCAL_SRC_FILES += $(ARCH_SRC_FILES)

LOCAL_MODULE := libvideoencoder
include $(BUILD_STATIC_LIBRARY)
