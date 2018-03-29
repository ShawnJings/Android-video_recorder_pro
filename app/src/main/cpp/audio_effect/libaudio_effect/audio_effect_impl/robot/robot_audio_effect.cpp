#include "./robot_audio_effect.h"

#define LOG_TAG "RobotAudioEffect"

RobotAudioEffect::RobotAudioEffect() {
}

RobotAudioEffect::RobotAudioEffect(float vocalPitch, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam) :
		AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolumeParam, audioVolumeParam, filterChainParam, outputGainParam) {
	this->vocalPitch = vocalPitch;
}

RobotAudioEffect::~RobotAudioEffect() {
}
