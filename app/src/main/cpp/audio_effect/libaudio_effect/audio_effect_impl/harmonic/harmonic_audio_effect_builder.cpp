#include "harmonic_audio_effect_builder.h"

#define LOG_TAG "HarmonicAudioEffectBuilder"

HarmonicAudioEffectBuilder::HarmonicAudioEffectBuilder() {
}

HarmonicAudioEffectBuilder::~HarmonicAudioEffectBuilder() {
}

AudioEffect* HarmonicAudioEffectBuilder::generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam) {
	jclass cls_AudioEffect = env->GetObjectClass(audioEffect);
	jmethodID audioEffect_getAudioInfo = env->GetMethodID(cls_AudioEffect, "getAudioInfo", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/AudioInfo;");
	jobject audioInfoInJNI = env->CallObjectMethod(audioEffect, audioEffect_getAudioInfo);
	jclass cls_AudioInfo = env->GetObjectClass(audioInfoInJNI);
	jmethodID audioInfo_getMelFilePath = env->GetMethodID(cls_AudioInfo, "getMelFilePath", "()Ljava/lang/String;");
	jmethodID audioInfo_isHarmonicOnlyChorus = env->GetMethodID(cls_AudioInfo, "isHarmonicOnlyChorus", "()Z");
	jmethodID audioInfo_isHarmonicFastMode = env->GetMethodID(cls_AudioInfo, "isHarmonicFastMode", "()Z");

	jstring jmelpath = (jstring) env->CallObjectMethod(audioInfoInJNI, audioInfo_getMelFilePath);
	char *melpath = "";
	if (jmelpath != NULL) {
		melpath = (char *) env->GetStringUTFChars(jmelpath, NULL);
	}
	bool isOnlyChorus = (bool)env->CallBooleanMethod(audioInfoInJNI, audioInfo_isHarmonicOnlyChorus);
	bool isFastMode = (bool)env->CallBooleanMethod(audioInfoInJNI, audioInfo_isHarmonicFastMode);
	LOGI("enter HarmonicAudioEffectBuilder::generateAudioEffect() melpath : %s, isOnlyChorus=%d, isFastMode=%d", melpath, isOnlyChorus, isFastMode);

	AudioEffect* ret = new HarmonicAudioEffect(melpath, audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGainParam,
			isOnlyChorus, isFastMode);
	if (jmelpath != NULL && melpath != NULL) {
		env->ReleaseStringUTFChars(jmelpath, melpath);
	}
	return ret;
}
