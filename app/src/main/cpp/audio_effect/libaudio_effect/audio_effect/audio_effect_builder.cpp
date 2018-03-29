#include "./audio_effect_builder.h"

#define LOG_TAG "AudioEffectBuilder"

AudioEffectBuilder::AudioEffectBuilder() {
}

AudioEffectBuilder::~AudioEffectBuilder() {
}

AudioEffect* AudioEffectBuilder::buildAudioEffect(jobject audioEffect, JNIEnv* env) {
	jclass cls_AudioEffect = env->GetObjectClass(audioEffect);
	//method in class AudioEffect
	jmethodID audioEffect_getAccompanyVolume = env->GetMethodID(cls_AudioEffect, "getAccompanyVolume", "()F");
	jmethodID audioEffect_getAudioVolume = env->GetMethodID(cls_AudioEffect, "getAudioVolume", "()F");
	jmethodID audioEffect_getReverbParam = env->GetMethodID(cls_AudioEffect, "getReverbParam", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/SOXReverbParam;");
	jmethodID audioEffect_getEqualizerParam = env->GetMethodID(cls_AudioEffect, "getEqualizerParam", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/SOXEqualizerParam;");
	jmethodID audioEffect_getCompressorParam = env->GetMethodID(cls_AudioEffect, "getCompressorParam", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/SOXCompressorParam;");
	jmethodID audioEffect_getVocalEffectFilterChain = env->GetMethodID(cls_AudioEffect, "getVocalEffectFilterChain", "()Ljava/util/ArrayList;");
	jmethodID audioEffect_getAccompanyEffectFilterChain = env->GetMethodID(cls_AudioEffect, "getAccompanyEffectFilterChain", "()Ljava/util/ArrayList;");
	jmethodID audioEffect_getMixPostEffectFilterChain = env->GetMethodID(cls_AudioEffect, "getMixPostEffectFilterChain", "()Ljava/util/ArrayList;");
	jmethodID audioEffect_getAudioInfo = env->GetMethodID(cls_AudioEffect, "getAudioInfo", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/AudioInfo;");
	jmethodID audioEffect_getOutputGainParam = env->GetMethodID(cls_AudioEffect, "getOutputGainParam", "()Lcom/timeapp/shawn/recorder/pro/audioeffect/OutputGainParam;");

	jmethodID audioEffect_getAccompanyDefault = env->GetMethodID(cls_AudioEffect, "getAccompanyDefault", "()F");
	jmethodID audioEffect_getAccompanyVolumeDefault = env->GetMethodID(cls_AudioEffect, "getAccompanyVolumeDefault", "()F");
	jmethodID audioEffect_getVocalVolumeDefault = env->GetMethodID(cls_AudioEffect, "getVocalVolumeDefault", "()F");
	jfloat accompany_default = env->CallFloatMethod(audioEffect, audioEffect_getAccompanyDefault);
	jfloat accompanyvolume_default = env->CallFloatMethod(audioEffect, audioEffect_getAccompanyVolumeDefault);
	jfloat vocalvolume_default = env->CallFloatMethod(audioEffect, audioEffect_getVocalVolumeDefault);
	AudioEffect::AccompanyDefault = accompany_default;
	AudioEffect::AccompanyVolumeDefault = accompanyvolume_default;
	AudioEffect::VocalVolumeDefault = vocalvolume_default;

	/** 基本属性 **/
	jfloat accompanyVolume = env->CallFloatMethod(audioEffect, audioEffect_getAccompanyVolume);
	jfloat audioVolume = env->CallFloatMethod(audioEffect, audioEffect_getAudioVolume);

	SOXFilterChainParam* filterChainParam = SOXFilterChainParam::buildDefaultParam();
	/** Reverb属性 **/
	jobject reverbParam = env->CallObjectMethod(audioEffect, audioEffect_getReverbParam);
	setReverbFilterParam(reverbParam, env, filterChainParam);
	/** EQ属性 **/
	jobject equalizerParam = env->CallObjectMethod(audioEffect, audioEffect_getEqualizerParam);
	setEqualizerFilterParam(equalizerParam, env, filterChainParam);
	/** Compressor属性 **/
	jobject compressorParam = env->CallObjectMethod(audioEffect, audioEffect_getCompressorParam);
	setCompressorFilterParam(compressorParam, env, filterChainParam);
	/** AudioInfo属性 **/
	jobject audioInfoInJNI = env->CallObjectMethod(audioEffect, audioEffect_getAudioInfo);
	AudioInfo* audioInfo = getAudioInfo(audioInfoInJNI, env);
	/** Output Gain 属性	 **/
	float outputGain = 1.0f;
	jobject outputGainParamObj = env->CallObjectMethod(audioEffect, audioEffect_getOutputGainParam);
	if (outputGainParamObj != NULL) {
		jclass cls_OutputGainParam = env->GetObjectClass(outputGainParamObj);
		jmethodID outputGainParam_getGain = env->GetMethodID(cls_OutputGainParam, "getGain", "()F");
		outputGain = env->CallFloatMethod(outputGainParamObj, outputGainParam_getGain);
	}

	jobject vocalEffectFilterChain = env->CallObjectMethod(audioEffect, audioEffect_getVocalEffectFilterChain);
	jclass cls_arraylist = env->GetObjectClass(vocalEffectFilterChain);
	jmethodID arraylist_get = env->GetMethodID(cls_arraylist, "get", "(I)Ljava/lang/Object;");
	jmethodID arraylist_size = env->GetMethodID(cls_arraylist, "size", "()I");

	//从配置文件中获取人声和伴奏自己的FilterChain的参数
	std::list<int>* vocalEffectFilters = new std::list<int>();
	jint len = env->CallIntMethod(vocalEffectFilterChain, arraylist_size);
	LOGI("************* vocalEffectFilters len is %d ***********", len);
	for (int i = 0; i < len; i++) {
		jobject obj_vocalEffectFilterType = env->CallObjectMethod(vocalEffectFilterChain, arraylist_get, i);
		jclass cls_Integer = env->GetObjectClass(obj_vocalEffectFilterType);
		jmethodID int_value = env->GetMethodID(cls_Integer, "intValue", "()I");
		jint vocalEffectFilterType = env->CallIntMethod(obj_vocalEffectFilterType, int_value);
		LOGI("[%d]", vocalEffectFilterType);
		vocalEffectFilters->push_back(vocalEffectFilterType);
	}
	std::list<int>* accompanyEffectFilters = new std::list<int>();
	jobject accompanyEffectFilterChain = env->CallObjectMethod(audioEffect, audioEffect_getAccompanyEffectFilterChain);
	len = env->CallIntMethod(accompanyEffectFilterChain, arraylist_size);
	LOGI("************* accompanyEffectFilters len is %d ***********", len);
	for (int i = 0; i < len; i++) {
		jobject obj_vocalEffectFilterType = env->CallObjectMethod(accompanyEffectFilterChain, arraylist_get, i);
		jclass cls_Integer = env->GetObjectClass(obj_vocalEffectFilterType);
		jmethodID int_value = env->GetMethodID(cls_Integer, "intValue", "()I");
		jint vocalEffectFilterType = env->CallIntMethod(obj_vocalEffectFilterType, int_value);
		LOGI("[%d]", vocalEffectFilterType);
		accompanyEffectFilters->push_back(vocalEffectFilterType);
	}
	std::list<int>* mixPostEffectFilters = new std::list<int>();
	jobject mixPostEffectFilterChain = env->CallObjectMethod(audioEffect, audioEffect_getMixPostEffectFilterChain);
	len = env->CallIntMethod(mixPostEffectFilterChain, arraylist_size);
	LOGI("************* mixPostEffectFilters len is %d ***********", len);
	for (int i = 0; i < len; i++) {
		jobject obj_vocalEffectFilterType = env->CallObjectMethod(mixPostEffectFilterChain, arraylist_get, i);
		jclass cls_Integer = env->GetObjectClass(obj_vocalEffectFilterType);
		jmethodID int_value = env->GetMethodID(cls_Integer, "intValue", "()I");
		jint vocalEffectFilterType = env->CallIntMethod(obj_vocalEffectFilterType, int_value);
		LOGI("[%d]", vocalEffectFilterType);
		mixPostEffectFilters->push_back(vocalEffectFilterType);
	}
	return this->generateAudioEffect(audioEffect, env, audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume,
			filterChainParam, outputGain);
}

AudioInfo* AudioEffectBuilder::getAudioInfo(jobject audioInfo, JNIEnv* env) {
	jclass cls_AudioInfo = env->GetObjectClass(audioInfo);
	jmethodID audioInfo_getAccompanyAGCVolume = env->GetMethodID(cls_AudioInfo, "getAccompanyAGCVolume", "()F");
	jmethodID audioInfo_getAudioAGCVolume = env->GetMethodID(cls_AudioInfo, "getAudioAGCVolume", "()F");
	jmethodID audioInfo_getAccompanyPitch = env->GetMethodID(cls_AudioInfo, "getAccompanyPitch", "()F");
	jmethodID audioInfo_getTotalTimeMills = env->GetMethodID(cls_AudioInfo, "getTotalTimeMills", "()I");
	jmethodID audioInfo_getRecordedTimeMills = env->GetMethodID(cls_AudioInfo, "getRecordedTimeMills", "()I");
	jmethodID audioInfo_getAudioSampleRate = env->GetMethodID(cls_AudioInfo, "getAudioSampleRate", "()I");
	jmethodID audioInfo_getChannels = env->GetMethodID(cls_AudioInfo, "getChannels", "()I");
	jmethodID audioInfo_getPitchShiftLevel = env->GetMethodID(cls_AudioInfo, "getPitchShiftLevel", "()I");

	jfloat accompanyAGCVolume = env->CallFloatMethod(audioInfo, audioInfo_getAccompanyAGCVolume);
	jfloat audioAGCVolume = env->CallFloatMethod(audioInfo, audioInfo_getAudioAGCVolume);
	jfloat accompanyPitch = env->CallFloatMethod(audioInfo, audioInfo_getAccompanyPitch);
	jint totalTimeMills = env->CallIntMethod(audioInfo, audioInfo_getTotalTimeMills);
	jint recordedTimeMills = env->CallIntMethod(audioInfo, audioInfo_getRecordedTimeMills);
	jint audioSampleRate = env->CallIntMethod(audioInfo, audioInfo_getAudioSampleRate);
	jint channels = env->CallIntMethod(audioInfo, audioInfo_getChannels);
	jint pitchShiftLevel = env->CallIntMethod(audioInfo, audioInfo_getPitchShiftLevel);
	return new AudioInfo(channels, audioSampleRate, recordedTimeMills, totalTimeMills, accompanyAGCVolume, audioAGCVolume, accompanyPitch, pitchShiftLevel);
}

AudioEffect* AudioEffectBuilder::generateAudioEffect(jobject audioEffect, JNIEnv* env, AudioInfo* audioInfo, std::list<int>* vocalEffectFilters,
		std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolume, float audioVolume,
		SOXFilterChainParam* filterChainParam, float outputGainParam) {
	return new AudioEffect(audioInfo, vocalEffectFilters, accompanyEffectFilters, mixPostEffectFilters, accompanyVolume, audioVolume, filterChainParam, outputGainParam);
}

void AudioEffectBuilder::setCompressorFilterParam(jobject compressorParam, JNIEnv* env, SOXFilterChainParam* filterChainParam) {
	jclass cls_CompressorParam = env->GetObjectClass(compressorParam);
	jmethodID compressorParam_getAttacktime = env->GetMethodID(cls_CompressorParam, "getAttacktime", "()F");
	jmethodID compressorParam_getDecaytime = env->GetMethodID(cls_CompressorParam, "getDecaytime", "()F");
	jmethodID compressorParam_getThreshold = env->GetMethodID(cls_CompressorParam, "getThreshold", "()I");
	jmethodID compressorParam_getRatio = env->GetMethodID(cls_CompressorParam, "getRatio", "()F");
	jmethodID compressorParam_getExpansion_threshold = env->GetMethodID(cls_CompressorParam, "getExpansion_threshold", "()I");
	jmethodID compressorParam_getExpansion_ratio = env->GetMethodID(cls_CompressorParam, "getExpansion_ratio", "()F");
	jmethodID compressorParam_getGain = env->GetMethodID(cls_CompressorParam, "getGain", "()I");
	jfloat attackTime = env->CallFloatMethod(compressorParam, compressorParam_getAttacktime);
	jfloat decayTime = env->CallFloatMethod(compressorParam, compressorParam_getDecaytime);
	jint threshold = env->CallIntMethod(compressorParam, compressorParam_getThreshold);
	jfloat ratio = env->CallFloatMethod(compressorParam, compressorParam_getRatio);
	jint expansion_threshold = env->CallIntMethod(compressorParam, compressorParam_getExpansion_threshold);
	jfloat expansion_ratio = env->CallFloatMethod(compressorParam, compressorParam_getExpansion_ratio);
	jint gain = env->CallIntMethod(compressorParam, compressorParam_getGain);
	filterChainParam->setCompressorFilterParam(attackTime, decayTime, threshold, ratio, expansion_threshold, expansion_ratio, gain);
}

void AudioEffectBuilder::setReverbFilterParam(jobject reverbParam, JNIEnv* env, SOXFilterChainParam* filterChainParam) {
	jclass cls_ReverbParam = env->GetObjectClass(reverbParam);
	jmethodID reverbParam_getReverberance = env->GetMethodID(cls_ReverbParam, "getReverberance", "()I");
	jmethodID reverbParam_getDamping = env->GetMethodID(cls_ReverbParam, "getDamping", "()I");
	jmethodID reverbParam_getRoomScale = env->GetMethodID(cls_ReverbParam, "getRoomScale", "()I");
	jmethodID reverbParam_getPreDelay = env->GetMethodID(cls_ReverbParam, "getPreDelay", "()I");
	jmethodID reverbParam_getWetGain = env->GetMethodID(cls_ReverbParam, "getWetGain", "()I");
	jmethodID reverbParam_getStereoDepth = env->GetMethodID(cls_ReverbParam, "getStereoDepth", "()I");
	jmethodID reverbParam_getDryWetMix = env->GetMethodID(cls_ReverbParam, "getDryWetMix", "()I");
	jmethodID reverbParam_getEchoDelay = env->GetMethodID(cls_ReverbParam, "getEchoDelay", "()I");
	jmethodID reverbParam_getEchoPredecay = env->GetMethodID(cls_ReverbParam, "getEchoPredecay", "()I");
	jmethodID reverbParam_getEchoDecayRatio = env->GetMethodID(cls_ReverbParam, "getEchoDecayRatio", "()I");
	jmethodID reverbParam_getEchoPhaseDiff = env->GetMethodID(cls_ReverbParam, "getEchoPhaseDiff", "()I");
	jmethodID reverbParam_getEchoPeriod = env->GetMethodID(cls_ReverbParam, "getEchoPeriod", "()I");
	jmethodID reverbParam_getEchoNum = env->GetMethodID(cls_ReverbParam, "getEchoNum", "()I");
	jint reverberance = env->CallIntMethod(reverbParam, reverbParam_getReverberance);
	jint damping = env->CallIntMethod(reverbParam, reverbParam_getDamping);
	jint roomScale = env->CallIntMethod(reverbParam, reverbParam_getRoomScale);
	jint preDelay = env->CallIntMethod(reverbParam, reverbParam_getPreDelay);
	jint wetGain = env->CallIntMethod(reverbParam, reverbParam_getWetGain);
	jint stereoDepth = env->CallIntMethod(reverbParam, reverbParam_getStereoDepth);
	jint dryWetMix = env->CallIntMethod(reverbParam, reverbParam_getDryWetMix);
	jint echoDelay = env->CallIntMethod(reverbParam, reverbParam_getEchoDelay);
	jint echoPredecay = env->CallIntMethod(reverbParam, reverbParam_getEchoPredecay);
	jint echoDecayRatio = env->CallIntMethod(reverbParam, reverbParam_getEchoDecayRatio);
	jint echoPhaseDiff = env->CallIntMethod(reverbParam, reverbParam_getEchoPhaseDiff);
	jint echoPeriod = env->CallIntMethod(reverbParam, reverbParam_getEchoPeriod);
	jint echoNum = env->CallIntMethod(reverbParam, reverbParam_getEchoNum);
	filterChainParam->setReverbRatio((float) ((dryWetMix)) / 100);
	filterChainParam->setReverbFilterParam(reverberance, damping, roomScale, preDelay, wetGain, stereoDepth, echoDelay, echoPredecay, echoPeriod, echoDecayRatio, echoPhaseDiff,
			echoNum);
}

void AudioEffectBuilder::setEqualizerFilterParam(jobject equalizerParam, JNIEnv* env, SOXFilterChainParam* filterChainParam) {
	jclass cls_EqualizerParam = env->GetObjectClass(equalizerParam);
	jmethodID equalizerParam_isEnable1 = env->GetMethodID(cls_EqualizerParam, "isEnable1", "()Z");
	jmethodID equalizerParam_getFrequency1 = env->GetMethodID(cls_EqualizerParam, "getFrequency1", "()I");
	jmethodID equalizerParam_getBandWidth1 = env->GetMethodID(cls_EqualizerParam, "getBandWidth1", "()F");
	jmethodID equalizerParam_getGain1 = env->GetMethodID(cls_EqualizerParam, "getGain1", "()F");
	jmethodID equalizerParam_isEnable2 = env->GetMethodID(cls_EqualizerParam, "isEnable2", "()Z");
	jmethodID equalizerParam_getFrequency2 = env->GetMethodID(cls_EqualizerParam, "getFrequency2", "()I");
	jmethodID equalizerParam_getBandWidth2 = env->GetMethodID(cls_EqualizerParam, "getBandWidth2", "()F");
	jmethodID equalizerParam_getGain2 = env->GetMethodID(cls_EqualizerParam, "getGain2", "()F");
	jmethodID equalizerParam_isEnable3 = env->GetMethodID(cls_EqualizerParam, "isEnable3", "()Z");
	jmethodID equalizerParam_getFrequency3 = env->GetMethodID(cls_EqualizerParam, "getFrequency3", "()I");
	jmethodID equalizerParam_getBandWidth3 = env->GetMethodID(cls_EqualizerParam, "getBandWidth3", "()F");
	jmethodID equalizerParam_getGain3 = env->GetMethodID(cls_EqualizerParam, "getGain3", "()F");
	jmethodID equalizerParam_isEnable4 = env->GetMethodID(cls_EqualizerParam, "isEnable4", "()Z");
	jmethodID equalizerParam_getFrequency4 = env->GetMethodID(cls_EqualizerParam, "getFrequency4", "()I");
	jmethodID equalizerParam_getBandWidth4 = env->GetMethodID(cls_EqualizerParam, "getBandWidth4", "()F");
	jmethodID equalizerParam_getGain4 = env->GetMethodID(cls_EqualizerParam, "getGain4", "()F");
	jmethodID equalizerParam_isLowpassEnabled = env->GetMethodID(cls_EqualizerParam, "isLowpassEnabled", "()Z");
	jmethodID equalizerParam_getLowpassFrequency = env->GetMethodID(cls_EqualizerParam, "getLowpassFrequency", "()I");
	jmethodID equalizerParam_getLowpassBandWidth = env->GetMethodID(cls_EqualizerParam, "getLowpassBandWidth", "()F");
	jmethodID equalizerParam_isHighpassEnabled = env->GetMethodID(cls_EqualizerParam, "isHighpassEnabled", "()Z");
	jmethodID equalizerParam_getHighpassFrequency = env->GetMethodID(cls_EqualizerParam, "getHighpassFrequency", "()I");
	jmethodID equalizerParam_getHighpassBandWidth = env->GetMethodID(cls_EqualizerParam, "getHighpassBandWidth", "()F");
	jboolean enable1 = env->CallBooleanMethod(equalizerParam, equalizerParam_isEnable1);
	jint frequency1 = env->CallIntMethod(equalizerParam, equalizerParam_getFrequency1);
	jfloat bandWidth1 = env->CallFloatMethod(equalizerParam, equalizerParam_getBandWidth1);
	jfloat gain1 = env->CallFloatMethod(equalizerParam, equalizerParam_getGain1);
	jboolean enable2 = env->CallBooleanMethod(equalizerParam, equalizerParam_isEnable2);
	jint frequency2 = env->CallIntMethod(equalizerParam, equalizerParam_getFrequency2);
	jfloat bandWidth2 = env->CallFloatMethod(equalizerParam, equalizerParam_getBandWidth2);
	jfloat gain2 = env->CallFloatMethod(equalizerParam, equalizerParam_getGain2);
	jboolean enable3 = env->CallBooleanMethod(equalizerParam, equalizerParam_isEnable3);
	jint frequency3 = env->CallIntMethod(equalizerParam, equalizerParam_getFrequency3);
	jfloat bandWidth3 = env->CallFloatMethod(equalizerParam, equalizerParam_getBandWidth3);
	jfloat gain3 = env->CallFloatMethod(equalizerParam, equalizerParam_getGain3);
	jboolean enable4 = env->CallBooleanMethod(equalizerParam, equalizerParam_isEnable4);
	jint frequency4 = env->CallIntMethod(equalizerParam, equalizerParam_getFrequency4);
	jfloat bandWidth4 = env->CallFloatMethod(equalizerParam, equalizerParam_getBandWidth4);
	jfloat gain4 = env->CallFloatMethod(equalizerParam, equalizerParam_getGain4);
	jboolean lowpassEnabled = env->CallBooleanMethod(equalizerParam, equalizerParam_isLowpassEnabled);
	jint lowpassFrequency = env->CallIntMethod(equalizerParam, equalizerParam_getLowpassFrequency);
	jfloat lowpassBandWidth = env->CallFloatMethod(equalizerParam, equalizerParam_getLowpassBandWidth);
	jboolean highpassEnabled = env->CallBooleanMethod(equalizerParam, equalizerParam_isHighpassEnabled);
	jint highpassFrequency = env->CallIntMethod(equalizerParam, equalizerParam_getHighpassFrequency);
	jfloat highpassBandWidth = env->CallFloatMethod(equalizerParam, equalizerParam_getHighpassBandWidth);
	filterChainParam->setEqualizerFilterParam(enable1, frequency1, bandWidth1, gain1, enable2, frequency2, bandWidth2, gain2, enable3, frequency3, bandWidth3, gain3, enable4,
			frequency4, bandWidth4, gain4, lowpassEnabled, lowpassFrequency, lowpassBandWidth, highpassEnabled, highpassFrequency, highpassBandWidth);
}
