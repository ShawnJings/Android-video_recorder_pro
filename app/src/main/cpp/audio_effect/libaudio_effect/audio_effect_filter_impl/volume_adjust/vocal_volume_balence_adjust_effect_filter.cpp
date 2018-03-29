#include "./vocal_volume_balence_adjust_effect_filter.h"

#define LOG_TAG "VocalVolumeBalenceAdjustEffectFilter"

VocalVolumeBalenceAdjustEffectFilter::VocalVolumeBalenceAdjustEffectFilter() {
}

VocalVolumeBalenceAdjustEffectFilter::~VocalVolumeBalenceAdjustEffectFilter() {
}

int VocalVolumeBalenceAdjustEffectFilter::init(AudioEffect* audioEffect) {
	LOGI("VocalVolumeBalenceAdjustEffectFilter::init()...");
	accompanyVolume = audioEffect->getAccompanyVolume();
	vocalVolume = audioEffect->getAudioVolume();
	outputGain = audioEffect->getOutputGain();
	accompanyVolumeMax = audioEffect->getAudioInfo()->getAccompanyAGCVolume();
	return 1;
}

void VocalVolumeBalenceAdjustEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	bool *mergerFlag = (bool *)AudioEffectContext::GetInstance()->getResponseDataForKey(FLAG_IS_MERGE);
	//LOGI("VocalVolumeBalenceAdjustEffectFilter::doFilter()...");
	bool isMerger = false;
	if (mergerFlag != NULL) {
		isMerger = *mergerFlag;
	}
	if (isMerger) {
		//LOGI("VocalVolumeBalenceAdjustEffectFilter::doFilter() merger");

		float AL = accompanyVolume * AudioEffect::AccompanyDefault / accompanyVolumeMax;
		//float VL = vocalVolume * outputGain;
		AL = AL * AudioEffect::VocalVolumeDefault / vocalVolume;

		float VL = AudioEffect::VocalVolumeDefault * outputGain;
		if (AL > (AudioEffect::AccompanyVolumeDefault / accompanyVolumeMax)) {
			VL = VL * (AudioEffect::AccompanyVolumeDefault / accompanyVolumeMax) / AL;
		}
		this->volumeFactor = VL;
		/**
		LOGI("VocalVolumeBalenceAdjustEffectFilter::doFilter() accompanyVolume=%f, AudioEffect::AccompanyDefault=%f, accompanyVolumeMax=%f, volumeFactor=%f, outputGain=%f",
				accompanyVolume,
				AudioEffect::AccompanyDefault,
				accompanyVolumeMax,
				volumeFactor,
				outputGain);**/
	} else{
		//LOGI("VocalVolumeBalenceAdjustEffectFilter::doFilter() not merger");

		float VL = vocalVolume * outputGain;
		this->volumeFactor = VL;
		/**
		LOGI("VocalVolumeBalenceAdjustEffectFilter::doFilter() accompanyVolume=%f, vocalVolume=%f, outputGain=%f, volumeFactor=%f",
				accompanyVolume, vocalVolume, outputGain, volumeFactor);**/
	}
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize();
	this->process(samples, sampleSize);
}
