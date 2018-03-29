#include "./vocal_agc_stat_effect_filter.h"

#define LOG_TAG "VocalAGCStatEffectFilter"

VocalAGCStatEffectFilter::VocalAGCStatEffectFilter() {
}

VocalAGCStatEffectFilter::~VocalAGCStatEffectFilter() {
}

int VocalAGCStatEffectFilter::init(AudioEffect* audioEffect) {
	int audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	audioAGCStat = new AGCStat<short>(AGC_AUDIO_TYPE, audioSampleRate, 1, AGC_AUDIO_VOLUME_RATE);
	return 1;
}

void VocalAGCStatEffectFilter::doFilter(AudioRequest* request, AudioResponse* response){
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize() / 2;
	this->process(samples, sampleSize);
}

void VocalAGCStatEffectFilter::destroy(AudioResponse* response) {
	if (NULL != audioAGCStat) {
		float* agcStatGain = new float;
		*agcStatGain = (float) audioAGCStat->GetGain();
		response->put(VOCAL_AGC_STAT_GAIN, agcStatGain);
		delete audioAGCStat;
		audioAGCStat = NULL;
	}
}
