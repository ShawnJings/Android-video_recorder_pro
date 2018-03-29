# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/audio_effect/libaudio_effect/audio_effect_filter_impl/pitch_shift/include
            ${CMAKE_SOURCE_DIR}/src/main/cpp/libcommon
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rdparty/ffmpeg/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT='' -UHAVE_CONFIG_H")

add_library(libaudioeffect STATIC
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/AAFilter.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/BPMDetect.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/cpu_detect_x86_gcc.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/FIFOSampleBuffer.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/FIRFilter.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/mmx_optimized.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/PeakFinder.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/RateTransposer.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/SoundTouch.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/sse_optimized.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/SoundTouch/TDStretch.cpp

                libaudio_effect/audio_effect_impl/harmonic/harmonic_audio_effect.cpp
                libaudio_effect/audio_effect_impl/harmonic/harmonic_audio_effect_builder.cpp

                libaudio_effect/audio_effect_impl/robot/robot_audio_effect.cpp
                libaudio_effect/audio_effect_impl/robot/robot_audio_effect_builder.cpp

                libaudio_effect/audio_effect_impl/auto_tune/auto_tune_audio_effect.cpp
                libaudio_effect/audio_effect_impl/auto_tune/auto_tune_audio_effect_builder.cpp
                libaudio_effect/audio_effect_filter_impl/delay_output/accompany_delay_output_effect_filter.cpp

                libaudio_effect/audio_effect_filter_impl/double_u/double_u_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/double_u/doubleyou.cpp
                libaudio_effect/audio_effect_filter_impl/double_u/eq.cpp
                libaudio_effect/audio_effect_filter_impl/double_u/hsfeq.cpp
                libaudio_effect/audio_effect_filter_impl/double_u/micro_pitchshift.cpp

                libaudio_effect/audio_effect_impl/adjust_echo_reverb/adjust_echo_reverb_audio_effect_builder.cpp
                libaudio_effect/audio_effect_impl/adjust_echo_reverb/adjust_echo_reverb_audio_effect.cpp

                libaudio_effect/audio_effect/sox_effect_param/compressor_param_builder.cpp
                libaudio_effect/audio_effect/sox_effect_param/sox_filter_chain_param.cpp
                libaudio_effect/audio_effect/sox_effect_param/sox_filter_compressor_param.cpp
                libaudio_effect/audio_effect/sox_effect_param/sox_filter_equalizer_param.cpp
                libaudio_effect/audio_effect/sox_effect_param/sox_filter_reverb_param.cpp
                libaudio_effect/audio_effect/audio_effect.cpp
                libaudio_effect/audio_effect/audio_info.cpp
                libaudio_effect/audio_effect/audio_effect_adapter.cpp
                libaudio_effect/audio_effect/audio_effect_builder.cpp
                libaudio_effect/audio_effect_filter/audio_effect_filter_factory.cpp
                libaudio_effect/audio_effect_filter/audio_effect_filter_chain.cpp
                libaudio_effect/audio_effect_filter_impl/agc/agc_stat_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/agc/vocal_agc_stat_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/agc/accompany_agc_stat_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/vocal_detector/vocal_detect.cpp
                libaudio_effect/audio_effect_filter_impl/vocal_detector/vocal_detect_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/fade/fade_out_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/accompany_pitch_shift_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/pitch_shift_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/pitch_shift/vocal_pitch_shift_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/sox_compressor_effect.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/compressor_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/sox_equalizer_effect.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/equalizer_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/sox_reverb_effect.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/stereo_reverb_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/adjust_echo_reverb_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/reverb_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/sox_effect/sox_base_effect.cpp
                libaudio_effect/audio_effect_filter_impl/specific/mix_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/specific/pitch_shift_mix_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/specific/mono_2_stereo_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/volume_adjust/accompany_agc_volume_adjust_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/volume_adjust/accompany_volume_adjust_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/volume_adjust/vocal_agc_volume_adjust_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/volume_adjust/vocal_volume_adjust_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/volume_adjust/accompany_volume_balence_adjust_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/volume_adjust/vocal_volume_balence_adjust_effect_filter.cpp
                libaudio_effect/audio_effect_filter_impl/vocal_volume_detector/volume_monitor.cpp
                libaudio_effect/audio_effect_filter_impl/vocal_volume_detector/vocal_volume_detect_effect_filter.cpp

                libresampler/Resampler.cpp

                libaudio_effect/audio_effect_processor/recording/audio_effect_recording_processor.cpp
                libaudio_effect/audio_effect_processor/post/audio_effect_post_processor.cpp
                libaudio_effect/audio_effect_processor/live/audio_effect_live_processor.cpp
                libaudio_effect/audio_effect_processor/live/accompany_effect_processor.cpp
                libaudio_effect/audio_effect_processor/audio_effect_processor_factory.cpp
                libaudio_effect/audio_effect_processor/audio_request.cpp
                libaudio_effect/audio_effect_processor/audio_response.cpp
                libaudio_effect/audio_effect_processor/audio_effect_context.cpp


                 )

target_link_libraries(libaudioeffect
            sox)