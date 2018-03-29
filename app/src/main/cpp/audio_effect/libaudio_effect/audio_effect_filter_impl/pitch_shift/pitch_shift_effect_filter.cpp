#include "./pitch_shift_effect_filter.h"

#define LOG_TAG "PitchShiftEffectFilter"

PitchShiftEffectFilter::PitchShiftEffectFilter() {
	soundTouch = NULL;
}

PitchShiftEffectFilter::~PitchShiftEffectFilter() {
}

int PitchShiftEffectFilter::init(AudioEffect* audioEffect) {
	soundTouch = new SoundTouch();
	soundTouch->setSampleRate(audioEffect->getAudioInfo()->getAudioSampleRate());
	this->setPitch(audioEffect);
	//这里Channel默认为2
	this->channels = 2;
	soundTouch->setChannels(channels);
	return 1;
}

int PitchShiftEffectFilter::process(short* samples, int sampleSize) {
	//放入要处理的samples
	soundTouch->putSamples((SAMPLETYPE *) samples, sampleSize / channels);
	//取回已经处理好的data，并且返回响应data的个数
	int actualSampleSize = soundTouch->receiveSamples((SAMPLETYPE *) samples, sampleSize / channels);
	return actualSampleSize;
}

void PitchShiftEffectFilter::destroy(AudioResponse* response) {
	if (NULL != soundTouch) {
		delete soundTouch;
		soundTouch = NULL;
	}
}
