#include "./accompany_volume_adjust_effect_filter.h"

#define LOG_TAG "AccompanyVolumeAdjustEffectFilter"

AccompanyVolumeAdjustEffectFilter::AccompanyVolumeAdjustEffectFilter() {
}

AccompanyVolumeAdjustEffectFilter::~AccompanyVolumeAdjustEffectFilter() {
}

int AccompanyVolumeAdjustEffectFilter::init(AudioEffect* audioEffect) {
	this->volumeFactor = audioEffect->getAccompanyVolume();
	return 1;
}

void AccompanyVolumeAdjustEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getAccompanyBuffer();
	int sampleSize = request->getAccompanyBufferSize();
	this->process(samples, sampleSize);
}
