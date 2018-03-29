#include "./accompany_pitch_shift_effect_filter.h"

#define LOG_TAG "AccompanyPitchShiftEffectFilter"

AccompanyPitchShiftEffectFilter::AccompanyPitchShiftEffectFilter() {
	unProcessSampleCount = 0;
}

AccompanyPitchShiftEffectFilter::~AccompanyPitchShiftEffectFilter() {
}

void AccompanyPitchShiftEffectFilter::setPitch(AudioEffect* audioEffect){
	accompanyPitchShift = audioEffect->getAudioInfo()->getAccompanyPitch();
	soundTouch->setPitch(accompanyPitchShift);
	//		soundTouch -> setPitchSemiTones(10);
	//		soundTouch -> setRateChange(-0.7f);
	//		soundTouch -> setTempoChange(0.5f);
	soundTouch->setSetting(SETTING_SEEKWINDOW_MS, 0);
	soundTouch->setSetting(SETTING_SEQUENCE_MS, 0);
	soundTouch->setSetting(SETTING_OVERLAP_MS, 8);
	soundTouch->setSetting(SETTING_USE_QUICKSEEK, 1);
}

void AccompanyPitchShiftEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
//	LOGI("AccompanyPitchShiftEffectFilter::doFilter accompanyPitchShift %.3f", accompanyPitchShift);
	if (accompanyPitchShift != 1.0f) {
		short* samples = request->getAccompanyBuffer();
		int sampleSize = request->getAccompanyBufferSize();
		int actualSampleSize = this->process(samples, sampleSize) * 2;
		//LOGI("AccompanyPitchShiftEffectFilter::doFilter() in:sampleSize=%d,  out:actualSampleSize=%d", sampleSize, actualSampleSize);
		response->put(ACCOMPANYRESPONSE_KEY_RECEIVESAMPLES_SIZE, new int(actualSampleSize));
		unProcessSampleCount += (sampleSize-actualSampleSize);
	}
}

void AccompanyPitchShiftEffectFilter::destroy(AudioResponse* response) {
	PitchShiftEffectFilter::destroy(response);
	response->put(ACCOMPANYRESPONSE_KEY_PITCHSHIFT_UNPROCESS_SIZE, new int(unProcessSampleCount));
}
