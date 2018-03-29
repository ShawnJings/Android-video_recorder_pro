#include "./sox_filter_reverb_param.h"

#define LOG_TAG "SOXReverbFilterParam"

SOXReverbFilterParam::SOXReverbFilterParam() {
}

SOXReverbFilterParam::SOXReverbFilterParam(int reverberance, int damping, int roomScale, int stereoDepth, int preDelay, int wetGain,
		int echoDelay, int echoPredecay, int echoPeriod, int echoDecayRatio, int echoPhaseDiff, int echoNum) {
	this->reverberance = reverberance;
	this->damping = damping;
	this->roomScale = roomScale;
	this->stereoDepth = stereoDepth;
	this->preDelay = preDelay;
	this->wetGain = wetGain;
	/** echo **/
	if(echoDelay >=1 && echoDelay <= 400){
		this->echoDelay = echoDelay;
	} else{
		this->echoDelay = 20;
	}

	if(echoPredecay >=1 && echoPredecay <= 100){
		this->echoPredecay = echoPredecay;
	} else{
		this->echoPredecay = 15;
	}

	if(echoPeriod >= 1 && echoPeriod <= 400){
		this->echoPeriod = echoPeriod;
	} else{
		this->echoPeriod = 40;
	}
	if(echoDecayRatio >=14 && echoDecayRatio <= 90){
		this->echoDecayRatio = echoDecayRatio;
	} else{
		this->echoDecayRatio = 80;
	}
	if(echoPhaseDiff >=1 && echoPhaseDiff <= 100){
		this->echoPhaseDiff = echoPhaseDiff;
	} else{
		this->echoPhaseDiff = 20;
	}
	if(echoNum >=0 && echoNum <= 7){
		this->echoNum = echoNum;
	} else{
		this->echoNum = 2;
	}
}

SOXReverbFilterParam::~SOXReverbFilterParam() {

}

bool SOXReverbFilterParam::equals(SOXReverbFilterParam* reverbFilterParam){
	return reverberance == reverbFilterParam->getReverbranceIntegerType() &&
			damping == reverbFilterParam->getDampingIntegerType() &&
			roomScale == reverbFilterParam->getRoomScaleIntegerType() &&
			stereoDepth == reverbFilterParam->getStereoDepthIntegerType() &&
			preDelay == reverbFilterParam->getPreDelayIntegerType() &&
			wetGain == reverbFilterParam->getWetGainIntegerType() &&
			echoDelay == reverbFilterParam->getEchoDelayIntegerType() &&
			echoPredecay == reverbFilterParam->getEchoPredecayIntegerType() &&
			echoPeriod == reverbFilterParam->getEchoPeriodIntegerType() &&
			echoDecayRatio == reverbFilterParam->getEchoDecayRatioIntegerType() &&
			echoPhaseDiff == reverbFilterParam->getEchoPhaseDiffIntegerType() &&
			echoNum == reverbFilterParam->getEchoNumIntegerType();
}

void SOXReverbFilterParam::getWetOnly(char** wetOnly) {
	sprintf(*wetOnly, "%s", "-w");
}
void SOXReverbFilterParam::getReverbrance(char** reverbRance) {
	sprintf(*reverbRance, "%d", this->reverberance);
}
void SOXReverbFilterParam::getDamping(char** damping) {
	sprintf(*damping, "%d", this->damping);
}
void SOXReverbFilterParam::getRoomScale(char** roomScale) {
	sprintf(*roomScale, "%d", this->roomScale);
}
void SOXReverbFilterParam::getStereoDepth(char** stereoDepth) {
	sprintf(*stereoDepth, "%d", this->stereoDepth);
}
void SOXReverbFilterParam::getPreDelay(char** preDelay) {
	sprintf(*preDelay, "%d", this->preDelay);
}
void SOXReverbFilterParam::getWetGain(char** wetGain) {
	sprintf(*wetGain, "%d", this->wetGain);
}
/** echo 部分 **/
void SOXReverbFilterParam::getEchoDelay(char** echoDelay) {
	sprintf(*echoDelay, "%d", this->echoDelay);
}
void SOXReverbFilterParam::getEchoPredecay(char** echoPredecay) {
	sprintf(*echoPredecay, "%d", this->echoPredecay);
}
void SOXReverbFilterParam::getEchoPeriod(char** echoPeriod){
	sprintf(*echoPeriod, "%d", this->echoPeriod);
}
void SOXReverbFilterParam::getEchoDecayRatio(char** echoDecayRatio) {
	sprintf(*echoDecayRatio, "%d", this->echoDecayRatio);
}
void SOXReverbFilterParam::getEchoPhaseDiff(char** echoDecayRatio) {
	sprintf(*echoDecayRatio, "%d", this->echoPhaseDiff);
}
void SOXReverbFilterParam::getEchoNum(char** echoNum) {
	sprintf(*echoNum, "%d", this->echoNum);
}
