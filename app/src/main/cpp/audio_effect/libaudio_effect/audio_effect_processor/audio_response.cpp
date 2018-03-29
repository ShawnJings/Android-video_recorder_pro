#include "./audio_response.h"

#define LOG_TAG "AudioResponse"

AudioResponse::AudioResponse() {
	extraData = new std::map<std::string, void*>();
}
AudioResponse::~AudioResponse() {
}
