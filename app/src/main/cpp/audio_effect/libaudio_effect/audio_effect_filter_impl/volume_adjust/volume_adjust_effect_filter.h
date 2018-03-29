#ifndef AUDIOEFFECT_VOLUME_ADJUST_FILTER_H_
#define AUDIOEFFECT_VOLUME_ADJUST_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"

class VolumeAdjustEffectFilter: public AudioEffectFilter{
protected:
	float volumeFactor;

	void process(short* samples, int sampleSize){
		adjustSamplesVolume(samples, sampleSize, this->volumeFactor);
	}
public:
	virtual int init(AudioEffect* audioEffect) = 0;
	virtual void doFilter(AudioRequest* request, AudioResponse* response) = 0;
	void destroy(AudioResponse* response){};
};

#endif /* AUDIOEFFECT_VOLUME_ADJUST_FILTER_H_ */
