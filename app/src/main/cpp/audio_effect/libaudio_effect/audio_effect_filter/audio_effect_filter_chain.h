#ifndef AUDIOEFFECT_FILTER_CHAIN_H_
#define AUDIOEFFECT_FILTER_CHAIN_H_

#include "../audio_effect/audio_effect.h"
#include "../audio_effect_filter/audio_effect_filter_factory.h"
#include "./../libcommon/CommonTools.h"
#include <list>

class AudioEffectFilterChain {
private:
	std::list<AudioEffectFilter*> * filters;

public:
	AudioEffectFilterChain();
	~AudioEffectFilterChain();

	int init(AudioEffect* audioEffect, std::list<int>* effectFilters);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_FILTER_CHAIN_H_ */
