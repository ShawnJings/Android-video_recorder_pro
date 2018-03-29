#include "./audio_effect_filter_chain.h"

#define LOG_TAG "AudioEffectFilterChain"

AudioEffectFilterChain::AudioEffectFilterChain() {
	filters = NULL;
}

AudioEffectFilterChain::~AudioEffectFilterChain() {
}

int AudioEffectFilterChain::init(AudioEffect* audioEffect, std::list<int>* effectFilters) {
	LOGI("enter init ...");
	filters = new std::list<AudioEffectFilter*>();
	if (NULL != effectFilters && effectFilters->size() > 0) {
		std::list<int>::iterator ite = effectFilters->begin();
		for (; ite != effectFilters->end(); ++ite) {
			int effectFilterType = *ite;
			LOGI("init filter effectFilterType %d", effectFilterType);
			AudioEffectFilter* filter = AudioEffectFilterFactory::GetInstance()->buildFilter((EffectFilterType)effectFilterType);
			if (NULL != filter) {
				if (filter->init(audioEffect) > 0) {
					filters->push_back(filter);
				}
			}
		}
	}

	return 1;
}

void AudioEffectFilterChain::doFilter(AudioRequest* request, AudioResponse* response) {
	std::list<AudioEffectFilter*>::iterator ite = filters->begin();
	for (; ite != filters->end(); ++ite) {
		AudioEffectFilter* filter = *ite;
		filter->doFilter(request, response);
	}
}

void AudioEffectFilterChain::destroy(AudioResponse* response) {
	std::list<AudioEffectFilter*>::iterator ite = filters->begin();
	for (; ite != filters->end(); ++ite) {
		AudioEffectFilter* filter = *ite;
		filter->destroy(response);
		delete filter;
	}
	filters->clear();
	delete filters;
}
