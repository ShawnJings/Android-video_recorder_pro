#include "./auto_tune_audio_effect.h"

#define LOG_TAG "AutoTuneAudioEffect"

AutoTuneAudioEffect::AutoTuneAudioEffect() {
}

AutoTuneAudioEffect::AutoTuneAudioEffect(char* melPathParam, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam,
		float outputGainParam) :
		AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolumeParam, audioVolumeParam, filterChainParam, outputGainParam) {
	this->melPath = melPathParam;
}

AutoTuneAudioEffect::~AutoTuneAudioEffect() {
}
