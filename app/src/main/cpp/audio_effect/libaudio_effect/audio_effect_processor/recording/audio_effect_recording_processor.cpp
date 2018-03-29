#include "./audio_effect_recording_processor.h"

#define LOG_TAG "AudioEffectRecordingProcessor"

AudioEffectRecordingProcessor::AudioEffectRecordingProcessor() {
	request = new AudioRequest();
	response = new AudioResponse();
}

AudioEffectRecordingProcessor::~AudioEffectRecordingProcessor() {
}

void AudioEffectRecordingProcessor::init(AudioEffect *audioEffect){
	AudioEffectProcessor::init(audioEffect);
	LOGI("init vocalEffectFilterChain");
	vocalEffectFilterChain = new AudioEffectFilterChain();
	vocalEffectFilterChain->init(audioEffect, audioEffect->getVocalEffectFilters());
	accompanyEffectFilterChain = new AudioEffectFilterChain();
	accompanyEffectFilterChain->init(audioEffect, audioEffect->getAccompanyEffectFilters());
}

AudioResponse* AudioEffectRecordingProcessor::process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum){
	request->init(vocalBuffer, vocalBufferSize, accompanyBuffer, accompanyBufferSize, position, frameNum);
	response->clear();
	vocalEffectFilterChain->doFilter(request, response);
	accompanyEffectFilterChain->doFilter(request, response);
	return response;
}

AudioResponse* AudioEffectRecordingProcessor::process(short *vocalBuffer, int vocalBufferSize, float position, long frameNum){
	request->init(vocalBuffer, vocalBufferSize, position, frameNum);
	response->clear();
	vocalEffectFilterChain->doFilter(request, response);
	return response;
}

void AudioEffectRecordingProcessor::resetFilterChains(){

}

void AudioEffectRecordingProcessor::destroy(){
	response->clear();
	vocalEffectFilterChain->destroy(response);
	AudioEffectContext::GetInstance()->copyData(response->getData());
	accompanyEffectFilterChain->destroy(response);
	AudioEffectProcessor::destroy();
}

void AudioEffectRecordingProcessor::setAudioEffect(AudioEffect *audioEffect){
	//在录制过程中，是不会调用到这里的
}
