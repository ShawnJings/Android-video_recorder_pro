#ifndef ACCOMPANY_PITCH_SHIFT_EFFECT_FILTER_H
#define ACCOMPANY_PITCH_SHIFT_EFFECT_FILTER_H

#include "./pitch_shift_effect_filter.h"

class AccompanyPitchShiftEffectFilter: public PitchShiftEffectFilter{
public:
	AccompanyPitchShiftEffectFilter();
	~AccompanyPitchShiftEffectFilter();

	void setPitch(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);

private:
	float accompanyPitchShift;
	int unProcessSampleCount;
};
#endif //ACCOMPANY_PITCH_SHIFT_EFFECT_FILTER_H
