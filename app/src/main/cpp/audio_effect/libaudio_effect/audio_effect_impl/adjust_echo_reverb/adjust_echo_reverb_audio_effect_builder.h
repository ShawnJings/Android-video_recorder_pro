#ifndef ADJUST_ECHO_REVERB_AUDIOEFFECT_BUILDER_H_
#define ADJUST_ECHO_REVERB_AUDIOEFFECT_BUILDER_H_

#include "../../audio_effect/audio_effect_builder.h"
#include "adjust_echo_reverb_audio_effect.h"

class AdjustEchoReverbAudioEffectBuilder: public AudioEffectBuilder {
protected:
	AudioEffect* generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam);
public:
	AdjustEchoReverbAudioEffectBuilder();
	~AdjustEchoReverbAudioEffectBuilder();
};

#endif /* ADJUST_ECHO_REVERB_AUDIOEFFECT_BUILDER_H_ */
