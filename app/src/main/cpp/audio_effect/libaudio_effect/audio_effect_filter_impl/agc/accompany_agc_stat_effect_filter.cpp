#include "./accompany_agc_stat_effect_filter.h"

#define LOG_TAG "AccompanyAGCStatEffectFilter"

AccompanyAGCStatEffectFilter::AccompanyAGCStatEffectFilter() {
}

AccompanyAGCStatEffectFilter::~AccompanyAGCStatEffectFilter() {
}

int AccompanyAGCStatEffectFilter::init(AudioEffect* audioEffect) {
	int audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	audioAGCStat = new AGCStat<short>(AGC_ACCOMPANY_TYPE, audioSampleRate, 2, AGC_ACCOMPANY_VOLUME_RATE);
	return 1;
}

void AccompanyAGCStatEffectFilter::doFilter(AudioRequest* request, AudioResponse* response){
	short* samples = request->getAccompanyBuffer();
	int sampleSize = request->getAccompanyBufferSize();
	this->process(samples, sampleSize);
}

void AccompanyAGCStatEffectFilter::destroy(AudioResponse* response) {
	if (NULL != audioAGCStat) {
		float* agcStatGain = new float;
		*agcStatGain = (float) audioAGCStat->GetGain();
		response->put(ACCOMPANY_AGC_STAT_GAIN, agcStatGain);
		delete audioAGCStat;
		audioAGCStat = NULL;
	}
}
