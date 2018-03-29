#ifndef AUDIOEFFECT_BUILDER_H_
#define AUDIOEFFECT_BUILDER_H_

#include "./audio_effect.h"
#include "./audio_info.h"
#include "CommonTools.h"
#include <jni.h>

class AudioEffectBuilder {
protected:
	void setCompressorFilterParam(jobject compressorParam, JNIEnv* env, SOXFilterChainParam* filterChainParam);
	void setReverbFilterParam(jobject reverbParam, JNIEnv* env, SOXFilterChainParam* filterChainParam);
	void setEqualizerFilterParam(jobject equalizerParam, JNIEnv* env, SOXFilterChainParam* filterChainParam);
	virtual AudioEffect* generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam);

	AudioInfo* getAudioInfo(jobject audioInfo, JNIEnv* env);
public:
	AudioEffectBuilder();
	~AudioEffectBuilder();

	AudioEffect* buildAudioEffect(jobject audioEffect, JNIEnv* env);
};

#endif /* AUDIOEFFECT_BUILDER_H_ */
