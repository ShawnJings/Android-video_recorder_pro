#include "./vocal_pitch_shift_effect_filter.h"

#define LOG_TAG "VocalPitchShiftEffectFilter"

VocalPitchShiftEffectFilter::VocalPitchShiftEffectFilter() {
}

VocalPitchShiftEffectFilter::~VocalPitchShiftEffectFilter() {
}

void VocalPitchShiftEffectFilter::setPitch(AudioEffect* audioEffect){
	RobotAudioEffect* robotAudioEffect = (RobotAudioEffect*)audioEffect;
	soundTouch->setPitch(robotAudioEffect->getVocalPitch());
	LOGI("VocalPitchShiftEffectFilter::setPitch() vocalPitch=%f", robotAudioEffect->getVocalPitch());
	//		soundTouch -> setPitchSemiTones(10);
	//		soundTouch -> setRateChange(-0.7f);
	//		soundTouch -> setTempoChange(0.5f);
	soundTouch->setSetting(SETTING_SEEKWINDOW_MS, 15);
	soundTouch->setSetting(SETTING_SEQUENCE_MS, 20);
	soundTouch->setSetting(SETTING_OVERLAP_MS, 8);
	soundTouch->setSetting(SETTING_USE_QUICKSEEK, 1);
}

void VocalPitchShiftEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize();
	int actualSampleSize = this->process(samples, sampleSize) * 2;
	LOGI("VocalPitchShiftEffectFilter::doFilter() sampleSize=%d, actualSampleSize=%d", sampleSize, actualSampleSize);
	response->put(AUDIORESPONSE_KEY_RECEIVESAMPLES_SIZE, new int(actualSampleSize));
}
