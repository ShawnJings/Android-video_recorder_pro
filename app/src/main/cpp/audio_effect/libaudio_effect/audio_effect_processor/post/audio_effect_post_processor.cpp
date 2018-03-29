#include "./audio_effect_post_processor.h"

#define LOG_TAG "AudioEffectPostProcessor"

AudioEffectPostProcessor::AudioEffectPostProcessor() {
	request = new AudioRequest();
	response = new AudioResponse();
}

AudioEffectPostProcessor::~AudioEffectPostProcessor() {
}

void AudioEffectPostProcessor::init(AudioEffect *audioEffect) {
	AudioEffectProcessor::init(audioEffect);
	this->audioEffect = audioEffect;
	isAudioEffectChanged = false;
	vocalEffectFilterChain = new AudioEffectFilterChain();
	accompanyEffectFilterChain = new AudioEffectFilterChain();
	mixPostEffectFilterChain = new AudioEffectFilterChain();
	this->initFilterChains();
}


AudioResponse* AudioEffectPostProcessor::process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum){
	detectRebuildFilterChain();
	request->init(vocalBuffer, vocalBufferSize, accompanyBuffer, accompanyBufferSize, position, frameNum);
	response->clear();
	vocalEffectFilterChain->doFilter(request, response);
	accompanyEffectFilterChain->doFilter(request, response);
	mixEffectFilter->doFilter(request, response);
	mixPostEffectFilterChain->doFilter(request, response);
	return response;
}

AudioResponse* AudioEffectPostProcessor::process(short *vocalBuffer, int vocalBufferSize, float position, long frameNum) {
	detectRebuildFilterChain();
	request->init(vocalBuffer, vocalBufferSize, position, frameNum);
	response->clear();
	vocalEffectFilterChain->doFilter(request, response);
	mixPostEffectFilterChain->doFilter(request, response);
	return response;
}

AudioResponse* AudioEffectPostProcessor::processAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum) {
	response->clear();
	detectRebuildFilterChain();
	request->initAccompany(accompanyBuffer, accompanyBufferSize, position, frameNum);
	accompanyEffectFilterChain->doFilter(request, response);
	mixPostEffectFilterChain->doFilter(request, response);
	return response;
}

void AudioEffectPostProcessor::detectRebuildFilterChain(){
	if(isAudioEffectChanged){
		this->destroyFilterChains();
		this->initFilterChains();
		isAudioEffectChanged = false;
	}
}

void AudioEffectPostProcessor::resetFilterChains(){
	setAudioEffect(audioEffect);
}

void AudioEffectPostProcessor::initFilterChains(){
	vocalEffectFilterChain->init(audioEffect, audioEffect->getVocalEffectFilters());
	accompanyEffectFilterChain->init(audioEffect, audioEffect->getAccompanyEffectFilters());
	mixPostEffectFilterChain->init(audioEffect, audioEffect->getMixPostEffectFilters());
	if (isAccompanyPitchShift(audioEffect) || isRobotAudioEffectFilter(audioEffect)) {
		LOGI("AudioEffectPostProcessor::initFilterChains() new PitchShiftMixEffectFilter");
		mixEffectFilter = new PitchShiftMixEffectFilter();
	} else {
		mixEffectFilter = new MixEffectFilter();
		LOGI("AudioEffectPostProcessor::initFilterChains() new MixEffectFilter");
	}
	mixEffectFilter->init(audioEffect);
}

bool AudioEffectPostProcessor::isAccompanyPitchShift(AudioEffect *audioEffect) {
	if (audioEffect == NULL) {
		return false;
	}
	AudioInfo *audioInfo = audioEffect->getAudioInfo();
	if (audioInfo == NULL) {
		return false;
	}
	return (audioInfo->getAccompanyPitch() != 1.0f || audioInfo->getPitchShiftLevel() != 0);
}

bool AudioEffectPostProcessor::isRobotAudioEffectFilter(AudioEffect *audioEffect) {
	if (audioEffect == NULL) {
		return false;
	}
	std::list<int>* effectFilters = audioEffect->getVocalEffectFilters();
	if (effectFilters == NULL || effectFilters->size()==0) {
		return false;
	}
	std::list<int>::iterator ite = effectFilters->begin();
	for (; ite != effectFilters->end(); ++ite) {
		if(*ite == VocalPitchShiftFilterType){
			return true;
		}
	}
	return false;
}


void AudioEffectPostProcessor::destroyFilterChains(){
	vocalEffectFilterChain->destroy(response);
	mixPostEffectFilterChain->destroy(response);
	accompanyEffectFilterChain->destroy(response);
	if (mixEffectFilter != NULL) {
		mixEffectFilter->destroy(response);
		delete mixEffectFilter;
		mixEffectFilter = NULL;
	}
}

void AudioEffectPostProcessor::setAudioEffect(AudioEffect *audioEffect) {
	LOGI("enter AudioEffectPostProcessor::setAudioEffect()...");
	isAudioEffectChanged = true;
	this->audioEffect = audioEffect;
}

void AudioEffectPostProcessor::destroy() {
	response->clear();
	this->destroyFilterChains();
	if(NULL != vocalEffectFilterChain){
		delete vocalEffectFilterChain;
		vocalEffectFilterChain = NULL;
	}
	if(NULL != accompanyEffectFilterChain){
		delete accompanyEffectFilterChain;
		accompanyEffectFilterChain = NULL;
	}
	if(NULL != mixPostEffectFilterChain){
		delete mixPostEffectFilterChain;
		mixPostEffectFilterChain = NULL;
	}
	AudioEffectProcessor::destroy();
}
