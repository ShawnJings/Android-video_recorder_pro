#ifndef AUDIOEFFECT_AGC_STAT_FILTER_H_
#define AUDIOEFFECT_AGC_STAT_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./agc_stat.h"

class AGCStatEffectFilter: public AudioEffectFilter{
protected:
	AGCStat<short>* audioAGCStat;

	void process(short* samples, int sampleSize) {
		if (NULL != audioAGCStat) {
			audioAGCStat->StatFlow(samples, sampleSize);
		}
	}
public:
	AGCStatEffectFilter();
	~AGCStatEffectFilter();

	virtual int init(AudioEffect* audioEffect) = 0;
	virtual void doFilter(AudioRequest* request, AudioResponse* response) = 0;
	virtual void destroy(AudioResponse* response) = 0;
};

#endif /* AUDIOEFFECT_AGC_STAT_FILTER_H_ */
