#include "./accompany_agc_volume_adjust_effect_filter.h"

#define LOG_TAG "AccompanyAGCVolumeAdjustEffectFilter"

AccompanyAGCVolumeAdjustEffectFilter::AccompanyAGCVolumeAdjustEffectFilter() {
}

AccompanyAGCVolumeAdjustEffectFilter::~AccompanyAGCVolumeAdjustEffectFilter() {
}

int AccompanyAGCVolumeAdjustEffectFilter::init(AudioEffect* audioEffect) {
	this->volumeFactor = audioEffect->getAudioInfo()->getAccompanyAGCVolume();
	LOGI("AccompanyAGCVolumeAdjustEffectFilter::init() volumeFactor=%f", volumeFactor);
	return 1;
}

void AccompanyAGCVolumeAdjustEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getAccompanyBuffer();
	int sampleSize = request->getAccompanyBufferSize();
	this->process(samples, sampleSize);
}
