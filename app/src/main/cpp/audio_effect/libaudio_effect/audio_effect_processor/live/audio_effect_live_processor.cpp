#include "./audio_effect_live_processor.h"

#define LOG_TAG "AudioEffectLiveProcessor"

AudioEffectLiveProcessor::AudioEffectLiveProcessor() {
}

AudioEffectLiveProcessor::~AudioEffectLiveProcessor() {
}

void AudioEffectLiveProcessor::initFilterChains() {
	vocalEffectFilterChain->init(audioEffect, audioEffect->getVocalEffectFilters());
	mixPostEffectFilterChain->init(audioEffect, audioEffect->getMixPostEffectFilters());
	//这种实现是 我们的MixEffectFilter来处理Accompany和Vocal的buffer不一样的情况
//	mixEffectFilter = new PitchShiftMixEffectFilter();
	//不论怎么样, 由客户端代码自己处理 伴奏每个Packet不足200ms的bufferSize的事情
	mixEffectFilter = new MixEffectFilter();
	mixEffectFilter->init(audioEffect);
}

void AudioEffectLiveProcessor::destroyFilterChains() {
	vocalEffectFilterChain->destroy(response);
	mixPostEffectFilterChain->destroy(response);
	if (mixEffectFilter != NULL) {
		mixEffectFilter->destroy(response);
		delete mixEffectFilter;
		mixEffectFilter = NULL;
	}
}

AudioResponse* AudioEffectLiveProcessor::process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum){
	detectRebuildFilterChain();
	request->init(vocalBuffer, vocalBufferSize, accompanyBuffer, accompanyBufferSize, position, frameNum);
	response->clear();
	vocalEffectFilterChain->doFilter(request, response);
//	response->put(ACCOMPANYRESPONSE_KEY_RECEIVESAMPLES_SIZE, new int(accompanyBufferSize));
	mixEffectFilter->doFilter(request, response);
	mixPostEffectFilterChain->doFilter(request, response);
	return response;
}
