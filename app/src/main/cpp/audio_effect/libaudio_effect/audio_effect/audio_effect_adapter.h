#ifndef AUDIOEFFECT_ADAPTER_H_
#define AUDIOEFFECT_ADAPTER_H_

#include "CommonTools.h"
#include <jni.h>
#include "../audio_effect_filter/audio_effect_filter_factory.h"
#include "./audio_effect.h"
#include "./audio_effect_builder.h"
#include "../audio_effect_impl/auto_tune/auto_tune_audio_effect_builder.h"
#include "../audio_effect_impl/robot/robot_audio_effect_builder.h"
#include "../audio_effect_impl/harmonic/harmonic_audio_effect_builder.h"
#include "../audio_effect_impl/adjust_echo_reverb/adjust_echo_reverb_audio_effect_builder.h"

/** 这里面是所有AudioEffectType的类型
 * 这里面一定要注意:Type不能删除，并且添加的时候一定要从后边开始添加，否则跟配置文件里面就对应不上了
 * **/
typedef enum AudioEffectType_t {
	BASE_AUDIO_EFFECT_TYPE, //原声|留声机|迷幻|流行|摇滚|舞曲|新世纪|R&B|LIVE_XXX
	AUTO_TUNE_EFFECT_TYPE, //电音
	ROBOT_EFFECT_TYPE, //机器人
	LENOVO_EFFECT_TYPE, //联想乐蒙特效
	HARMONIC, //和声音效
	ADJUST_ECHO_REVERB_EFFECT_TYPE, //超级混响
} AudioEffectType;

class AudioEffectAdapter {
private:
	AudioEffectAdapter();
	static AudioEffectAdapter* instance;

	AudioEffectBuilder* getAudioEffectBuilder(int type) {
		AudioEffectBuilder* ret = NULL;
		switch (type) {
		case AUTO_TUNE_EFFECT_TYPE:
			ret = new AutoTuneAudioEffectBuilder();
			break;
		case ROBOT_EFFECT_TYPE:
			ret = new RobotAudioEffectBuilder();
			break;
		case HARMONIC:
			ret = new HarmonicAudioEffectBuilder();
			break;
		case ADJUST_ECHO_REVERB_EFFECT_TYPE:
			ret = new AdjustEchoReverbAudioEffectBuilder();
			break;
		case BASE_AUDIO_EFFECT_TYPE:
		case LENOVO_EFFECT_TYPE:
		default:
			ret = new AudioEffectBuilder();
			break;
		}
		return ret;
	}

public:
	static AudioEffectAdapter* GetInstance();

	~AudioEffectAdapter();

	AudioEffect* buildMVAudioEffect(jobject audioEffect, JNIEnv* env);
	AudioEffect* buildAudioEffect(jobject audioEffect, JNIEnv* env);
	AudioEffect* buildRecordingAudioEffect(int channels, int audioSampleRate, bool isUnAccom);
	AudioEffect* buildLiveAudioEffect(jobject audioEffect, JNIEnv* env);
	AudioEffect* buildVideoDuetRecordingAudioEffect(int channels, int audioSampleRate, bool isUnAccom);
	AudioEffect* buildDefaultAudioEffect(int channels, int audioSampleRate, bool isUnAccom);
};

#endif /* AUDIOEFFECT_ADAPTER_H_ */
