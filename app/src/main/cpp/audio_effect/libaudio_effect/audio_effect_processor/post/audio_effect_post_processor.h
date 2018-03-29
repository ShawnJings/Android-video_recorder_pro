#ifndef AUDIOEFFECT_POST_PROCESSOR_H_
#define AUDIOEFFECT_POST_PROCESSOR_H_

#include "../../audio_effect/audio_effect.h"
#include "../../audio_effect_filter/audio_effect_filter_chain.h"
#include "../audio_effect_processor.h"
#include "../../audio_effect_filter_impl/specific/mix_effect_filter.h"
#include "../../audio_effect_filter_impl/specific/pitch_shift_mix_effect_filter.h"

/**
 *      AGCFilter->CompressorFilter->....->ReverbFilter
 *                           |
 *                vocalEffectFilterChain-----------------------
 *                                                            |---->MIX/Mono2Stereo-->mixPostEffectFilterChain
 *              accompanyEffectFilterChain---------------------                                 |
 *                           |                                                        FadeOutFilter->....
 *      AGCFilter->PitchShiftFilter->....->ReverbFilter
 *
 */
class AudioEffectPostProcessor: public AudioEffectProcessor{
protected:
	AudioEffectFilterChain* vocalEffectFilterChain;
	AudioEffectFilterChain* mixPostEffectFilterChain;
	AudioEffectFilterChain* accompanyEffectFilterChain;
	MixEffectFilter* mixEffectFilter;

	bool isAudioEffectChanged;
	AudioEffect* audioEffect;

	void detectRebuildFilterChain();
	virtual void initFilterChains();
	virtual void destroyFilterChains();
public:
	AudioEffectPostProcessor();
	~AudioEffectPostProcessor();

	virtual void init(AudioEffect *audioEffect);
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum);
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, float position, long frameNum);
	virtual AudioResponse* processAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum);
	virtual void destroy();

	virtual void setAudioEffect(AudioEffect *audioEffect);
	virtual void resetFilterChains();

protected:
	bool isAccompanyPitchShift(AudioEffect *audioEffect);
	bool isRobotAudioEffectFilter(AudioEffect *audioEffect);
};

#endif /* AUDIOEFFECT_POST_PROCESSOR_H_ */
