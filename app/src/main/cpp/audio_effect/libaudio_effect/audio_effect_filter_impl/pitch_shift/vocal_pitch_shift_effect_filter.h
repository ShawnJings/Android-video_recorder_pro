#ifndef VOCAL_PITCH_SHIFT_EFFECT_FILTER_H
#define VOCAL_PITCH_SHIFT_EFFECT_FILTER_H

#include "./pitch_shift_effect_filter.h"
#include "../../audio_effect_impl/robot/robot_audio_effect.h"

class VocalPitchShiftEffectFilter: public PitchShiftEffectFilter{
public:
	VocalPitchShiftEffectFilter();
	~VocalPitchShiftEffectFilter();

	void setPitch(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
};
#endif //VOCAL_PITCH_SHIFT_EFFECT_FILTER_H
