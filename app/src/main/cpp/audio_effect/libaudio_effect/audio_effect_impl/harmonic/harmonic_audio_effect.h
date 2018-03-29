#ifndef HARMONIC_AUDIOEFFECT_H_
#define HARMONIC_AUDIOEFFECT_H_

#include "../../audio_effect/audio_effect.h"
#include <string>

class HarmonicAudioEffect: public AudioEffect {
private:
	std::string melPath;
	bool isOnlyChorusParam;
	bool isFastMode;

public:
	HarmonicAudioEffect();
	HarmonicAudioEffect(char* melPathParam, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
			std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam,
			bool isOnlyChorus, bool isFastMode);
	~HarmonicAudioEffect();
	std::string getMelPath() {
		return melPath;
	}
	bool isOnlyChorus() {
		return isOnlyChorusParam;
	}
	bool isHarmonicFastMode() {
		return isFastMode;
	}
};

#endif /* HARMONIC_AUDIOEFFECT_H_ */
