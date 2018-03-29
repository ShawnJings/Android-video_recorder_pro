#include "./sox_filter_chain_param.h"

#define LOG_TAG "SOXFilterChainParam"

SOXFilterChainParam::SOXFilterChainParam() {
}

SOXFilterChainParam::SOXFilterChainParam(int volume, SOXCompressorFilterParam* compressorFilterParam, std::list<SOXEqualizerFilterParam*> *equalizerFilterParams,
		SOXReverbFilterParam* reverbFilterParam, float reverbRatio) {
	this->volume = volume;
	this->compressorFilterParam = compressorFilterParam;
	this->equalizerFilterParams = equalizerFilterParams;
	this->reverbFilterParam = reverbFilterParam;
	this->reverbRatio = reverbRatio;
}

SOXFilterChainParam::~SOXFilterChainParam() {
	if (NULL != compressorFilterParam){
		delete compressorFilterParam;
		compressorFilterParam = NULL;
	}

	if (NULL != equalizerFilterParams){
		for (std::list<SOXEqualizerFilterParam*>::iterator it = equalizerFilterParams->begin();
				it != equalizerFilterParams->end(); ++it){
			delete *it;
		}

		equalizerFilterParams->clear();
		delete equalizerFilterParams;
		equalizerFilterParams = NULL;
	}
	if (NULL != reverbFilterParam){
		delete reverbFilterParam;
		reverbFilterParam = NULL;
	}
}

void SOXFilterChainParam::update(int echoLevel) {
//	SOXCompressorFilterParam* compressorParam = CompressorParamBuilder::GetInstance()->buildCompressorFilterParam();
//	if (compressorParam->isValid()) {
//		LOGI("UPDATE compressor is %s", compressorParam->getFunctionTransforTable());
//		this->compressorFilterParam = compressorFilterParam;
//	}
//	switch (echoLevel) {
//	case 1:
//		reverbFilterParam = new SOXReverbFilterParam(70, 50, 62, 70, 43, 6);
//		reverbRatio = 0.19;
//		break;
//	case 2:
//		reverbFilterParam = new SOXReverbFilterParam(90, 20, 71, 80, 60, 6);
//		reverbRatio = 0.20;
//		break;
//	case 3:
//		reverbFilterParam = new SOXReverbFilterParam(92, 10, 80, 88, 80, 6);
//		reverbRatio = 0.30;
//		break;
//	}
}

SOXFilterChainParam* SOXFilterChainParam::buildRecordingParam() {
	int volume = 0;

	std::list<SOXEqualizerFilterParam*> * equalizerFilterParams = new std::list<SOXEqualizerFilterParam*>();
	SOXEqualizerFilterParam* equalizerFilterParam = new SOXEqualizerFilterParam(LOW_PASS, 16000, 0.75f);
	equalizerFilterParams->push_back(equalizerFilterParam);
	equalizerFilterParam = new SOXEqualizerFilterParam(HIGH_PASS, 100, 0.7f);
	equalizerFilterParams->push_back(equalizerFilterParam);

	SOXCompressorFilterParam* compressorFilterParam = NULL;

	SOXReverbFilterParam* reverbFilterParam = NULL;
	float reverbRatio = 0.20;

	SOXFilterChainParam* instance = new SOXFilterChainParam(volume, compressorFilterParam, equalizerFilterParams, reverbFilterParam, reverbRatio);
	return instance;
}

SOXFilterChainParam* SOXFilterChainParam::buildDefaultParam() {
	int volume = 0;

	std::list<SOXEqualizerFilterParam*> * equalizerFilterParams = new std::list<SOXEqualizerFilterParam*>();
	SOXEqualizerFilterParam* equalizerFilterParam = new SOXEqualizerFilterParam(1600, 2.0, 3);
	equalizerFilterParams->push_back(equalizerFilterParam);
	equalizerFilterParam = new SOXEqualizerFilterParam(5600, 0.2, 10);
	equalizerFilterParams->push_back(equalizerFilterParam);
	equalizerFilterParam = new SOXEqualizerFilterParam(17000, 0.1, -6);
	equalizerFilterParams->push_back(equalizerFilterParam);

	SOXCompressorFilterParam* compressorFilterParam = CompressorParamBuilder::GetInstance()->buildCompressorFilterParam();
	if (!compressorFilterParam->isValid()) {
//		compressorFilterParam = new SOXCompressorFilterParam(0.002, 0.03, 6, -70, -55, -49, -50, -40, -30, -20, -10, -10, 0, -6, 0, -90, 0.0);
		compressorFilterParam = new SOXCompressorFilterParam(0.002, 0.03, 6, -70, -80, -100, -30, -27, -25, -20, -10, -14, -5, -10, 0, -8, 0, -90, 0.0);
	}

	SOXReverbFilterParam* reverbFilterParam = new SOXReverbFilterParam(90, 20, 71, 80, 10, 6, 20, 15, 30, 80, 20, 2);
	float reverbRatio = 0.20;

	SOXFilterChainParam* instance = new SOXFilterChainParam(volume, compressorFilterParam, equalizerFilterParams, reverbFilterParam, reverbRatio);
	return instance;
}

