#include "./audio_effect_context.h"

#define LOG_TAG "AudioEffectContext"

AudioEffectContext::AudioEffectContext() {
	responseData = new std::map<std::string, void*>();
}
AudioEffectContext::~AudioEffectContext() {
}
//初始化静态成员
AudioEffectContext* AudioEffectContext::instance = new AudioEffectContext();
AudioEffectContext* AudioEffectContext::GetInstance() {
	return instance;
}

void AudioEffectContext::initResource() {
	//note that: sox init类似于一个全局变量如果init了一次在调用sox_quit之前就不能再init第二次了
	LOGI("before sox_init...");
	sox_init();
	LOGI("after sox_init...");
}

void AudioEffectContext::destroyResource() {
	LOGI("before sox_quit...");
	sox_quit();
	LOGI("after sox_quit...");
}
