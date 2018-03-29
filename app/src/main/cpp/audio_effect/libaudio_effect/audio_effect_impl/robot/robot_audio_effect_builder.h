#ifndef ROBOT_AUDIOEFFECT_BUILDER_H_
#define ROBOT_AUDIOEFFECT_BUILDER_H_

#include "../../audio_effect/audio_effect_builder.h"
#include "./robot_audio_effect.h"

class RobotAudioEffectBuilder: public AudioEffectBuilder {
protected:
	AudioEffect* generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam);
public:
	RobotAudioEffectBuilder();
	~RobotAudioEffectBuilder();
};

#endif /* ROBOT_AUDIOEFFECT_BUILDER_H_ */
