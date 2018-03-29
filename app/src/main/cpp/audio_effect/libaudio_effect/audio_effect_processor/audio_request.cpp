#include "./audio_request.h"

#define LOG_TAG "AudioRequest"

AudioRequest::AudioRequest() {
	extraData = new std::map<std::string, void*>();
}
AudioRequest::~AudioRequest() {
}
