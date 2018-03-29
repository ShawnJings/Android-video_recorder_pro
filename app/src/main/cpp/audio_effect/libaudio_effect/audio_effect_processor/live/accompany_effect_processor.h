#ifndef LIVE_ACCOMPANY_EFFECT_PROCESSOR_H_
#define LIVE_ACCOMPANY_EFFECT_PROCESSOR_H_

#include "../../audio_effect/audio_effect.h"
#include "../../audio_effect_filter/audio_effect_filter_chain.h"
#include "../audio_effect_processor.h"
#include "../../audio_effect_filter_impl/specific/mix_effect_filter.h"
#include "../../audio_effect_filter_impl/specific/pitch_shift_mix_effect_filter.h"

class AccompanyEffectProcessor: public AudioEffectProcessor{
protected:
	AudioEffectFilterChain* accompanyEffectFilterChain;

	bool isAudioEffectChanged;
	AudioEffect* audioEffect;

	void detectRebuildFilterChain();
	virtual void initFilterChains();
	virtual void destroyFilterChains();
public:
	AccompanyEffectProcessor();
	~AccompanyEffectProcessor();

	virtual void init(AudioEffect *audioEffect);
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum){
		return NULL;
	}
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, float position, long frameNum){
		return NULL;
	}

	virtual AudioResponse* processAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum);
	virtual void destroy();

	virtual void setAudioEffect(AudioEffect *audioEffect);
	virtual void resetFilterChains();

};

#endif /* LIVE_ACCOMPANY_EFFECT_PROCESSOR_H_ */
