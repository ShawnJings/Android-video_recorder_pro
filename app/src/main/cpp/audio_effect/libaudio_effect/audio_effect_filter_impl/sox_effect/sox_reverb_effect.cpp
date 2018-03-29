#include "./sox_reverb_effect.h"

#define LOG_TAG "SoxReverbEffect"

SoxReverbEffect::SoxReverbEffect(double sampleRate, int channels) :
		SoxBaseEffect(sampleRate, channels) {
	reverbChain = NULL;
}

SoxReverbEffect::~SoxReverbEffect() {
}

short* SoxReverbEffect::preProcessSamples = NULL;
int SoxReverbEffect::size = 0;
short* SoxReverbEffect::responseData = NULL;

/** 初始化全局变量 **/
int SoxReverbEffect::init(int bufferSize) {
	responseData = new short[bufferSize * 2];
	return SoxBaseEffect::init(bufferSize);
}

/** 销毁 **/
void SoxReverbEffect::destroy() {
	if (NULL != responseData) {
		delete[] responseData;
		responseData = NULL;
	}
}

int SoxReverbEffect::initChain(SOXFilterChainParam* filterChainParam) {
	SoxBaseEffect::initSignal();
	SoxReverbEffect::initReverbOutputSignal(2);
	this->initReverbChain(filterChainParam->getReverbFilterParam());
	return 1;
}

void SoxReverbEffect::initReverbOutputSignal(int channels) {
	reverb_out_signal.precision = 16;
	reverb_out_signal.channels = channels;
	reverb_out_signal.rate = inputSampleRate;
	reverb_out_signal.length = inputSampleRate * channels * 15 * 60;
	reverb_out_signal.mult = NULL;
}

int SoxReverbEffect::process(short* audioSamples, int sizeParam) {
	size = sizeParam;
	preProcessSamples = audioSamples;
	drain_cnt_down = 1;
	flow_frequency = 0;
	sox_flow_effects(reverbChain, NULL, NULL);
	int bytesPerShort = 2;
	int monoDrainStereoFlow = 2;
	memcpy(audioSamples, responseData, size * bytesPerShort * monoDrainStereoFlow);
	return size;
}

void SoxReverbEffect::destroyChain() {
	if (NULL != reverbChain) {
		sox_delete_effects_chain(reverbChain);
	}
}

void SoxReverbEffect::initReverbChain(SOXReverbFilterParam* reverbFilterParam) {
	//-w表示只要混响，不要干声
//	LOGI("signal channels is %d reverb_output_signles channels is %d",  signal.channels, reverb_out_signal.channels);
	reverbChain = sox_create_effects_chain(&encoding, &encoding);
	sox_effect_t * e = sox_create_effect(reverb_input_handler());
	assert(sox_add_effect(reverbChain, e, &signal, &reverb_out_signal) == SOX_SUCCESS);
	free(e);
//	LOGI("signal channels is %d reverb_output_signles channels is %d",  signal.channels, reverb_out_signal.channels);

	if (NULL != reverbFilterParam) {
		int reverbArgs = 13;
		char* wetOnly = new char[10];
		reverbFilterParam->getWetOnly(&wetOnly);
		char* reverbrance = new char[10];
		reverbFilterParam->getReverbrance(&reverbrance);
		char* damping = new char[10];
		reverbFilterParam->getDamping(&damping);
		char* roomScale = new char[10];
		reverbFilterParam->getRoomScale(&roomScale);
		char* stereoDepth = new char[10];
		reverbFilterParam->getStereoDepth(&stereoDepth);
		char* preDelay = new char[10];
		reverbFilterParam->getPreDelay(&preDelay);
		char* wetGain = new char[10];
		reverbFilterParam->getWetGain(&wetGain);
		char* echoDelay = new char[10];
		reverbFilterParam->getEchoDelay(&echoDelay);
		char* echoPredecay = new char[10];
		reverbFilterParam->getEchoPredecay(&echoPredecay);
		char* echoPeriod = new char[10];
		reverbFilterParam->getEchoPeriod(&echoPeriod);
		char* echoDecayRatio = new char[10];
		reverbFilterParam->getEchoDecayRatio(&echoDecayRatio);
		char* echoPhaseDiff = new char[10];
		reverbFilterParam->getEchoPhaseDiff(&echoPhaseDiff);
		char* echoNum = new char[10];
		reverbFilterParam->getEchoNum(&echoNum);
		char * reverbArgv[] = { wetOnly, reverbrance, damping, roomScale, stereoDepth, preDelay, wetGain, echoDelay, echoPredecay, echoPeriod, echoDecayRatio, echoPhaseDiff,
				echoNum };
		LOGI("reverbArgv is %s %s %s %s %s %s %s \n", reverbArgv[0], reverbArgv[1], reverbArgv[2], reverbArgv[3], reverbArgv[4], reverbArgv[5], reverbArgv[6]);
		LOGI("echoArgv is delay:%s predecay:%s period:%s decayRatio:%s phaseDiff:%s num:%s\n", reverbArgv[7], reverbArgv[8], reverbArgv[9], reverbArgv[10], reverbArgv[11],
				reverbArgv[12]);
		e = sox_create_effect(sox_find_effect("reverb"));
		assert(sox_effect_options(e, reverbArgs, reverbArgv) == SOX_SUCCESS);
		assert(sox_add_effect(reverbChain, e, &signal, &reverb_out_signal) == SOX_SUCCESS);
		free(e);
		for (int i = 0; i < reverbArgs; i++) {
			delete[] reverbArgv[i];
		}
	}

//	LOGI("signal channels is %d reverb_output_signles channels is %d",  signal.channels, reverb_out_signal.channels);
	e = sox_create_effect(reverb_output_handler());
	assert(sox_add_effect(reverbChain, e, &signal, &reverb_out_signal) == SOX_SUCCESS);
	free(e);
}
