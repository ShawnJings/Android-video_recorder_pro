#ifndef AUDIOEFFECT_ADJUST_ECHO_REVERB_FILTER_H_
#define AUDIOEFFECT_ADJUST_ECHO_REVERB_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./sox_reverb_effect.h"
#include "../../audio_effect_impl/adjust_echo_reverb/adjust_echo_reverb_audio_effect.h"

class AdjustEchoReverbEffectFilter: public AudioEffectFilter {
private:
	SoxReverbEffect* reverbEffect;
	bool isInitialized;
	AdjustEchoReverbAudioEffect* audioEffect;

public:
	AdjustEchoReverbEffectFilter();
	~AdjustEchoReverbEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_ADJUST_ECHO_REVERB_FILTER_H_ */
