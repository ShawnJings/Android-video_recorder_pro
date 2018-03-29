#include "./reverb_effect_filter.h"

#define LOG_TAG "ReverbEffectFilter"

ReverbEffectFilter::ReverbEffectFilter() {
}

ReverbEffectFilter::~ReverbEffectFilter() {
}

int ReverbEffectFilter::init(AudioEffect* audioEffect) {
	isInitialized = false;
	this->audioEffect = audioEffect;
	reverbEffect = new SoxReverbEffect(audioEffect->getAudioInfo()->getAudioSampleRate(), audioEffect->getAudioInfo()->getChannels());
	reverbRatio = audioEffect->getFilterChainParam()->getReverbRatio();
	return 1;
}

void ReverbEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != reverbEffect) {
		short* audioBuffer = request->getVocalBuffer();
		int audioBufferSize = request->getVocalBufferSize();
		if(!isInitialized){
			reverbEffect->init(audioBufferSize / 2);
			reverbEffect->initChain(audioEffect->getFilterChainParam());
			isInitialized = true;
		}
		short* directAudioBuffer = new short[audioBufferSize];
		for(int i = 0; i < audioBufferSize/2 ; i++){
			directAudioBuffer[2 * i] = audioBuffer[i];
			directAudioBuffer[2 * i + 1] = audioBuffer[i];
		}
		//混响处理
		reverbEffect->process(audioBuffer, audioBufferSize / 2);
		//4、合成直达声与混响参数
		float directAudioRatio = 1 - reverbRatio;
		for (int i = 0; i < audioBufferSize; i++) {
			audioBuffer[i] = directAudioBuffer[i] * directAudioRatio + audioBuffer[i] * reverbRatio;
		}
		if (NULL != directAudioBuffer) {
			delete[] directAudioBuffer;
		}
	}
}

void ReverbEffectFilter::destroy(AudioResponse* response) {
	if (NULL != reverbEffect) {
		reverbEffect->destroyChain();
		reverbEffect->destroy();
		delete reverbEffect;
		reverbEffect = NULL;
	}
}
