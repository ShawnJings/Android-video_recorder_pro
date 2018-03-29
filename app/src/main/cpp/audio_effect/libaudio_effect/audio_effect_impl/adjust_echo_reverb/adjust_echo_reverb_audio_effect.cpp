#include "adjust_echo_reverb_audio_effect.h"

#define LOG_TAG "AdjustEchoReverbAudioEffect"

AdjustEchoReverbAudioEffect::AdjustEchoReverbAudioEffect() {
}

AdjustEchoReverbAudioEffect::AdjustEchoReverbAudioEffect(AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam,
		float echoRatio, float reverbRatio) :
		AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolumeParam, audioVolumeParam, filterChainParam, outputGainParam) {
	this->echoRatioParam = echoRatio;
	this->reverbRatioParam = reverbRatio;
}

AdjustEchoReverbAudioEffect::~AdjustEchoReverbAudioEffect() {
}
