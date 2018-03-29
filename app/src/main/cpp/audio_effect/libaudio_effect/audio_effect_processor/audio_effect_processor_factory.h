#ifndef AUDIOEFFECT_PROCESSOR_FACTORY_H_
#define AUDIOEFFECT_PROCESSOR_FACTORY_H_

#include "./audio_effect_processor.h"
#include "./live/audio_effect_live_processor.h"
#include "./live/accompany_effect_processor.h"
#include "./post/audio_effect_post_processor.h"
#include "./recording/audio_effect_recording_processor.h"

class AudioEffectProcessorFactory {
private:
	AudioEffectProcessorFactory();
	static AudioEffectProcessorFactory* instance;

public:
	static AudioEffectProcessorFactory* GetInstance();
	~AudioEffectProcessorFactory();

	AudioEffectProcessor* buildAccompanyEffectProcessor();
	AudioEffectProcessor* buildPostAudioEffectProcessor();
	AudioEffectProcessor* buildRecordingAudioEffectProcessor();
	AudioEffectProcessor* buildLiveAudioEffectProcessor();
};

#endif /* AUDIOEFFECT_PROCESSOR_FACTORY_H_ */
