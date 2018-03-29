#include "./aacompany_delay_output_effect_filter.h"

#define LOG_TAG "AccompanyDelayOutputEffectFilter"

AccompanyDelayOutputEffectFilter::AccompanyDelayOutputEffectFilter() {
	delayCircleBuffer = NULL;
}

AccompanyDelayOutputEffectFilter::~AccompanyDelayOutputEffectFilter() {
}

int AccompanyDelayOutputEffectFilter::init(AudioEffect* audioEffect) {
	int audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	int channels = 2;
	float sampleCountInTimeMills = (float) (audioSampleRate * channels) / 1000;
	delayBufferSize = AUTO_TUNE_DELAY_TIME_MILLS * sampleCountInTimeMills;
	delayCircleBuffer = new short[delayBufferSize];
	memset(delayCircleBuffer, 0, delayBufferSize * sizeof(short));
	delayCircleBufferCursor = 0;
	return 1;
}

void AccompanyDelayOutputEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* accompanyBuffer = request->getAccompanyBuffer();
	int audioBufferSize = request->getAccompanyBufferSize();
	for (int i = 0; i < audioBufferSize; i++) {
		accompanyBuffer[i] = delay(accompanyBuffer[i]);
	}
}

short AccompanyDelayOutputEffectFilter::delay(short sample) {
	short delaySample = delayCircleBuffer[delayCircleBufferCursor];
	delayCircleBuffer[delayCircleBufferCursor] = sample;
	delayCircleBufferCursor++;
	if (delayCircleBufferCursor == delayBufferSize) {
		delayCircleBufferCursor = 0;
	}
	return delaySample;
}

void AccompanyDelayOutputEffectFilter::destroy(AudioResponse* response) {
	if (NULL != delayCircleBuffer) {
		delete[] delayCircleBuffer;
		delayCircleBuffer = NULL;
	}
}
