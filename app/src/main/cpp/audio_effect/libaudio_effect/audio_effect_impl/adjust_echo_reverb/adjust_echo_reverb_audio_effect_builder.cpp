#include "adjust_echo_reverb_audio_effect_builder.h"

#define LOG_TAG "AdjustEchoReverbAudioEffectBuilder"

AdjustEchoReverbAudioEffectBuilder::AdjustEchoReverbAudioEffectBuilder() {
}

AdjustEchoReverbAudioEffectBuilder::~AdjustEchoReverbAudioEffectBuilder() {
}

AudioEffect* AdjustEchoReverbAudioEffectBuilder::generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam) {

	jclass cls_AudioEffect = env->GetObjectClass(audioEffect);

	jmethodID audioEffect_getAdjustEchoReverbParam = env->GetMethodID(cls_AudioEffect, "getAdjustEchoReverbParam", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/AdjustEchoReverbParam;");
	/** 超级混响参数 echo,reverb **/
	float adjustEcho = 1.0f;
	float adjustReverb = 1.0f;
	jobject adjustEchoReverbParamObj = env->CallObjectMethod(audioEffect, audioEffect_getAdjustEchoReverbParam);
	if (adjustEchoReverbParamObj != NULL) {
		jclass cls_AdjustEchoReverbParam = env->GetObjectClass(adjustEchoReverbParamObj);
		jmethodID adjustEchoReverbParam_getAdjustEchoEffectRatio = env->GetMethodID(cls_AdjustEchoReverbParam, "getAdjustEchoEffectRatio", "()F");
		jmethodID adjustEchoReverbParam_getAdjustReverbEffectRatio = env->GetMethodID(cls_AdjustEchoReverbParam, "getAdjustReverbEffectRatio", "()F");
		adjustEcho = env->CallFloatMethod(adjustEchoReverbParamObj, adjustEchoReverbParam_getAdjustEchoEffectRatio);
		adjustReverb = env->CallFloatMethod(adjustEchoReverbParamObj, adjustEchoReverbParam_getAdjustReverbEffectRatio);
	}
	AudioEffect* ret = new AdjustEchoReverbAudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGainParam,
			adjustEcho, adjustReverb);
	return ret;
}
