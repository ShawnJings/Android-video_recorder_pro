#ifndef AUDIOEFFECT_ACCOMPANY_VOLUME_ADJUST_FILTER_H_
#define AUDIOEFFECT_ACCOMPANY_VOLUME_ADJUST_FILTER_H_

#include "./volume_adjust_effect_filter.h"

class AccompanyVolumeAdjustEffectFilter: public VolumeAdjustEffectFilter{
public:
	AccompanyVolumeAdjustEffectFilter();
	~AccompanyVolumeAdjustEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
};

#endif /* AUDIOEFFECT_ACCOMPANY_VOLUME_ADJUST_FILTER_H_ */
