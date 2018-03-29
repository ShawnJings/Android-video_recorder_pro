#include <math.h>
#include "adjust_echo_reverb_effect_filter.h"

#define LOG_TAG "AdjustEchoReverbEffectFilter"

AdjustEchoReverbEffectFilter::AdjustEchoReverbEffectFilter() {
}

AdjustEchoReverbEffectFilter::~AdjustEchoReverbEffectFilter() {
}

int AdjustEchoReverbEffectFilter::init(AudioEffect* audioEffect) {
	LOGI("enter AdjustEchoReverbEffectFilter::init() ....");
	isInitialized = false;
	this->audioEffect = (AdjustEchoReverbAudioEffect*)audioEffect;
	reverbEffect = new SoxReverbEffect(audioEffect->getAudioInfo()->getAudioSampleRate(), audioEffect->getAudioInfo()->getChannels());
	return 1;
}

void AdjustEchoReverbEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != reverbEffect) {
		short* audioBuffer = request->getVocalBuffer();
		int audioBufferSize = request->getVocalBufferSize();

		float echoRatio = audioEffect->getEchoRatio();
		float reverbRatio = audioEffect->getReverbRatio();
		//float dryWetMix = (float)reverbRatio*45 / 100;
		float dryWetMix = pow(10, (reverbRatio-1 ))/1.2;
		if(!isInitialized){
			reverbEffect->init(audioBufferSize / 2);
//			free reverb:
//			空间0 ~ 100 对应：
//			roomScale 10 ~ 95(同时被混响比例影响)
//			echoNum 1 ~ 4
//			echoDecayRatio 14 ~ 90
//			echoPredecay 30 ~ 60
//			preDelay 15 ~ 130
//			echoPeriod 15 ~ 220
//			echoDelay 5 ~ 110
//			混响 0 ~ 100:
//			dryWetMix 0 ~ 45
//			damping 20 ~ 10
//			reverberance 0 ~ 90
//			固定：
//			echoPhaseDiff 50
//			wetGain 3
//			stereoDepth 80

			SOXFilterChainParam* filterChainParam = audioEffect->getFilterChainParam();
//
			int reverberance = 50 + reverbRatio * (90-50);
			int damping = 20 + reverbRatio * (10-20);
			int roomScale = 40 + echoRatio * (20-0) + reverbRatio * (75 - 40);
			int preDelay = 15 + echoRatio * (90-15);
			int wetGain = 3;
			int stereoDepth = 80;
			int echoDelay = 5 + echoRatio * (110-5);
			int echoPredecay = 45 + echoRatio * (30-45);
			int echoPeriod = 15 + echoRatio * (220-15);
			int echoDecayRatio = 44 + echoRatio * (85-44);
			int echoPhaseDiff = 50;
			int echoNum = 1 + echoRatio * (4-1);

			filterChainParam->setReverbFilterParam(reverberance, damping, roomScale, preDelay,
					wetGain, stereoDepth, echoDelay, echoPredecay, echoPeriod, echoDecayRatio,
					echoPhaseDiff, echoNum);

			reverbEffect->initChain(audioEffect->getFilterChainParam());
			isInitialized = true;
		}
		short* directAudioBuffer = new short[audioBufferSize];
		for(int i = 0; i < audioBufferSize/2 ; i++){
			directAudioBuffer[2 * i] = audioBuffer[i];
			directAudioBuffer[2 * i + 1] = audioBuffer[i];
		}
		//混响处理
		reverbEffect->process(audioBuffer, audioBufferSize / 2);
		//4、合成直达声与混响参数
		float directAudioRatio = 1 - dryWetMix;
		for (int i = 0; i < audioBufferSize; i++) {
			audioBuffer[i] = directAudioBuffer[i] * directAudioRatio + audioBuffer[i] * dryWetMix;
		}
		if (NULL != directAudioBuffer) {
			delete[] directAudioBuffer;
		}
	}
}

void AdjustEchoReverbEffectFilter::destroy(AudioResponse* response) {
	if (NULL != reverbEffect) {
		reverbEffect->destroyChain();
		reverbEffect->destroy();
		delete reverbEffect;
		reverbEffect = NULL;
	}
}
