#include "./vocal_volume_adjust_effect_filter.h"

#define LOG_TAG "VocalVolumeAdjustEffectFilter"

VocalVolumeAdjustEffectFilter::VocalVolumeAdjustEffectFilter() {
}

VocalVolumeAdjustEffectFilter::~VocalVolumeAdjustEffectFilter() {
}

int VocalVolumeAdjustEffectFilter::init(AudioEffect* audioEffect) {
	this->volumeFactor = audioEffect->getAudioVolume();
	return 1;
}

void VocalVolumeAdjustEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize();
	this->process(samples, sampleSize);
}
