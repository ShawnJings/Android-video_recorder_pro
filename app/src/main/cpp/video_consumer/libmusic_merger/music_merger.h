#ifndef SONGSTUDIO_MUSIC_MERGER_H
#define SONGSTUDIO_MUSIC_MERGER_H

#include "./../libcommon/CommonTools.h"
#include "../audio_effect/libaudio_effect/audio_effect_processor/audio_effect_processor_factory.h"
#include "../audio_effect/libaudio_effect/audio_effect/audio_effect.h"

class MusicMerger {
protected:
	AudioEffectProcessor* audioEffectProcessor;

	long frameNum;
	int audioSampleRate;
public:
	MusicMerger();
	~MusicMerger();
	/** 直播中试音以及普通录制歌曲的时候由于不会保存AAC文件，所以只处理音效  现在又有一种情况 是视频也不会编码AAC文件 **/
	virtual void initWithAudioEffectProcessor(int audioSampleRate, AudioEffect *audioEffect);
	/** 设置音效可以实时调节 **/
	void setAudioEffect(AudioEffect *audioEffect);
	/** 销毁这个processor **/
	void stop();

	/** ......直播过程中调用到的函数...... **/
	//在直播中调用
	virtual int mixtureMusicBuffer(long frameNum, short* accompanySamples, int accompanySampleSize, short* audioSamples, int audioSampleSize);
};

#endif //SONGSTUDIO_MUSIC_MERGER_H
