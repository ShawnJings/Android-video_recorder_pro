#ifndef AUDIOEFFECT_MIX_FILTER_H_
#define AUDIOEFFECT_MIX_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "CommonTools.h"

class MixEffectFilter: public AudioEffectFilter {
public:
	MixEffectFilter();
	~MixEffectFilter();

	virtual int init(AudioEffect* audioEffect);
	virtual void doFilter(AudioRequest* request, AudioResponse* response);
	virtual void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_MIX_FILTER_H_ */
