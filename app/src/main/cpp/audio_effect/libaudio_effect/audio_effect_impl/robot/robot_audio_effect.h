#ifndef ROBOT_AUDIOEFFECT_H_
#define ROBOT_AUDIOEFFECT_H_

#include "../../audio_effect/audio_effect.h"

class RobotAudioEffect: public AudioEffect {
private:
	/** 变声（升降调） **/
	float vocalPitch;

public:
	RobotAudioEffect();
	RobotAudioEffect(float vocalPitch, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
			std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam);
	~RobotAudioEffect();

	float getVocalPitch() {
		return vocalPitch;
	};
};

#endif /* ROBOT_AUDIOEFFECT_H_ */