void SOXFilterChainParam::setCompressorFilterParam(float attackTime, float decayTime, int threshold, float ratio, int expansion_threshold, float expansion_ratio, int gain) {
	this->compressorFilterParam = CompressorParamBuilder::GetInstance()->buildCompressorFilterParamWithFactor(attackTime, decayTime, threshold, ratio, expansion_threshold,
			expansion_ratio, gain);
}

void SOXFilterChainParam::setEqualizerFilterParam(bool enable1, int frequency1, float bandWidth1, float gain1, bool enable2, int frequency2, float bandWidth2, float gain2,
		bool enable3, int frequency3, float bandWidth3, float gain3, bool enable4, int frequency4, float bandWidth4, float gain4, bool lowpassEnabled, int lowpassFrequency,
		float lowpassBandWidth, bool highpassEnabled, int highpassFrequency, float highpassBandWidth) {
	this->equalizerFilterParams->clear();
	if (enable1) {
		this->equalizerFilterParams->push_back(new SOXEqualizerFilterParam(frequency1, bandWidth1, gain1));
	}
	if (enable2) {
		this->equalizerFilterParams->push_back(new SOXEqualizerFilterParam(frequency2, bandWidth2, gain2));
	}
	if (enable3) {
		this->equalizerFilterParams->push_back(new SOXEqualizerFilterParam(frequency3, bandWidth3, gain3));
	}
	if (enable4) {
		this->equalizerFilterParams->push_back(new SOXEqualizerFilterParam(frequency4, bandWidth4, gain4));
	}
	if (lowpassEnabled) {
		this->equalizerFilterParams->push_back(new SOXEqualizerFilterParam(LOW_PASS, lowpassFrequency, lowpassBandWidth));
	}
	if (highpassEnabled) {
		this->equalizerFilterParams->push_back(new SOXEqualizerFilterParam(HIGH_PASS, highpassFrequency, highpassBandWidth));
	}
}

void SOXFilterChainParam::setReverbFilterParam(int reverberance, int hf_damping, int roomScale, int preDelay, int wetGain, int stereoDepth,
		int echoDelay, int echoPredecay, int echoPeriod, int echoDecayRatio, int echoPhaseDiff, int echoNum) {
	this->reverbFilterParam = new SOXReverbFilterParam(reverberance, hf_damping, roomScale, stereoDepth, preDelay, wetGain, echoDelay, echoPredecay, echoPeriod, echoDecayRatio, echoPhaseDiff, echoNum);
}

SOXCompressorFilterParam* SOXFilterChainParam::getCompressorFilterParam() {
	return compressorFilterParam;
}
std::list<SOXEqualizerFilterParam*>* SOXFilterChainParam::getEqualizerFilterParam() {
	return equalizerFilterParams;
}
SOXReverbFilterParam* SOXFilterChainParam::getReverbFilterParam() {
	return reverbFilterParam;
}

bool SOXFilterChainParam::equals(SOXFilterChainParam* filterChainParam){
	LOGI("compressorFilterParam->equals");
	if(!compressorFilterParam->equals(filterChainParam->getCompressorFilterParam())){
		return false;
	}
	LOGI("reverbFilterParam->equals");
	if(!reverbFilterParam->equals(filterChainParam->getReverbFilterParam())){
		return false;
	}

	LOGI("target_equalizerFilterParams size ->equals");
	std::list<SOXEqualizerFilterParam*>* target_equalizerFilterParams = filterChainParam->getEqualizerFilterParam();
	if(target_equalizerFilterParams->size() != equalizerFilterParams->size()){
		return false;
	}
	LOGI("target_equalizerFilterParams content ->equals");
	std::list<SOXEqualizerFilterParam*>::iterator source_ite = equalizerFilterParams->begin();
	std::list<SOXEqualizerFilterParam*>::iterator target_ite = target_equalizerFilterParams->begin();
	for (; source_ite != equalizerFilterParams->end(); ++source_ite, ++target_ite) {
		SOXEqualizerFilterParam* source_equalizerFilterParam = *source_ite;
		SOXEqualizerFilterParam* target_equalizerFilterParam = *target_ite;
		LOGI("source_equalizerFilterParam is %x target_equalizerFilterParam is %x",  source_equalizerFilterParam, target_equalizerFilterParam);
		if(!source_equalizerFilterParam->equals(target_equalizerFilterParam)){
			return false;
		}
	}
	return true;
}

char* SOXFilterChainParam::getVolume() {
	char* volumedBOptions = new char[10];
	sprintf(volumedBOptions, "%ddB", volume);
	return volumedBOptions;
}

float SOXFilterChainParam::getReverbRatio() {
	return reverbRatio;
}
