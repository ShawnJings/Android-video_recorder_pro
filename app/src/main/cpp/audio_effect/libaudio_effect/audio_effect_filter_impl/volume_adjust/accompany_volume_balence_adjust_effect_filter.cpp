#include "./accompany_volume_balence_adjust_effect_filter.h"

#define LOG_TAG "AccompanyVolumeBalenceAdjustEffectFilter"

AccompanyVolumeBalenceAdjustEffectFilter::AccompanyVolumeBalenceAdjustEffectFilter() {
}

AccompanyVolumeBalenceAdjustEffectFilter::~AccompanyVolumeBalenceAdjustEffectFilter() {
}

int AccompanyVolumeBalenceAdjustEffectFilter::init(AudioEffect* audioEffect) {
	LOGI("AccompanyVolumeBalenceAdjustEffectFilter::init()...");
	accompanyVolume = audioEffect->getAccompanyVolume();
	vocalVolume = audioEffect->getAudioVolume();
	outputGain = audioEffect->getOutputGain();
	accompanyVolumeMax = audioEffect->getAudioInfo()->getAccompanyAGCVolume();
	return 1;
}

void AccompanyVolumeBalenceAdjustEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	bool *mergerFlag = (bool *)AudioEffectContext::GetInstance()->getResponseDataForKey(FLAG_IS_MERGE);
	//LOGI("AccompanyVolumeBalenceAdjustEffectFilter::doFilter()...");
	bool isMerger = false;
	if (mergerFlag != NULL) {
		isMerger = *mergerFlag;
	}
	if (isMerger) {
		//LOGI("AccompanyVolumeBalenceAdjustEffectFilter::doFilter() merger");

		float AL = accompanyVolume * AudioEffect::AccompanyDefault / accompanyVolumeMax;
		//float VL = vocalVolume * outputGain;
		AL = AL * AudioEffect::VocalVolumeDefault / vocalVolume;
		if (AL > (AudioEffect::AccompanyVolumeDefault / accompanyVolumeMax)) {
			AL = (AudioEffect::AccompanyVolumeDefault / accompanyVolumeMax);
		}
		this->volumeFactor = AL;
		/**
		LOGI("AccompanyVolumeBalenceAdjustEffectFilter::doFilter() accompanyVolume=%f, AudioEffect::AccompanyDefault=%f, accompanyVolumeMax=%f, volumeFactor=%f, outputGain=%f",
				accompanyVolume,
				AudioEffect::AccompanyDefault,
				accompanyVolumeMax,
				volumeFactor,
				outputGain);**/
	} else{
		//LOGI("AccompanyVolumeBalenceAdjustEffectFilter::doFilter() not merger");

		float AL = accompanyVolume * AudioEffect::AccompanyDefault / accompanyVolumeMax;
		this->volumeFactor = AL;
		/**
		LOGI("AccompanyVolumeBalenceAdjustEffectFilter::doFilter() accompanyVolume=%f, AudioEffect::AccompanyDefault=%f, accompanyVolumeMax=%f, volumeFactor=%f",
				accompanyVolume,
				AudioEffect::AccompanyDefault,
				accompanyVolumeMax,
				volumeFactor);**/

	}

	short* samples = request->getAccompanyBuffer();
	int sampleSize = request->getAccompanyBufferSize();
	this->process(samples, sampleSize);
}
