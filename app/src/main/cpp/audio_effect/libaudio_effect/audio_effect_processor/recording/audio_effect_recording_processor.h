#ifndef AUDIOEFFECT_RECORDING_PROCESSOR_H_
#define AUDIOEFFECT_RECORDING_PROCESSOR_H_

#include "../audio_effect_processor.h"
#include "../audio_effect_context.h"
#include "../../audio_effect_filter_impl/specific/mix_effect_filter.h"

class AudioEffectRecordingProcessor: public AudioEffectProcessor {
protected:
	AudioEffectFilterChain* vocalEffectFilterChain;
	AudioEffectFilterChain* accompanyEffectFilterChain;

public:
	AudioEffectRecordingProcessor();
	~AudioEffectRecordingProcessor();

	virtual void init(AudioEffect *audioEffect);
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum);
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, float position, long frameNum);
	virtual AudioResponse* processAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum) {
		return NULL;
	};
	virtual void setAudioEffect(AudioEffect *audioEffect);
	virtual void resetFilterChains();
	virtual void destroy();
};

#endif /* AUDIOEFFECT_RECORDING_PROCESSOR_H_ */
