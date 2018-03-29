#ifndef AUTO_TUNE_AUDIOEFFECT_H_
#define AUTO_TUNE_AUDIOEFFECT_H_

#include "../../audio_effect/audio_effect.h"
#include <string>

class AutoTuneAudioEffect: public AudioEffect {
private:
	std::string melPath;

public:
	AutoTuneAudioEffect();
	AutoTuneAudioEffect(char* melPathParam, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
			std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam,
			float outputGainParam);
	~AutoTuneAudioEffect();
	std::string getMelPath() {
		return melPath;
	}
};

#endif /* AUTO_TUNE_AUDIOEFFECT_H_ */
