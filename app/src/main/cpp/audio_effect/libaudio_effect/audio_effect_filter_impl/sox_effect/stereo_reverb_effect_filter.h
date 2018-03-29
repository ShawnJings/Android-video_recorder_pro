#ifndef STEREO_AUDIOEFFECT_REVERB_FILTER_H_
#define STEREO_AUDIOEFFECT_REVERB_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./sox_reverb_effect.h"

class StereoReverbEffectFilter: public AudioEffectFilter {
private:
	SoxReverbEffect* reverbEffect;
	float reverbRatio;
	bool isInitialized;
	AudioEffect* audioEffect;

public:
	StereoReverbEffectFilter();
	~StereoReverbEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* STEREO_AUDIOEFFECT_REVERB_FILTER_H_ */
