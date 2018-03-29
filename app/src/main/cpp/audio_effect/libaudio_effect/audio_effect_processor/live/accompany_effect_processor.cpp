#include "accompany_effect_processor.h"

#define LOG_TAG "AccompanyEffectProcessor"

AccompanyEffectProcessor::AccompanyEffectProcessor() {
	request = new AudioRequest();
	response = new AudioResponse();
}

AccompanyEffectProcessor::~AccompanyEffectProcessor() {
}

void AccompanyEffectProcessor::init(AudioEffect *audioEffect) {
	this->audioEffect = audioEffect;
	isAudioEffectChanged = false;
	accompanyEffectFilterChain = new AudioEffectFilterChain();
	this->initFilterChains();
}

AudioResponse* AccompanyEffectProcessor::processAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum) {
	response->clear();
	detectRebuildFilterChain();
	request->initAccompany(accompanyBuffer, accompanyBufferSize, position, frameNum);
	accompanyEffectFilterChain->doFilter(request, response);
	return response;
}

void AccompanyEffectProcessor::detectRebuildFilterChain(){
	if(isAudioEffectChanged){
		this->destroyFilterChains();
		this->initFilterChains();
		isAudioEffectChanged = false;
	}
}

void AccompanyEffectProcessor::resetFilterChains(){
	setAudioEffect(audioEffect);
}

void AccompanyEffectProcessor::initFilterChains(){
	accompanyEffectFilterChain->init(audioEffect, audioEffect->getAccompanyEffectFilters());
}

void AccompanyEffectProcessor::destroyFilterChains(){
	accompanyEffectFilterChain->destroy(response);
}

void AccompanyEffectProcessor::setAudioEffect(AudioEffect *audioEffect) {
	LOGI("enter AccompanyEffectProcessor::setAudioEffect()...");
	isAudioEffectChanged = true;
	this->audioEffect = audioEffect;
}

void AccompanyEffectProcessor::destroy() {
	response->clear();
	this->destroyFilterChains();
	if(NULL != accompanyEffectFilterChain){
		delete accompanyEffectFilterChain;
		accompanyEffectFilterChain = NULL;
	}
}
