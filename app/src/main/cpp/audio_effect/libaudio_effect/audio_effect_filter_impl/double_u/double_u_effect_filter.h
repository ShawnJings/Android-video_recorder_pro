#ifndef DOUBLE_U_EFFECT_FILTER_H
#define DOUBLE_U_EFFECT_FILTER_H

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./doubleyou.h"

#define DOUBLE_U_INPUT_CHANNELS	2

class DoubleUEffectFilter: public AudioEffectFilter{
private:
	DoubleYou* doubleU;
	int subFrameNumThreshold;
	long lastProcessFrameNum;

	bool hasSeekAction(long actualFrameNum, int audioBufferSize);
public:
	DoubleUEffectFilter();
	~DoubleUEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};
#endif //DOUBLE_U_EFFECT_FILTER_H
