#ifndef AUDIOEFFECT_AUDIO_RESPONSE_H_
#define AUDIOEFFECT_AUDIO_RESPONSE_H_

#include "CommonTools.h"
#include <map>
#include <string>

#define PITCH_SHIFT_MIX_RESPONSE_KEY_RECEIVE_SAMPLES_SIZE "pitch_shift_mix_response_key_receive_samples_size"
#define AUDIORESPONSE_KEY_RECEIVESAMPLES_SIZE "audioresponse_key_receivesamples_size"
#define ACCOMPANYRESPONSE_KEY_RECEIVESAMPLES_SIZE "accompanyresponse_key_receivesamples_size"
#define ACCOMPANYRESPONSE_KEY_PITCHSHIFT_UNPROCESS_SIZE "accompanyresponse_key_pitchshift_unprocess_size"

class AudioResponse {
private:
	std::map<std::string, void*>* extraData;

public:
	AudioResponse();
	~AudioResponse();

	void clear() {
//		std::map<std::string, void*>::iterator itr = extraData->begin();
//		for (; itr != extraData->end(); ++itr) {
//			void* value = itr->second;
//			delete value;
//		}

		extraData->clear();
	}

	std::map<std::string, void*>* getData() {
		return extraData;
	}

	void put(std::string key, void* value) {
		extraData->insert(std::pair<std::string, void*>(key, value));
	}

	void deleteKey(std::string key) {
		extraData->erase(key);
	}

	void* get(std::string key) {
		std::map<std::string, void*>::iterator itr = extraData->find(key);
		if (itr != extraData->end()) {
			return itr->second;
		}
		return NULL;
	}
};

#endif /* AUDIOEFFECT_AUDIO_RESPONSE_H_ */
