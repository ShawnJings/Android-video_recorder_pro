#ifndef AUDIOEFFECT_VOCAL_VOLUME_BALENCE_ADJUST_FILTER_H_
#define AUDIOEFFECT_VOCAL_VOLUME_BALENCE_ADJUST_FILTER_H_

#include "./volume_adjust_effect_filter.h"
#include "../../audio_effect_processor/audio_effect_context.h"

class VocalVolumeBalenceAdjustEffectFilter: public VolumeAdjustEffectFilter{

#define FLAG_IS_MERGE	"flag_is_merge" //标识当前是否在merge

public:
	VocalVolumeBalenceAdjustEffectFilter();
	~VocalVolumeBalenceAdjustEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);

private:
	float accompanyVolume;
	float vocalVolume;
	float outputGain;
	float accompanyVolumeMax;

};

#endif /* AUDIOEFFECT_VOCAL_VOLUME_BALENCE_ADJUST_FILTER_H_ */
