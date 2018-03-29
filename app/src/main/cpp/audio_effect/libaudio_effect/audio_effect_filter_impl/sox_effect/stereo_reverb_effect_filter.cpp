#include "stereo_reverb_effect_filter.h"

#define LOG_TAG "StereoReverbEffectFilter"

StereoReverbEffectFilter::StereoReverbEffectFilter() {
}

StereoReverbEffectFilter::~StereoReverbEffectFilter() {
}

int StereoReverbEffectFilter::init(AudioEffect* audioEffect) {
	isInitialized = false;
	this->audioEffect = audioEffect;
	reverbEffect = new SoxReverbEffect(audioEffect->getAudioInfo()->getAudioSampleRate(), audioEffect->getAudioInfo()->getChannels());
	reverbRatio = audioEffect->getFilterChainParam()->getReverbRatio();
	return 1;
}

void StereoReverbEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != reverbEffect) {
		short* audioBuffer = request->getVocalBuffer();
		int audioBufferSize = request->getVocalBufferSize();
		if(!isInitialized){
			reverbEffect->init(audioBufferSize / 2);
			reverbEffect->initChain(audioEffect->getFilterChainParam());
			isInitialized = true;
		}

		short* directAudioBuffer = new short[audioBufferSize];
		for(int i = 0; i < audioBufferSize ; i++){
			directAudioBuffer[i] = audioBuffer[i];
		}

		//双声道转单声道
		short* monoAudioBuffer = new short[audioBufferSize];
		for (int i=0; i<audioBufferSize/2; i++) {
			short l = audioBuffer[2*i];
			short r = audioBuffer[2*i+1];
			monoAudioBuffer[i] = l/2+r/2;
		}

		//混响处理
		reverbEffect->process(monoAudioBuffer, audioBufferSize / 2);
		//4、合成直达声与混响参数
		float directAudioRatio = 1 - reverbRatio;
		for (int i = 0; i < audioBufferSize; i++) {
			audioBuffer[i] = directAudioBuffer[i] * directAudioRatio + monoAudioBuffer[i] * reverbRatio;
		}

		//LOGI("StereoReverbEffectFilter::doFilter() reverbRatio=%f, audioBufferSize=%d", reverbRatio, audioBufferSize);

		if (NULL != monoAudioBuffer) {
			delete[] monoAudioBuffer;
		}
		if (NULL != directAudioBuffer) {
			delete[] directAudioBuffer;
		}
	}
}

void StereoReverbEffectFilter::destroy(AudioResponse* response) {
	if (NULL != reverbEffect) {
		reverbEffect->destroyChain();
		reverbEffect->destroy();
		delete reverbEffect;
		reverbEffect = NULL;
	}
}
