#include "./auto_tune_audio_effect_builder.h"

#define LOG_TAG "AutoTuneAudioEffectBuilder"

AutoTuneAudioEffectBuilder::AutoTuneAudioEffectBuilder() {
}

AutoTuneAudioEffectBuilder::~AutoTuneAudioEffectBuilder() {
}

AudioEffect* AutoTuneAudioEffectBuilder::generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters,
		std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume, SOXFilterChainParam* filterChainParam, float outputGainParam) {
	jclass cls_AudioEffect = env->GetObjectClass(audioEffect);
	jmethodID audioEffect_getAudioInfo = env->GetMethodID(cls_AudioEffect, "getAudioInfo", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/AudioInfo;");
	jobject audioInfoInJNI = env->CallObjectMethod(audioEffect, audioEffect_getAudioInfo);
	jclass cls_AudioInfo = env->GetObjectClass(audioInfoInJNI);
	jmethodID audioInfo_getMelFilePath = env->GetMethodID(cls_AudioInfo, "getMelFilePath", "()Ljava/lang/String;");

	jstring jmelpath = (jstring) env->CallObjectMethod(audioInfoInJNI, audioInfo_getMelFilePath);
	char *melpath = NULL;
	if (jmelpath != NULL) {
		melpath = (char *) env->GetStringUTFChars(jmelpath, NULL);
	}
	AudioEffect* ret = new AutoTuneAudioEffect(melpath, audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGainParam);
	if (jmelpath != NULL && melpath != NULL) {
		env->ReleaseStringUTFChars(jmelpath, melpath);
	}
	return ret;
}
