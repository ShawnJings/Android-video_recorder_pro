LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/libaudio_effect/audio_effect_filter_impl/pitch_shift/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../libcommon 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../3rdparty/ffmpeg/include 

SOUND_TOUCH_LIB_SOURCE = \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/AAFilter.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/BPMDetect.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/cpu_detect_x86_gcc.cpp\
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/FIFOSampleBuffer.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/FIRFilter.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/mmx_optimized.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/PeakFinder.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/RateTransposer.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/SoundTouch.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/sse_optimized.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/TDStretch.cpp 
 
HARMONIC_MODULE = \
./libaudio_effect/audio_effect_impl/harmonic/harmonic_audio_effect.cpp \
./libaudio_effect/audio_effect_impl/harmonic/harmonic_audio_effect_builder.cpp 

ROBOT_MODULE = \
./libaudio_effect/audio_effect_impl/robot/robot_audio_effect.cpp \
./libaudio_effect/audio_effect_impl/robot/robot_audio_effect_builder.cpp 

AUTO_TUNE_MODULE = \
./libaudio_effect/audio_effect_impl/auto_tune/auto_tune_audio_effect.cpp \
./libaudio_effect/audio_effect_impl/auto_tune/auto_tune_audio_effect_builder.cpp \
./libaudio_effect/audio_effect_filter_impl/delay_output/accompany_delay_output_effect_filter.cpp 
DOUBLE_U_MODULE = \
./libaudio_effect/audio_effect_filter_impl/double_u/double_u_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/double_u/doubleyou.cpp \
./libaudio_effect/audio_effect_filter_impl/double_u/eq.cpp \
./libaudio_effect/audio_effect_filter_impl/double_u/hsfeq.cpp \
./libaudio_effect/audio_effect_filter_impl/double_u/micro_pitchshift.cpp

ADJUST_ECHO_REVERB_MODULE = \
./libaudio_effect/audio_effect_impl/adjust_echo_reverb/adjust_echo_reverb_audio_effect_builder.cpp \
./libaudio_effect/audio_effect_impl/adjust_echo_reverb/adjust_echo_reverb_audio_effect.cpp

BASE_EFFECT_MODULES = \
./libaudio_effect/audio_effect/sox_effect_param/compressor_param_builder.cpp \
./libaudio_effect/audio_effect/sox_effect_param/sox_filter_chain_param.cpp \
./libaudio_effect/audio_effect/sox_effect_param/sox_filter_compressor_param.cpp \
./libaudio_effect/audio_effect/sox_effect_param/sox_filter_equalizer_param.cpp \
./libaudio_effect/audio_effect/sox_effect_param/sox_filter_reverb_param.cpp \
./libaudio_effect/audio_effect/audio_effect.cpp \
./libaudio_effect/audio_effect/audio_info.cpp \
./libaudio_effect/audio_effect/audio_effect_adapter.cpp \
./libaudio_effect/audio_effect/audio_effect_builder.cpp \
./libaudio_effect/audio_effect_filter/audio_effect_filter_factory.cpp \
./libaudio_effect/audio_effect_filter/audio_effect_filter_chain.cpp \
./libaudio_effect/audio_effect_filter_impl/agc/agc_stat_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/agc/vocal_agc_stat_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/agc/accompany_agc_stat_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/vocal_detector/vocal_detect.cpp \
./libaudio_effect/audio_effect_filter_impl/vocal_detector/vocal_detect_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/fade/fade_out_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/accompany_pitch_shift_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/pitch_shift_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/pitch_shift/vocal_pitch_shift_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/sox_compressor_effect.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/compressor_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/sox_equalizer_effect.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/equalizer_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/sox_reverb_effect.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/stereo_reverb_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/adjust_echo_reverb_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/reverb_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/sox_effect/sox_base_effect.cpp \
./libaudio_effect/audio_effect_filter_impl/specific/mix_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/specific/pitch_shift_mix_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/specific/mono_2_stereo_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/volume_adjust/accompany_agc_volume_adjust_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/volume_adjust/accompany_volume_adjust_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/volume_adjust/vocal_agc_volume_adjust_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/volume_adjust/vocal_volume_adjust_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/volume_adjust/accompany_volume_balence_adjust_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/volume_adjust/vocal_volume_balence_adjust_effect_filter.cpp \
./libaudio_effect/audio_effect_filter_impl/vocal_volume_detector/volume_monitor.cpp \
./libaudio_effect/audio_effect_filter_impl/vocal_volume_detector/vocal_volume_detect_effect_filter.cpp 

RESAMPLER_SOURCE = \
./libresampler/Resampler.cpp 

LOCAL_SRC_FILES = \
./libaudio_effect/audio_effect_processor/recording/audio_effect_recording_processor.cpp \
./libaudio_effect/audio_effect_processor/post/audio_effect_post_processor.cpp \
./libaudio_effect/audio_effect_processor/live/audio_effect_live_processor.cpp \
./libaudio_effect/audio_effect_processor/live/accompany_effect_processor.cpp \
./libaudio_effect/audio_effect_processor/audio_effect_processor_factory.cpp \
./libaudio_effect/audio_effect_processor/audio_request.cpp \
./libaudio_effect/audio_effect_processor/audio_response.cpp \
./libaudio_effect/audio_effect_processor/audio_effect_context.cpp 

LOCAL_SRC_FILES += $(BASE_EFFECT_MODULES)
LOCAL_SRC_FILES += $(AUTO_TUNE_MODULE)
LOCAL_SRC_FILES += $(DOUBLE_U_MODULE)
LOCAL_SRC_FILES += $(HARMONIC_MODULE)
LOCAL_SRC_FILES += $(ROBOT_MODULE)
LOCAL_SRC_FILES += $(SOUND_TOUCH_LIB_SOURCE)
LOCAL_SRC_FILES += $(ADJUST_ECHO_REVERB_MODULE)
LOCAL_SRC_FILES += $(RESAMPLER_SOURCE)
LOCAL_SRC_FILES += $(MEL_PARSER_SOURCE)

LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H

LOCAL_STATIC_LIBRARIES := libmelparser
LOCAL_ARM_MODE := arm
LOCAL_MODULE := libaudioeffect
include $(BUILD_STATIC_LIBRARY)

