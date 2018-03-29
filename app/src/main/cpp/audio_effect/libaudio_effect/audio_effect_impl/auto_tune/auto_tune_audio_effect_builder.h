#ifndef AUTO_TUNE_AUDIOEFFECT_BUILDER_H_
#define AUTO_TUNE_AUDIOEFFECT_BUILDER_H_

#include "../../audio_effect/audio_effect_builder.h"
#include "./auto_tune_audio_effect.h"

class AutoTuneAudioEffectBuilder: public AudioEffectBuilder {
protected:
	AudioEffect* generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam);
public:
	AutoTuneAudioEffectBuilder();
	~AutoTuneAudioEffectBuilder();
};

#endif /* AUTO_TUNE_AUDIOEFFECT_BUILDER_H_ */
