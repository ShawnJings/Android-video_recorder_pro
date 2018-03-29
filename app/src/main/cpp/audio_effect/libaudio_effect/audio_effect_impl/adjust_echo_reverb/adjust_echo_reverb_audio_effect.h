#ifndef ADJUST_ECHO_REVERB_AUDIOEFFECT_H_
#define ADJUST_ECHO_REVERB_AUDIOEFFECT_H_

#include "../../audio_effect/audio_effect.h"
#include <string>

class AdjustEchoReverbAudioEffect: public AudioEffect {
private:
	float echoRatioParam;
	float reverbRatioParam;

public:
	AdjustEchoReverbAudioEffect();
	AdjustEchoReverbAudioEffect(AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
			std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam,
			float echoRatio, float reverbRatio);
	~AdjustEchoReverbAudioEffect();

	float getEchoRatio() {
		return echoRatioParam;
	}

	float getReverbRatio() {
		return reverbRatioParam;
	}

};

#endif /* ADJUST_ECHO_REVERB_AUDIOEFFECT_H_ */
