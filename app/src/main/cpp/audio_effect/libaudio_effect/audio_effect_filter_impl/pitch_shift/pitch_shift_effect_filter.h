#ifndef PITCH_SHIFT_EFFECT_FILTER_H
#define PITCH_SHIFT_EFFECT_FILTER_H

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./include/SoundTouch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace soundtouch;

class PitchShiftEffectFilter: public AudioEffectFilter{
protected:
	SoundTouch* soundTouch;
	int channels;

	int process(short* samples, int sampleSize);
public:
	PitchShiftEffectFilter();
	~PitchShiftEffectFilter();

	int init(AudioEffect* audioEffect);
	virtual void setPitch(AudioEffect* audioEffect) = 0;
	virtual void doFilter(AudioRequest* request, AudioResponse* response) = 0;
	void destroy(AudioResponse* response);
};
#endif //PITCH_SHIFT_EFFECT_FILTER_H
