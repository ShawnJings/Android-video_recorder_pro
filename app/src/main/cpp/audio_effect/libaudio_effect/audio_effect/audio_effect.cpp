#include "./audio_effect.h"

#define LOG_TAG "AudioEffect"

float AudioEffect::AccompanyDefault = 0.0f;
float AudioEffect::AccompanyVolumeDefault = 0.0f;
float AudioEffect::VocalVolumeDefault = 0.0f;

AudioEffect::AudioEffect() {
}

AudioEffect::AudioEffect(AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolumeParam,
		float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam) {
	this->filterChainParam = filterChainParam;
	this->audioInfo = audioInfo;
	this->accompanyVolume = accompanyVolumeParam;
	this->audioVolume = audioVolumeParam;
	this->vocalEffectFilters = vocalEffectFilters;
	this->accompanyEffectFilters = accompanyEffectFilters;
	this->mixPostEffectFilters = mixPostEffectFilters;
	this->outputGain = outputGainParam;
}

AudioEffect::~AudioEffect() {
}
