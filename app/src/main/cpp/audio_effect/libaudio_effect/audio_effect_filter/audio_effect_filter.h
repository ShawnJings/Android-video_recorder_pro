#ifndef BASE_AUDIOEFFECT_FILTER_H_
#define BASE_AUDIOEFFECT_FILTER_H_

#include "../audio_effect/audio_effect.h"
#include "../audio_effect_processor/audio_request.h"
#include "../audio_effect_processor/audio_response.h"
#include "CommonTools.h"

//其中有一些音效是判断frameNum来看是否需要seek操作
//比如说DoubleU、电音、合唱团 由于视频在解码的时候根据时间算的frameNum所以有一个最大允许误差 50ms
#define CORRECTION_SEEK_FRAME_IN_SECS				0.05

class AudioEffectFilter {
public:
	virtual int init(AudioEffect* audioEffect) = 0;
	virtual void doFilter(AudioRequest* request, AudioResponse* response) = 0;
	virtual void destroy(AudioResponse* response) = 0;
};

#endif /* BASE_AUDIOEFFECT_FILTER_H_ */
