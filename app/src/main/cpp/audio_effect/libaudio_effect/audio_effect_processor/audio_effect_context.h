#ifndef AUDIOEFFECT_AUDIO_EFFECT_CONTEXT_H_
#define AUDIOEFFECT_AUDIO_EFFECT_CONTEXT_H_

#include "CommonTools.h"
#include "../audio_effect_filter_impl/sox_effect/sox.h"
#include <map>
#include <string>

class AudioEffectContext {
private:
	std::map<std::string, void*>* responseData;
	AudioEffectContext();

	static AudioEffectContext* instance;
public:
	~AudioEffectContext();
	static AudioEffectContext* GetInstance();

	void initResource();
	void destroyResource();
	void copyData(std::map<std::string, void*>* src) {
		if (NULL != src) {
			std::map<std::string, void*>::iterator itr = src->begin();
			for (; itr != src->end(); ++itr) {
				std::string key = itr->first;
				responseData->erase(key);
				responseData->insert(std::pair<std::string, void*>(key, itr->second));
			}
		}
	}

	void deleteResponseDataForKey(std::string key) {
		responseData->erase(key);
	}

	void* getResponseDataForKey(std::string key) {
		std::map<std::string, void*>::iterator itr = responseData->find(key);
		if (itr != responseData->end()) {
			return itr->second;
		}
		return NULL;
	}
};

#endif /* AUDIOEFFECT_AUDIO_EFFECT_CONTEXT_H_ */
