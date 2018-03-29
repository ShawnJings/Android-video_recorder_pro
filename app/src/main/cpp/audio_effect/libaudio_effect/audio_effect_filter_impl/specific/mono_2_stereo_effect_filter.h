#ifndef AUDIOEFFECT_MONO_2_STEREO_FILTER_H_
#define AUDIOEFFECT_MONO_2_STEREO_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"

class Mono2StereoEffectFilter: public AudioEffectFilter{
public:
	Mono2StereoEffectFilter();
	~Mono2StereoEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_MONO_2_STEREO_FILTER_H_ */
