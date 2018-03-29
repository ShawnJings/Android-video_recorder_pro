#ifndef AUDIOEFFECT_ACCOMPANY_DELAY_OUTPUT_FILTER_H_
#define AUDIOEFFECT_ACCOMPANY_DELAY_OUTPUT_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#define AUTO_TUNE_DELAY_TIME_MILLS		50

class AccompanyDelayOutputEffectFilter: public AudioEffectFilter{
private:
	short* delayCircleBuffer;
	int delayCircleBufferCursor;
	int delayBufferSize;

	short delay(short sample);
public:
	AccompanyDelayOutputEffectFilter();
	~AccompanyDelayOutputEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_ACCOMPANY_DELAY_OUTPUT_FILTER_H_ */
