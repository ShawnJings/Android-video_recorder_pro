#include "music_merger.h"

#define LOG_TAG "SongStudio MusicMerger"

MusicMerger::MusicMerger() {
	audioEffectProcessor = NULL;
	audioSampleRate = -1;
	frameNum = 0;
}

MusicMerger::~MusicMerger() {

}

void MusicMerger::initWithAudioEffectProcessor(int audioSampleRate, AudioEffect *audioEffect) {
	this->audioSampleRate = audioSampleRate;
	if (NULL == audioEffectProcessor) {
		audioEffectProcessor = AudioEffectProcessorFactory::GetInstance()->buildLiveAudioEffectProcessor();
		audioEffectProcessor->init(audioEffect);
	}
}

/** 在直播中调用 **/
int MusicMerger::mixtureMusicBuffer(long frameNum, short* accompanySamples, int accompanySampleSize, short* audioSamples, int audioSampleSize) {
	int actualSize = min(accompanySampleSize, audioSampleSize);
	//通过frameNum计算出position（单位是ms）
	float position = frameNum * 1000 / audioSampleRate;
	AudioResponse* response = audioEffectProcessor->process(audioSamples, audioSampleSize, accompanySamples, accompanySampleSize, position, frameNum);
	// 经过sound touch处理返回的实际大小
	int *soundTouchReceiveSamples = (int *)response->get(PITCH_SHIFT_MIX_RESPONSE_KEY_RECEIVE_SAMPLES_SIZE);
	if (soundTouchReceiveSamples != NULL) {
		audioSampleSize = *soundTouchReceiveSamples;
		delete soundTouchReceiveSamples;
	}
	return actualSize;
}

void MusicMerger::setAudioEffect(AudioEffect *audioEffect) {
	if(NULL != audioEffectProcessor){
		audioEffectProcessor->setAudioEffect(audioEffect);
	}
}

void MusicMerger::stop() {
	if (NULL != audioEffectProcessor) {
		audioEffectProcessor->destroy();
		delete audioEffectProcessor;
		audioEffectProcessor = NULL;
	}
}
