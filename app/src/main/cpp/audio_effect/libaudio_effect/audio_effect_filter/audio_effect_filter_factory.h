#ifndef BASE_AUDIOEFFECT_FILTER_FACTORY_H_
#define BASE_AUDIOEFFECT_FILTER_FACTORY_H_

#include "../audio_effect_filter_impl/delay_output/aacompany_delay_output_effect_filter.h"
#include "../audio_effect_filter_impl/double_u/double_u_effect_filter.h"
#include "../audio_effect_filter_impl/fade/fade_out_effect_filter.h"
#include "../audio_effect_filter_impl/pitch_shift/accompany_pitch_shift_effect_filter.h"
#include "../audio_effect_filter_impl/pitch_shift/vocal_pitch_shift_effect_filter.h"
#include "../audio_effect_filter_impl/sox_effect/compressor_effect_filter.h"
#include "../audio_effect_filter_impl/sox_effect/equalizer_effect_filter.h"
#include "../audio_effect_filter_impl/sox_effect/reverb_effect_filter.h"
#include "../audio_effect_filter_impl/sox_effect/stereo_reverb_effect_filter.h"
#include "../audio_effect_filter_impl/sox_effect/adjust_echo_reverb_effect_filter.h"
#include "../audio_effect_filter_impl/specific/mono_2_stereo_effect_filter.h"
#include "../audio_effect_filter_impl/volume_adjust/accompany_agc_volume_adjust_effect_filter.h"
#include "../audio_effect_filter_impl/volume_adjust/accompany_volume_adjust_effect_filter.h"
#include "../audio_effect_filter_impl/volume_adjust/vocal_agc_volume_adjust_effect_filter.h"
#include "../audio_effect_filter_impl/volume_adjust/vocal_volume_adjust_effect_filter.h"
#include "../audio_effect_filter_impl/volume_adjust/vocal_volume_balence_adjust_effect_filter.h"
#include "../audio_effect_filter_impl/volume_adjust/accompany_volume_balence_adjust_effect_filter.h"
#include "../audio_effect_filter_impl/agc/accompany_agc_stat_effect_filter.h"
#include "../audio_effect_filter_impl/agc/vocal_agc_stat_effect_filter.h"
#include "../audio_effect_filter_impl/vocal_detector/vocal_detect_effect_filter.h"
#include "../audio_effect_filter_impl/vocal_volume_detector/vocal_volume_detect_effect_filter.h"

/** 这里面是所有Filter的类型
 * 这里面一定要注意:Filter不能删除，并且添加的时候一定要从后边开始添加，否则跟配置文件里面就对应不上了
 * **/
typedef enum EffectFilterType_t{
	VocalAGCVolumeAdjustEffectFilterType,//人声自动音量控制的音量调节
	AccompanyAGCVolumeAdjustEffectFilterType,//伴奏自动音量控制的音量调节
    CompressorFilterType,//压缩效果器
    EqualizerFilterType,//均衡效果器
	ReverbEchoFilterType,//混响+Echo效果器
	Mono2StereoFilterType,//单声道转双声道效果器
	AutoTuneEffectFilterType,//电音效果器
	DoubleUEffectFilterType,//迷幻效果器
	HarmonicEffectFilterType,//唱诗班效果器
	VocalPitchShiftFilterType,//人声升降调效果器
	AccompanyPitchShiftFilterType,//伴奏升降调效果器 10
	VocalVolumeAdjustFilterType,//人声音量增益效果器 11
	AccompanyVolumeAdjustFilterType,//伴奏音量增益效果器
	FadeOutEffectFilterType,//淡出效果器
	VocalAGCStatEffectFilterType,//人声自动音量控制的统计Filter
	AccompanyAGCStatEffectFilterType,//伴奏自动音量控制的统计Filter
	VocalDetectEffectFilterType,//人声段落检测的统计Filter
	AccompanyDelayOutputEffectFilterType,//为了电音做的伴奏的延迟输出效果器
	VocalVolumeBalanceAdjustFilterType,//7.0新增人声音量增益效果器 18
	AccompanyVolumeBalanceAdjustFilterType,//7.0新增伴奏音量增益效果器 19
	StereoReverbEchoFilterType,//支持双声道输入的混响+Echo效果器
	AdjustEchoReverbFilterType, //7.0增支持可调节的echo+reverb
	VocalVolumeDetectEffectFilterType//7.0.1新增只用来检测人声音量Filter，目前用在包房直播人声音量检测

} EffectFilterType;

class AudioEffectFilterFactory {
private:
	AudioEffectFilterFactory();
	static AudioEffectFilterFactory* instance;

public:
	static AudioEffectFilterFactory* GetInstance();
	~AudioEffectFilterFactory();
	AudioEffectFilter* buildFilter(EffectFilterType type);
};

#endif /* BASE_AUDIOEFFECT_FILTER_FACTORY_H_ */
