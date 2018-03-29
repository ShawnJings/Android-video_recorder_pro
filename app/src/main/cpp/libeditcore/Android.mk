LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES := libcommontool 

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/libpng
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libpng
LOCAL_EXPORT_LDLIBS := -lz
    
PNG_LIB_SOURCE = ./3rdparty/libpng/png.c \
				 ./3rdparty/libpng/pngerror.c \
				 ./3rdparty/libpng/pngget.c \
				 ./3rdparty/libpng/pngmem.c \
				 ./3rdparty/libpng/pngpread.c \
				 ./3rdparty/libpng/pngread.c \
				 ./3rdparty/libpng/pngrio.c \
				 ./3rdparty/libpng/pngrtran.c \
				 ./3rdparty/libpng/pngrutil.c \
				 ./3rdparty/libpng/pngset.c \
				 ./3rdparty/libpng/pngtrans.c \
				 ./3rdparty/libpng/pngwio.c \
				 ./3rdparty/libpng/pngwrite.c \
				 ./3rdparty/libpng/pngwtran.c \
				 ./3rdparty/libpng/pngwutil.c

MARKUP_LIB_SOURCE = ./3rdparty/markup/Markup.cpp 

JSONCPP_LIB_SOURCE = ./3rdparty/jsoncpp/jsoncpp.cpp 

COMMON_MODULE_SOURCE = \
./common/opengl_video_frame.cpp \
./common/theme_parser.cpp \
./common/video_filter_utils.cpp \
./common/matrix.cpp

DECODER_MODULE_SOURCE = \
./decoder/rgba_frame.cpp \
./decoder/scene_video_decoder.cpp \
./decoder/image.c \
./decoder/png_decoder.cpp 

EDIT_MODEL_BASE_MODULE_SOURCE = \
./editmodel/model_filter.cpp \
./editmodel/model_track.cpp \
./editmodel/model_timeline.cpp 

EFFECT_BASE_MODULE_SOURCE = \
./videoeffect/video_effect_cache.cpp \
./videoeffect/base_video_effect.cpp \
./videoeffect/direct_pass_effect.cpp 

OVERLAY_SCENE_SOURCE = \
./editmodel/overlay_scene/overlay_scene_filter.cpp \
./videoeffect/overlay_scene/overlay_scene_effect.cpp 

HEADER_SCREEN_SCENE_SOURCE = \
./editmodel/header_screen_scene/header_screen_scene_filter.cpp \
./videoeffect/header_screen_scene/header_screen_scene_effect.cpp 

HEADER_FADE_SCENE_SOURCE = \
./editmodel/header_fade_scene/header_fade_scene_filter.cpp \
./videoeffect/header_fade_scene/header_fade_scene_effect.cpp 

MASK_ALPHA_SOURCE = \
./editmodel/mask_alpha/model_mask_alpha_filter.cpp \
./videoeffect/mask_alpha/mask_alpha_effect.cpp 

TEXT_SCENE_SOURCE = \
./editmodel/text_scene/text_scene_filter.cpp \
./videoeffect/text_scene/text_scene_effect.cpp 

GIF_SCENE_SOURCE = \
./editmodel/video_decoder_scene/model_decoder_filter.cpp \
./videoeffect/gif_scene/gif_scene_effect.cpp 

PNG_SEQUENCE_SOURCE = \
./editmodel/png_sequence/png_sequence_filter.cpp \
./videoeffect/png_sequence/png_sequence_effect.cpp 

SCREEN_SCENE_SOURCE = \
./videoeffect/screen_scene/screen_scene_effect.cpp 

VIDEO_OVERLAY_SCENE_SOURCE = \
./editmodel/video_overlay_scene/video_overlay_scene_filter.cpp \
./videoeffect/video_overlay_scene/video_overlay_scene_effect.cpp 

TRANSITION_SCENE_SOURCE = \
./editmodel/transition_scene/transition_scene_filter.cpp \
./videoeffect/transition_scene/transition_scene_effect.cpp 

