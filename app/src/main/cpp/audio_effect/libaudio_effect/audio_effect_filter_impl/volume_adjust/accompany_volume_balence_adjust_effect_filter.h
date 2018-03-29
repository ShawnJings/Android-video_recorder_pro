#ifndef AUDIOEFFECT_ACCOMPANY_VOLUME_BALENCE_ADJUST_FILTER_H_
#define AUDIOEFFECT_ACCOMPANY_VOLUME_BALENCE_ADJUST_FILTER_H_

#include "./volume_adjust_effect_filter.h"
#include "../../audio_effect_processor/audio_effect_context.h"
#include "vocal_volume_balence_adjust_effect_filter.h"

class AccompanyVolumeBalenceAdjustEffectFilter: public VolumeAdjustEffectFilter{
public:
	AccompanyVolumeBalenceAdjustEffectFilter();
	~AccompanyVolumeBalenceAdjustEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);

private:
	float accompanyVolume;
	float vocalVolume;
	float outputGain;
	float accompanyVolumeMax;
};

#endif /* AUDIOEFFECT_ACCOMPANY_VOLUME_BALENCE_ADJUST_FILTER_H_ */
