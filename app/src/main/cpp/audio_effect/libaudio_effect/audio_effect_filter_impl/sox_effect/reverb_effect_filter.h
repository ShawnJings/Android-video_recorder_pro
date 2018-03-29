#ifndef AUDIOEFFECT_REVERB_FILTER_H_
#define AUDIOEFFECT_REVERB_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./sox_reverb_effect.h"

class ReverbEffectFilter: public AudioEffectFilter {
private:
	SoxReverbEffect* reverbEffect;
	float reverbRatio;
	bool isInitialized;
	AudioEffect* audioEffect;

public:
	ReverbEffectFilter();
	~ReverbEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_REVERB_FILTER_H_ */
