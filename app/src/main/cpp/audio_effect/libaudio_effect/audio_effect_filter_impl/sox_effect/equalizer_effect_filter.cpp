#include "./equalizer_effect_filter.h"

#define LOG_TAG "EqualizerEffectFilter"

EqualizerEffectFilter::EqualizerEffectFilter() {
}

EqualizerEffectFilter::~EqualizerEffectFilter() {
}

int EqualizerEffectFilter::init(AudioEffect* audioEffect) {
	isInitialized = false;
	this->audioEffect = audioEffect;
	equalizerEffect = new SoxEqualizerEffect(audioEffect->getAudioInfo()->getAudioSampleRate(), audioEffect->getAudioInfo()->getChannels());
	return 1;
}

void EqualizerEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != equalizerEffect) {
		short* audioBuffer = request->getVocalBuffer();
		int audioBufferSize = request->getVocalBufferSize();
		if(!isInitialized){
			equalizerEffect->init(audioBufferSize / 2);
			equalizerEffect->initChain(audioEffect->getFilterChainParam());
			isInitialized = true;
		}
		equalizerEffect->process(audioBuffer, audioBufferSize / 2);
	}
}

void EqualizerEffectFilter::destroy(AudioResponse* response) {
	if (NULL != equalizerEffect) {
		equalizerEffect->destroyChain();
		equalizerEffect->destroy();
		delete equalizerEffect;
		equalizerEffect = NULL;
	}
}
