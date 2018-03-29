#ifndef BASE_AUDIOEFFECT_PROCESSOR_H_
#define BASE_AUDIOEFFECT_PROCESSOR_H_

#include "../audio_effect/audio_effect.h"
#include "./audio_effect_context.h"
#include "../audio_effect_filter/audio_effect_filter_chain.h"

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
class AudioEffectProcessor {
protected:
	AudioRequest* request;
	AudioResponse* response;

public:
	virtual void init(AudioEffect *audioEffect){
		AudioEffectContext::GetInstance()->initResource();
	};
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum) = 0;
	virtual AudioResponse* process(short *vocalBuffer, int vocalBufferSize, float position, long frameNum) = 0;
	virtual AudioResponse* processAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum) = 0;
	virtual void setAudioEffect(AudioEffect *audioEffect) = 0;
	virtual void resetFilterChains() = 0;
	virtual void destroy(){
		AudioEffectContext::GetInstance()->destroyResource();
	};

	AudioResponse* getResponse(){
		return response;
	}
};

#endif /* BASE_AUDIOEFFECT_PROCESSOR_H_ */
