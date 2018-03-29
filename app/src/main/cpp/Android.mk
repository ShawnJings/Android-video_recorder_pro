LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/3rdparty \
    $(LOCAL_PATH)/3rdparty/ffmpeg/include \
    $(LOCAL_PATH)/libcommon \
    $(LOCAL_PATH)/video_encoder \
    $(LOCAL_PATH)/libeditcore \
	$(LOCAL_PATH)/audio_effect \
	$(LOCAL_PATH)/video_consumer \
	$(LOCAL_PATH)/liblivecore 

ifeq ($(strip $(COMMIT_VERSION)), )
	LOCAL_CFLAGS += -DMODULE_VERSION_COMMIT_ID="$(shell cd $(LOCAL_PATH) && git rev-parse --short HEAD)"
else
	LOCAL_CFLAGS += -DMODULE_VERSION_COMMIT_ID="$(strip $(COMMIT_VERSION))"
endif

define ALL_JNI_LAYER_CPP_FILES
$(shell cd $(LOCAL_PATH); \
find . -maxdepth 5 -name "*.cpp" | grep jni)
endef
    
#$(warning $(ALL_JNI_LAYER_CPP_FILES))

LOCAL_SRC_FILES += $(ALL_JNI_LAYER_CPP_FILES)

LOCAL_STATIC_LIBRARIES := libcommontool lib3rdparty libaudioeffect
LOCAL_STATIC_LIBRARIES += libmusicdecoder librecordprocessor libcamerapreview 
LOCAL_STATIC_LIBRARIES += libvideoencoder
LOCAL_STATIC_LIBRARIES += libeditcore liblivecore 
LOCAL_STATIC_LIBRARIES += libvideoconsumer 

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libfdk-aac.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libavfilter.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libvo-aacenc.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libavformat.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libavcodec.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libavutil.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libswscale.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libswresample.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libpostproc.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libsox.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI)/libx264.a

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
# Link with OpenSL ES
LOCAL_LDLIBS += -lOpenSLES
# Link with OpenGL ES
LOCAL_LDLIBS += -lGLESv2
LOCAL_LDLIBS += -lz
#LOCAL_LDLIBS += -lgomp
LOCAL_LDLIBS += -landroid 
LOCAL_LDLIBS += -lEGL

LOCAL_LDLIBS += -L$(LOCAL_PATH)/3rdparty/prebuilt/$(TARGET_ARCH_ABI) -lfdk-aac -lvo-aacenc

LOCAL_MODULE := libsongstudio
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))