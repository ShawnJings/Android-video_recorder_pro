#include "./compressor_effect_filter.h"

#define LOG_TAG "CompressorEffectFilter"

CompressorEffectFilter::CompressorEffectFilter() {
}

CompressorEffectFilter::~CompressorEffectFilter() {
}

int CompressorEffectFilter::init(AudioEffect* audioEffect) {
	isInitialized = false;
	this->audioEffect = audioEffect;
	compressorEffect = new SoxCompressorEffect(audioEffect->getAudioInfo()->getAudioSampleRate(), audioEffect->getAudioInfo()->getChannels());
	return 1;
}

void CompressorEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != compressorEffect) {
		short* audioBuffer = request->getVocalBuffer();
		int audioBufferSize = request->getVocalBufferSize();
		if(!isInitialized){
			compressorEffect->init(audioBufferSize / 2);
			compressorEffect->initChain(audioEffect->getFilterChainParam());
			isInitialized = true;
		}
		compressorEffect->process(audioBuffer, audioBufferSize / 2);
	}
}

void CompressorEffectFilter::destroy(AudioResponse* response) {
	if (NULL != compressorEffect) {
		compressorEffect->destroyChain();
		compressorEffect->destroy();
		delete compressorEffect;
		compressorEffect = NULL;
	}
}
