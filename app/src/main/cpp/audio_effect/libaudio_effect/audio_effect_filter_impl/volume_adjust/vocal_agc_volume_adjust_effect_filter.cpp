#include "./vocal_agc_volume_adjust_effect_filter.h"

#define LOG_TAG "VocalAGCVolumeAdjustEffectFilter"

VocalAGCVolumeAdjustEffectFilter::VocalAGCVolumeAdjustEffectFilter() {
}

VocalAGCVolumeAdjustEffectFilter::~VocalAGCVolumeAdjustEffectFilter() {
}

int VocalAGCVolumeAdjustEffectFilter::init(AudioEffect* audioEffect) {
	this->volumeFactor = audioEffect->getAudioInfo()->getAudioAGCVolume();
	LOGI("VocalAGCVolumeAdjustEffectFilter::init() volumeFactor=%f", volumeFactor);
	return 1;
}

void VocalAGCVolumeAdjustEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize() / 2;
	this->process(samples, sampleSize);
}
