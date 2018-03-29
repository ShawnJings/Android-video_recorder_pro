# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})




include_directories(
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/ffmpeg/include 
            ${CMAKE_SOURCE_DIR}/src/main/cpp/libeditcore/3rdparty/libpng
            )

# libpng存疑,define PNG_ARM_NEON_OPT 2=>0,暂时禁用neon优化
if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
set(ENABLE_NEON ON)
endif()

add_library(libeditcore STATIC
            3rdparty/libpng/png.c
            3rdparty/libpng/pngerror.c
            3rdparty/libpng/pngget.c
            3rdparty/libpng/pngmem.c
            3rdparty/libpng/pngpread.c
            3rdparty/libpng/pngread.c
            3rdparty/libpng/pngrio.c
            3rdparty/libpng/pngrtran.c
            3rdparty/libpng/pngrutil.c
            3rdparty/libpng/pngset.c
            3rdparty/libpng/pngtrans.c
            3rdparty/libpng/pngwio.c
            3rdparty/libpng/pngwrite.c
            3rdparty/libpng/pngwtran.c
            3rdparty/libpng/pngwutil.c

            3rdparty/markup/Markup.cpp

            3rdparty/jsoncpp/jsoncpp.cpp

            common/opengl_video_frame.cpp
            common/theme_parser.cpp
            common/video_filter_utils.cpp
            common/matrix.cpp

            decoder/rgba_frame.cpp
            decoder/scene_video_decoder.cpp
            decoder/image.c
            decoder/png_decoder.cpp

            editmodel/model_filter.cpp
            editmodel/model_track.cpp
            editmodel/model_timeline.cpp

            videoeffect/video_effect_cache.cpp
            videoeffect/base_video_effect.cpp
            videoeffect/direct_pass_effect.cpp

            editmodel/overlay_scene/overlay_scene_filter.cpp
            videoeffect/overlay_scene/overlay_scene_effect.cpp

            editmodel/header_screen_scene/header_screen_scene_filter.cpp
            videoeffect/header_screen_scene/header_screen_scene_effect.cpp

            editmodel/header_fade_scene/header_fade_scene_filter.cpp
            videoeffect/header_fade_scene/header_fade_scene_effect.cpp

            editmodel/mask_alpha/model_mask_alpha_filter.cpp
            videoeffect/mask_alpha/mask_alpha_effect.cpp

            editmodel/text_scene/text_scene_filter.cpp
            videoeffect/text_scene/text_scene_effect.cpp

            editmodel/video_decoder_scene/model_decoder_filter.cpp
            videoeffect/gif_scene/gif_scene_effect.cpp

            editmodel/png_sequence/png_sequence_filter.cpp
            videoeffect/png_sequence/png_sequence_effect.cpp

            videoeffect/screen_scene/screen_scene_effect.cpp

            editmodel/video_overlay_scene/video_overlay_scene_filter.cpp
            videoeffect/video_overlay_scene/video_overlay_scene_effect.cpp

            editmodel/transition_scene/transition_scene_filter.cpp
            videoeffect/transition_scene/transition_scene_effect.cpp

            videoeffect/image_effect/image_base_effect.cpp
            videoeffect/image_effect/thin/thin_whitening_effect.cpp
            videoeffect/image_effect/thin/thin_effect.cpp
            videoeffect/image_effect/hue_effect.cpp
            videoeffect/image_effect/beautify_face/beautify_face_cool_effect.cpp
            videoeffect/image_effect/beautify_face/beautify_face_effect.cpp
            videoeffect/image_effect/beautify_face/smooth_skin_effect.cpp
            videoeffect/image_effect/beautify_face/thin_beautify_face_effect.cpp
            videoeffect/image_effect/whitening/whitening_cool_effect.cpp
            videoeffect/image_effect/whitening/whitening_effect.cpp
            videoeffect/image_effect/whitening/whitening_hor_effect.cpp
            videoeffect/image_effect/whitening/whitening_ver_effect.cpp
            videoeffect/image_effect/grayscale_effect.cpp
            videoeffect/image_effect/mix_effect.cpp
            videoeffect/image_effect/sepia_effect.cpp
            videoeffect/image_effect/tone_curve_effect.cpp
            videoeffect/image_effect/vignette_effect.cpp
            videoeffect/image_effect/sharpen_effect.cpp
            videoeffect/image_effect/image_effect_group.cpp
            videoeffect/image_effect/highpass/high_pass_effect.cpp
            videoeffect/image_effect/player_contrast_effect.cpp
            videoeffect/image_effect/contrast_effect.cpp

            editmodel/trailer_scene/trailer_scene_filter.cpp
            videoeffect/trailer_scene/trailer_scene_effect.cpp

            editmodel/blur_scene/blur_scene_filter.cpp
            editmodel/selective_blur_scene/selective_blur_scene_filter.cpp
            videoeffect/blur_scene/selective_blur_scene_effect.cpp
            videoeffect/blur_scene/blur_scene_effect.cpp

            videoeffect/saturation_scene/saturation_effect.cpp

            videoeffect/image_effect/assistant_util/DelayedFrameTexture.cpp
            videoeffect/image_effect/assistant_util/image_effect_graph.cpp
            videoeffect/image_effect/assistant_util/image_mix_effect.cpp
            videoeffect/image_effect/assistant_util/image_transform_effect.cpp
            videoeffect/image_effect/ghost_scene/blur_mirror/image_blur_mirror_effect.cpp
            videoeffect/image_effect/ghost_scene/carousel/image_carousel_effect.cpp
            videoeffect/image_effect/ghost_scene/soul_stuff/image_soul_stuff_effect.cpp
            videoeffect/image_effect/ghost_scene/tilt_shift/image_tilt_shift_effect.cpp
            videoeffect/image_effect/ghost_scene/x_ray/image_color_invert_effect.cpp
            videoeffect/image_effect/ghost_scene/image_2mirror_effect.cpp
            videoeffect/image_effect/ghost_scene/image_4mirror_effect.cpp
            videoeffect/image_effect/time_stretch/time_stretch_effect.cpp

            gpu_texture_cache.cpp
            video_effect_processor.cpp
            )

target_link_libraries(libeditcore

                libcommontool
                z)