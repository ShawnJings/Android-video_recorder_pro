#ifndef AUDIOEFFECT_FADE_OUT_FILTER_H_
#define AUDIOEFFECT_FADE_OUT_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./fade_audio.h"

#define FADE_OUT_SECONDS 3
#define MIN_SECONDS_WITH_FADE_OUT 10

class FadeOutEffectFilter: public AudioEffectFilter{
private:
	int recordedTimeMills;
	FadeAudio<short>* fadeAudioEffect;

	int latestDiffTimeMills;
	int totalTimeMills;
public:
	FadeOutEffectFilter();
	~FadeOutEffectFilter();

	virtual int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	virtual void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_FADE_OUT_FILTER_H_ */
