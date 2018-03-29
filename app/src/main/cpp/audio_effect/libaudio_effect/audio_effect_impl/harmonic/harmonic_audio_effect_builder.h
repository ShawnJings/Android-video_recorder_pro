#ifndef HARMONIC_AUDIOEFFECT_BUILDER_H_
#define HARMONIC_AUDIOEFFECT_BUILDER_H_

#include "../../audio_effect/audio_effect_builder.h"
#include "harmonic_audio_effect.h"

class HarmonicAudioEffectBuilder: public AudioEffectBuilder {
protected:
	AudioEffect* generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam);
public:
	HarmonicAudioEffectBuilder();
	~HarmonicAudioEffectBuilder();
};

#endif /* HARMONIC_AUDIOEFFECT_BUILDER_H_ */
