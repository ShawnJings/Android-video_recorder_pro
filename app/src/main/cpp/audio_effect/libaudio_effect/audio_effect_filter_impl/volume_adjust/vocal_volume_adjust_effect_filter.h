#ifndef AUDIOEFFECT_VOCAL_VOLUME_ADJUST_FILTER_H_
#define AUDIOEFFECT_VOCAL_VOLUME_ADJUST_FILTER_H_

#include "./volume_adjust_effect_filter.h"

class VocalVolumeAdjustEffectFilter: public VolumeAdjustEffectFilter{
public:
	VocalVolumeAdjustEffectFilter();
	~VocalVolumeAdjustEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
};

#endif /* AUDIOEFFECT_VOCAL_VOLUME_ADJUST_FILTER_H_ */
