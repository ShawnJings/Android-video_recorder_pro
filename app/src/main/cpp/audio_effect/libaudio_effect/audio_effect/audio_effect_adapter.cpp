#include "./audio_effect_adapter.h"

#define LOG_TAG "AudioEffectAdapter"

AudioEffectAdapter::AudioEffectAdapter() {
}

AudioEffectAdapter::~AudioEffectAdapter() {
}

//初始化静态成员
AudioEffectAdapter* AudioEffectAdapter::instance = new AudioEffectAdapter();
AudioEffectAdapter* AudioEffectAdapter::GetInstance() {
	return instance;
}

AudioEffect* AudioEffectAdapter::buildDefaultAudioEffect(int channels, int audioSampleRate, bool isUnAccom) {
	float accompanyVolume = 1.0f;
	float audioVolume = 1.0f;
	SOXFilterChainParam* filterChainParam = SOXFilterChainParam::buildDefaultParam();
	std::list<int>* vocalEffectFilters = new std::list<int>();
	vocalEffectFilters->push_back(VocalAGCVolumeAdjustEffectFilterType);
	vocalEffectFilters->push_back(CompressorFilterType);
	vocalEffectFilters->push_back(EqualizerFilterType);
	vocalEffectFilters->push_back(ReverbEchoFilterType);
	vocalEffectFilters->push_back(VocalVolumeAdjustFilterType);
	std::list<int>* accompanyEffectFilters = new std::list<int>();
	if (!isUnAccom) {
		accompanyEffectFilters->push_back(AccompanyAGCVolumeAdjustEffectFilterType);
		accompanyEffectFilters->push_back(AccompanyVolumeAdjustFilterType);
	}
	std::list<int>* mixPostEffectFilters = new std::list<int>();
	accompanyEffectFilters->push_back(FadeOutEffectFilterType);
	int recordedTimeMills = 0;
	int totalTimeMills = 0;
	float accompanyAGCVolume = 1.0f;
	float audioAGCVolume = 1.0f;
	float accompanyPitch = 1.0f;
	float outputGain = 1.0f;
	int pitchShiftLevel = 0;
	AudioInfo* audioInfo = new AudioInfo(channels, audioSampleRate, recordedTimeMills, totalTimeMills, accompanyAGCVolume, audioAGCVolume, accompanyPitch, pitchShiftLevel);
	return new AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGain);
}

AudioEffect* AudioEffectAdapter::buildRecordingAudioEffect(int channels, int audioSampleRate, bool isUnAccom) {
	float accompanyVolume = 1.0f;
	float audioVolume = 1.0f;
	SOXFilterChainParam* filterChainParam = SOXFilterChainParam::buildRecordingParam();
	std::list<int>* vocalEffectFilters = new std::list<int>();
	//vocalEffectFilters->push_back(VocalAGCStatEffectFilterType);
	vocalEffectFilters->push_back(VocalDetectEffectFilterType);
	std::list<int>* accompanyEffectFilters = new std::list<int>();
	if (!isUnAccom) {
		//accompanyEffectFilters->push_back(AccompanyAGCStatEffectFilterType);
	}
	std::list<int>* mixPostEffectFilters = new std::list<int>();
	int recordedTimeMills = 0;
	int totalTimeMills = 0;
	float accompanyAGCVolume = 1.0f;
	float audioAGCVolume = 1.0f;
	float accompanyPitch = 1.0f;
	float outputGain = 1.0f;
	int pitchShiftLevel = 0;
	AudioInfo* audioInfo = new AudioInfo(channels, audioSampleRate, recordedTimeMills, totalTimeMills, accompanyAGCVolume, audioAGCVolume, accompanyPitch, pitchShiftLevel);
	return new AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGain);
}

AudioEffect* AudioEffectAdapter::buildVideoDuetRecordingAudioEffect(int channels, int audioSampleRate, bool isUnAccom) {
	AudioEffect* audioEffect = this->buildRecordingAudioEffect(channels, audioSampleRate, isUnAccom);
	//audioEffect->getVocalEffectFilters()->push_back(VocalDetectEffectFilterType);
	std::list<int>* effectFilters = audioEffect->getVocalEffectFilters();
	if (NULL != effectFilters && effectFilters->size() > 0) {
		std::list<int>::iterator ite = effectFilters->begin();
		for (; ite != effectFilters->end(); ++ite) {
			int effectFilterType = *ite;
			LOGI("buildVideoDuetRecordingAudioEffect effectFilterType is %d", effectFilterType);
		}
	}
	return audioEffect;
}

AudioEffect* AudioEffectAdapter::buildMVAudioEffect(jobject audioEffectInJNI, JNIEnv* env){
	AudioEffect* audioEffect = buildAudioEffect(audioEffectInJNI, env);
	LOGI("build audioEffect success");
	if(NULL != audioEffect){
		std::list<int>* mixPostEffectFilters = audioEffect->getMixPostEffectFilters();
		bool isFadeOutFilterExists = false;
		std::list<int>::iterator iterator = mixPostEffectFilters->begin();
		for (; iterator != mixPostEffectFilters->end(); iterator++) {
			if(*iterator == FadeOutEffectFilterType){
				isFadeOutFilterExists = true;
				break;
			}
		}
		LOGI("isFadeOutFilterExists is %d", isFadeOutFilterExists);
		if(isFadeOutFilterExists){
			mixPostEffectFilters->remove(FadeOutEffectFilterType);
			LOGI("mixPostEffectFilters->remove success");
		}
	}
	return audioEffect;
}

AudioEffect* AudioEffectAdapter::buildAudioEffect(jobject audioEffectInJNI, JNIEnv* env) {
	AudioEffect* audioEffect = NULL;
	jclass cls_AudioEffect = env->GetObjectClass(audioEffectInJNI);
	//method in class AudioEffect
	jmethodID audioEffect_getType = env->GetMethodID(cls_AudioEffect, "getType", "()I");
	jint type = env->CallIntMethod(audioEffectInJNI, audioEffect_getType);
	AudioEffectBuilder* audioEffectBuilder = getAudioEffectBuilder(type);
	if (NULL != audioEffectBuilder) {
		audioEffect = audioEffectBuilder->buildAudioEffect(audioEffectInJNI, env);
		audioEffect->setType(type);
		delete audioEffectBuilder;
	}
	return audioEffect;
}

AudioEffect* AudioEffectAdapter::buildLiveAudioEffect(jobject audioEffectInJNI, JNIEnv* env) {
	AudioEffect* audioEffect = buildAudioEffect(audioEffectInJNI, env);
	audioEffect->getVocalEffectFilters()->push_front(VocalVolumeDetectEffectFilterType);
	return audioEffect;
}
