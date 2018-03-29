#include "harmonic_audio_effect.h"

#define LOG_TAG "HarmonicAudioEffect"

HarmonicAudioEffect::HarmonicAudioEffect() {
}

HarmonicAudioEffect::HarmonicAudioEffect(char* melPathParam, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam,
		bool isOnlyChorus, bool fastModeParam) :
		AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolumeParam, audioVolumeParam, filterChainParam, outputGainParam) {
	this->melPath = melPathParam;
	this->isOnlyChorusParam = isOnlyChorus;
	this->isFastMode = fastModeParam;
}

HarmonicAudioEffect::~HarmonicAudioEffect() {
}
