#ifndef BASE_AUDIO_INFO_H_
#define BASE_AUDIO_INFO_H_

#include "CommonTools.h"

class AudioInfo {
protected:
	/** 声道 **/
	int channels;
	/** 人声的采样频率 **/
	int audioSampleRate;
	/** 录制的时间 **/
	int recordedTimeMills;
	/** 歌曲总时间 **/
	int totalTimeMills;
	float accompanyAGCVolume;
	float audioAGCVolume;
	float accompanyPitch;
	int pitchShiftLevel;
public:
	AudioInfo();
	AudioInfo(int channels, int audioSampleRate, int recordedTimeMills, int totalTimeMills,
			float accompanyAGCVolume, float audioAGCVolume, float accompanyPitch, int pitchShiftLevel);
	~AudioInfo();
	float getAccompanyAGCVolume() {
		return accompanyAGCVolume;
	};
	float getAudioAGCVolume() {
		return audioAGCVolume;
	};
	int getChannels() {
		return channels;
	};
	int getAudioSampleRate() {
		return audioSampleRate;
	};
	int getRecordedTimeMills() {
		return recordedTimeMills;
	};
	int getTotalTimeMills() {
		return totalTimeMills;
	};
	float getAccompanyPitch() {
		return accompanyPitch;
	};
	int getPitchShiftLevel() {
		return pitchShiftLevel;
	}

};

#endif /* BASE_AUDIO_INFO_H_ */
