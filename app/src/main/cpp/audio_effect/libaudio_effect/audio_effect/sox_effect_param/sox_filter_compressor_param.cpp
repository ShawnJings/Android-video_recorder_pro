#include "./sox_filter_compressor_param.h"

#define LOG_TAG "SOXCompressorFilterParam"

SOXCompressorFilterParam::SOXCompressorFilterParam() {
	this->valid = true;
}

SOXCompressorFilterParam::SOXCompressorFilterParam(float attack, float release, int soft_knee_db, int input, int input1, int output1, int input2, int output2, int input3,
		int output3, int input4, int output4, int input5, int output5, int input6, int output6, int gain, int initial_volume, float delay) {
	this->attack = attack;
	this->release = release;
	this->gain = gain;
	this->initial_volume = initial_volume;
	this->delay = delay;
	this->soft_knee_db = soft_knee_db;
	this->input = input;
	this->input1 = input1;
	this->output1 = output1;
	this->input2 = input2;
	this->output2 = output2;
	this->input3 = input3;
	this->output3 = output3;
	this->input4 = input4;
	this->output4 = output4;
	this->input5 = input5;
	this->output5 = output5;
	this->input6 = input6;
	this->output6 = output6;
	this->valid = true;
}

void SOXCompressorFilterParam::init(int input1, int output1, int input2, int output2, int input3, int output3, int input4, int output4, int input5, int output5, int input6, int output6) {
	this->attack = 0.002;
	this->release = 0.03;
	this->soft_knee_db = 3;
	this->gain = 0;
	this->initial_volume = -100;
	this->delay = 0.0;
	this->input = -100;
	this->input1 = input1;
	this->output1 = output1;
	this->input2 = input2;
	this->output2 = output2;
	this->input3 = input3;
	this->output3 = output3;
	this->input4 = input4;
	this->output4 = output4;
	this->input5 = input5;
	this->output5 = output5;
	this->input6 = input6;
	this->output6 = output6;
}

void SOXCompressorFilterParam::setAttackTime(float attack){
	this->attack = attack;
}

void SOXCompressorFilterParam::setDecayTime(float release){
	this->release = release;
}

void SOXCompressorFilterParam::setGain(int gain){
	this->gain = gain;
}

bool SOXCompressorFilterParam::isValid() {
	return this->valid;
}

void SOXCompressorFilterParam::setValid(bool valid) {
	this->valid = valid;
}

SOXCompressorFilterParam::~SOXCompressorFilterParam() {
}


void SOXCompressorFilterParam::getAttackRelease(char** attackRelease){
	sprintf(*attackRelease, "%g,%g", this->attack, this->release);
}
void SOXCompressorFilterParam::getFunctionTransforTable(char** functionTransforTable){
	sprintf(*functionTransforTable, "%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", soft_knee_db, input, input1, output1, input2, output2, input3, output3, input4, output4, input5,
				output5, input6, output6);
}
void SOXCompressorFilterParam::getGain(char** gain){
	sprintf(*gain, "%d", this->gain);
}
void SOXCompressorFilterParam::getInitialVolume(char** initialVolume){
	sprintf(*initialVolume, "%d", this->initial_volume);
}
void SOXCompressorFilterParam::getDelay(char** delay){
	sprintf(*delay, "%g", this->delay);
}
