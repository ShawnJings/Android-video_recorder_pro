#ifndef AUDIOEFFECT_ACCOMPANY_AGC_STAT_FILTER_H_
#define AUDIOEFFECT_ACCOMPANY_AGC_STAT_FILTER_H_

#include "./agc_stat_effect_filter.h"

#define ACCOMPANY_AGC_STAT_GAIN 				"accompany_agc_stat_gain"

class AccompanyAGCStatEffectFilter: public AGCStatEffectFilter{
public:
	AccompanyAGCStatEffectFilter();
	~AccompanyAGCStatEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_ACCOMPANY_AGC_STAT_FILTER_H_ */