IMAGE_EFFECT_SOURCE = \
./videoeffect/image_effect/image_base_effect.cpp \
./videoeffect/image_effect/thin/thin_whitening_effect.cpp \
./videoeffect/image_effect/thin/thin_effect.cpp \
./videoeffect/image_effect/hue_effect.cpp \
./videoeffect/image_effect/beautify_face/beautify_face_cool_effect.cpp \
./videoeffect/image_effect/beautify_face/beautify_face_effect.cpp \
./videoeffect/image_effect/beautify_face/smooth_skin_effect.cpp \
./videoeffect/image_effect/beautify_face/thin_beautify_face_effect.cpp \
./videoeffect/image_effect/whitening/whitening_cool_effect.cpp \
./videoeffect/image_effect/whitening/whitening_effect.cpp \
./videoeffect/image_effect/whitening/whitening_hor_effect.cpp \
./videoeffect/image_effect/whitening/whitening_ver_effect.cpp \
./videoeffect/image_effect/grayscale_effect.cpp \
./videoeffect/image_effect/mix_effect.cpp \
./videoeffect/image_effect/sepia_effect.cpp \
./videoeffect/image_effect/tone_curve_effect.cpp \
./videoeffect/image_effect/vignette_effect.cpp \
./videoeffect/image_effect/sharpen_effect.cpp \
./videoeffect/image_effect/image_effect_group.cpp \
./videoeffect/image_effect/highpass/high_pass_effect.cpp \
./videoeffect/image_effect/player_contrast_effect.cpp \
./videoeffect/image_effect/contrast_effect.cpp 

TRAILER_SCENE_SOURCE = \
./editmodel/trailer_scene/trailer_scene_filter.cpp \
./videoeffect/trailer_scene/trailer_scene_effect.cpp 

BLUR_SCENE_SOURCE = \
./editmodel/blur_scene/blur_scene_filter.cpp \
./editmodel/selective_blur_scene/selective_blur_scene_filter.cpp \
./videoeffect/blur_scene/selective_blur_scene_effect.cpp \
./videoeffect/blur_scene/blur_scene_effect.cpp 

SATURATION_SCENE_FILES = \
./videoeffect/saturation_scene/saturation_effect.cpp 

GHOST_SCENE_FILES = \
./videoeffect/image_effect/assistant_util/DelayedFrameTexture.cpp \
./videoeffect/image_effect/assistant_util/image_effect_graph.cpp \
./videoeffect/image_effect/assistant_util/image_mix_effect.cpp \
./videoeffect/image_effect/assistant_util/image_transform_effect.cpp \
./videoeffect/image_effect/ghost_scene/blur_mirror/image_blur_mirror_effect.cpp \
./videoeffect/image_effect/ghost_scene/carousel/image_carousel_effect.cpp \
./videoeffect/image_effect/ghost_scene/soul_stuff/image_soul_stuff_effect.cpp \
./videoeffect/image_effect/ghost_scene/tilt_shift/image_tilt_shift_effect.cpp \
./videoeffect/image_effect/ghost_scene/x_ray/image_color_invert_effect.cpp \
./videoeffect/image_effect/ghost_scene/image_2mirror_effect.cpp \
./videoeffect/image_effect/ghost_scene/image_4mirror_effect.cpp  \
./videoeffect/image_effect/time_stretch/time_stretch_effect.cpp

LOCAL_SRC_FILES = \
./gpu_texture_cache.cpp \
./video_effect_processor.cpp 



LOCAL_SRC_FILES += $(PNG_LIB_SOURCE)
LOCAL_SRC_FILES += $(MARKUP_LIB_SOURCE)
LOCAL_SRC_FILES += $(JSONCPP_LIB_SOURCE)
LOCAL_SRC_FILES += $(COMMON_MODULE_SOURCE)
LOCAL_SRC_FILES += $(DECODER_MODULE_SOURCE)
LOCAL_SRC_FILES += $(EDIT_MODEL_BASE_MODULE_SOURCE)
LOCAL_SRC_FILES += $(EFFECT_BASE_MODULE_SOURCE)
LOCAL_SRC_FILES += $(OVERLAY_SCENE_SOURCE)
LOCAL_SRC_FILES += $(MASK_ALPHA_SOURCE)
LOCAL_SRC_FILES += $(GIF_SCENE_SOURCE)
LOCAL_SRC_FILES += $(TEXT_SCENE_SOURCE)
LOCAL_SRC_FILES += $(SCREEN_SCENE_SOURCE)
LOCAL_SRC_FILES += $(VIDEO_OVERLAY_SCENE_SOURCE)
LOCAL_SRC_FILES += $(TRANSITION_SCENE_SOURCE)
LOCAL_SRC_FILES += $(TRAILER_SCENE_SOURCE)
LOCAL_SRC_FILES += $(PNG_SEQUENCE_SOURCE)
LOCAL_SRC_FILES += $(HEADER_SCREEN_SCENE_SOURCE)
LOCAL_SRC_FILES += $(HEADER_FADE_SCENE_SOURCE)
LOCAL_SRC_FILES += $(BLUR_SCENE_SOURCE)
LOCAL_SRC_FILES += $(IMAGE_EFFECT_SOURCE)
LOCAL_SRC_FILES += $(SATURATION_SCENE_FILES)
LOCAL_SRC_FILES += $(GHOST_SCENE_FILES)

LOCAL_MODULE := libeditcore
include $(BUILD_STATIC_LIBRARY)