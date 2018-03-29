#ifndef AUDIOEFFECT_LIVE_PROCESSOR_H_
#define AUDIOEFFECT_LIVE_PROCESSOR_H_

#include "../post/audio_effect_post_processor.h"

class AudioEffectLiveProcessor: public AudioEffectPostProcessor{
public:
	AudioEffectLiveProcessor();
	~AudioEffectLiveProcessor();

	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum);

	virtual void initFilterChains();
	virtual void destroyFilterChains();
};

#endif /* AUDIOEFFECT_LIVE_PROCESSOR_H_ */
