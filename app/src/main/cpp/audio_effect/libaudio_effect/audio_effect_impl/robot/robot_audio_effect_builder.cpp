#include "./robot_audio_effect_builder.h"

#define LOG_TAG "RobotAudioEffectBuilder"

RobotAudioEffectBuilder::RobotAudioEffectBuilder() {
}

RobotAudioEffectBuilder::~RobotAudioEffectBuilder() {
}

AudioEffect* RobotAudioEffectBuilder::generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam) {
	jclass cls_AudioEffect = env->GetObjectClass(audioEffect);
	jmethodID audioEffect_getVocalPitch = env->GetMethodID(cls_AudioEffect, "getVocalPitch", "()F");
	jfloat vocalPitch = env->CallFloatMethod(audioEffect, audioEffect_getVocalPitch);
	AudioEffect* ret = new RobotAudioEffect(vocalPitch, audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGainParam);
	return ret;
}
