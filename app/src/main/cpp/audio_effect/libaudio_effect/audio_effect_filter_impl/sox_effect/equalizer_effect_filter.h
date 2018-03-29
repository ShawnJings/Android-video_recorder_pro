#ifndef AUDIOEFFECT_EQUALIZER_FILTER_H_
#define AUDIOEFFECT_EQUALIZER_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./sox_equalizer_effect.h"

class EqualizerEffectFilter: public AudioEffectFilter {
private:
	SoxEqualizerEffect* equalizerEffect;
	bool isInitialized;
	AudioEffect* audioEffect;

public:
	EqualizerEffectFilter();
	~EqualizerEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_EQUALIZER_FILTER_H_ */
