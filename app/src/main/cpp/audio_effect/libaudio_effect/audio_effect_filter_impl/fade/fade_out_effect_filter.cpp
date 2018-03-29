#include "./fade_out_effect_filter.h"

#define LOG_TAG "FadeOutEffectFilter"

FadeOutEffectFilter::FadeOutEffectFilter() {
	fadeAudioEffect = NULL;
}

FadeOutEffectFilter::~FadeOutEffectFilter() {
}

int FadeOutEffectFilter::init(AudioEffect* audioEffect) {
	this->latestDiffTimeMills = 0;
	this->recordedTimeMills = audioEffect->getAudioInfo()->getRecordedTimeMills();
	this->totalTimeMills = audioEffect->getAudioInfo()->getTotalTimeMills();
	int audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	int channels = 2;
	fadeAudioEffect = new FadeAudio<short>(FADE_OUT_TYPE, audioSampleRate * channels * FADE_OUT_SECONDS);
	return 1;
}

void FadeOutEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize();
	int audioTimeMills = request->getPosition();
	if(recordedTimeMills >= (totalTimeMills - 1000) || recordedTimeMills < MIN_SECONDS_WITH_FADE_OUT * 1000){
		//如果小于10s或者演唱时间在总时间的差值小于1秒 都不做FADE_OUT
		return;
	}
//	LOGI("FadeOutFilter doFilter recordedTimeMills is %d audioTimeMills is %.4f", recordedTimeMills, audioTimeMills);
	int diffTimeMills = recordedTimeMills - audioTimeMills;
	if(diffTimeMills <= (FADE_OUT_SECONDS * 1000)){
//		LOGI("FadeOutFilter doFilter diffTimeMills is %d latestDiffTimeMills is %d", diffTimeMills, latestDiffTimeMills);
		if(diffTimeMills > latestDiffTimeMills){
			fadeAudioEffect->Init();
		}
		fadeAudioEffect->FadeFlow(samples, sampleSize);
		latestDiffTimeMills = diffTimeMills;
	}
}

void FadeOutEffectFilter::destroy(AudioResponse* response) {
	if(NULL != fadeAudioEffect){
		delete fadeAudioEffect;
		fadeAudioEffect = NULL;
	}
}
