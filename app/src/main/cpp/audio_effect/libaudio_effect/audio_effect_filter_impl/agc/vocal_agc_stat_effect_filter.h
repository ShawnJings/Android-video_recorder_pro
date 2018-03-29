#ifndef AUDIOEFFECT_VOCAL_AGC_STAT_FILTER_H_
#define AUDIOEFFECT_VOCAL_AGC_STAT_FILTER_H_

#include "./agc_stat_effect_filter.h"

#define VOCAL_AGC_STAT_GAIN 				"vocal_agc_stat_gain"

class VocalAGCStatEffectFilter: public AGCStatEffectFilter{
public:
	VocalAGCStatEffectFilter();
	~VocalAGCStatEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_VOCAL_AGC_STAT_FILTER_H_ */
