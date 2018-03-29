#include "./double_u_effect_filter.h"

#define LOG_TAG "DoubleUEffectFilter"

DoubleUEffectFilter::DoubleUEffectFilter() {
	doubleU = NULL;
	lastProcessFrameNum = 0.0f;
}

DoubleUEffectFilter::~DoubleUEffectFilter() {
}

int DoubleUEffectFilter::init(AudioEffect* audioEffect) {
	int audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	doubleU = new DoubleYou();
	doubleU->Init(audioSampleRate, DOUBLE_U_INPUT_CHANNELS);
	subFrameNumThreshold = audioSampleRate * CORRECTION_SEEK_FRAME_IN_SECS;
	return 1;
}

void DoubleUEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* audioBuffer = request->getVocalBuffer();
	int audioBufferSize = request->getVocalBufferSize();
	long frameNum = request->getFrameNum();
	if (hasSeekAction(frameNum, audioBufferSize)) {
		LOGI("need seek doubleU effect...");
		doubleU->ResetShiftDeposit();
	}
	short* outAudioBuffer = new short[audioBufferSize];
	//DoubleU返回双声道数据
	doubleU->DYFlow(audioBuffer, outAudioBuffer, audioBufferSize);
	memcpy(audioBuffer, outAudioBuffer, sizeof(short) * audioBufferSize);
	delete[] outAudioBuffer;
	lastProcessFrameNum = frameNum;
}

bool DoubleUEffectFilter::hasSeekAction(long actualFrameNum, int audioBufferSize) {
	//this method should detect this buffer has or has not seek action based on expectedFrameNum and actualFrameNum
	long expectedFrameNum = lastProcessFrameNum + audioBufferSize / 2;
	int subFrameNum = abs(expectedFrameNum - actualFrameNum);
	if(-1 == actualFrameNum || subFrameNum < subFrameNumThreshold){
		return false;
	}
	return true;
}

void DoubleUEffectFilter::destroy(AudioResponse* response) {
	if (NULL != doubleU) {
		delete doubleU;
		doubleU = NULL;
	}
}
