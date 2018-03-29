#include "./audio_info.h"

#define LOG_TAG "AudioInfo"

AudioInfo::AudioInfo(){
}

AudioInfo::AudioInfo(int channels, int audioSampleRate, int recordedTimeMills, int totalTimeMills,
		float accompanyAGCVolume, float audioAGCVolume, float accompanyPitch, int pitchShiftLevel) {
	this->channels = channels;
	this->audioSampleRate = audioSampleRate;
	this->recordedTimeMills = recordedTimeMills;
	this->totalTimeMills = totalTimeMills;
	this->accompanyAGCVolume = accompanyAGCVolume;
	this->audioAGCVolume = audioAGCVolume;
	this->accompanyPitch = accompanyPitch;
	this->pitchShiftLevel = pitchShiftLevel;
}

AudioInfo::~AudioInfo(){
}
