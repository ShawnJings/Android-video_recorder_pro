#include "./compressor_param_builder.h"

#define LOG_TAG "CompressorParamBuilder"

CompressorParamBuilder::CompressorParamBuilder() {
	samplesDistribution = new long[POINT_COUNT * 2];
	for (int i = 0; i < POINT_COUNT * 2; i++) {
		samplesDistribution[i] = 0;
	}
}

//初始化静态成员
CompressorParamBuilder* CompressorParamBuilder::instance = new CompressorParamBuilder();
CompressorParamBuilder* CompressorParamBuilder::GetInstance() {
	return instance;
}

CompressorParamBuilder::~CompressorParamBuilder() {
	if (NULL != samplesDistribution) {
		delete[] samplesDistribution;
		samplesDistribution = NULL;
	}
}

static short Rank_dB[12] = { -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -6, 0 };

static short Rank_Amp[12] = { 58, 104, 184, 328, 583, 1036, 1843, 3277, 5827, 10362, 16423, 32767 };

void CompressorParamBuilder::statisticSampleDistribution(short* samples, int audioSampleSize) {
	for (int i = 0; i < audioSampleSize; i++) {
		for (int k = 1; k < POINT_COUNT * 2; k++) {
			if ((samples[i] >= Rank_Amp[k - 1]) && (samples[i] < Rank_Amp[k])) {
				samplesDistribution[k]++;
			}
		}
	}
}

void CompressorParamBuilder::clearSamplesDistribution() {
	for (int i = 0; i < POINT_COUNT * 2; i++) {
		samplesDistribution[i] = 0;
	}
}

SOXCompressorFilterParam* CompressorParamBuilder::buildCompressorFilterParamWithFactor(float attackTime, float decayTime, int threshold, float ratio, int expansion_threshold,
		float expansion_ratio, int gain) {
	SOXCompressorFilterParam* result = new SOXCompressorFilterParam();
//	int input1 = -80;
//	int output1 = -100;
//	int input2 = -30;
//	int output2 = -27;
//	int input3 = -25;
//	int output3 = -20;
//	int input4 = -10;
//	int output4 = -12;
//	int input5 = -5;
//	int output5 = -8;
//	int input6 = 0;
//	int output6 = -6;

	//7.0 gt 修改参数
	int input1 = -75;
	int output1 = -100;
	int input2 = -20;
	int output2 = -20;
	int input3 = -15;
	int output3 = -15;
	int input4 = -10;
	int output4 = -11.3;
	int input5 = -6;
	int output5 = -9.2;
	int input6 = 0;
	int output6 = -6.7;

	if (input1 < input2 && input2 < input3 && input3 < input4 && input4 < input5) {
		result->init(input1, output1, input2, output2, input3, output3, input4, output4, input5, output5, input6, output6);
		result->setAttackTime(attackTime);
		result->setDecayTime(decayTime);
		result->setGain(gain);
	} else {
		result->setValid(false);
	}
	return result;
}

//SOXCompressorFilterParam* CompressorParamBuilder::buildCompressorFilterParamWithFactor(float attackTime, float decayTime, int threshold, float ratio, int expansion_threshold,
//		float expansion_ratio, int gain) {
//	SOXCompressorFilterParam* result = new SOXCompressorFilterParam();
//	//根据 threshold ratio 计算出 indb 后半部分的点
//	//根据 expansion_threshold expansion_ratio 计算出 indb 前半部分的点
//	int expansion_index = 0;
//	int compressor_index = POINT_COUNT * 2 - 1;
//	for (int k = 0; k < POINT_COUNT * 2; k++) {
//		if(Rank_dB[k] >= expansion_threshold){
//			Rank_dB[k] = expansion_threshold;
//			expansion_index = k;
//		}
//		if(Rank_dB[k] >= threshold){
//			Rank_dB[k] >= threshold;
//			compressor_index = k;
//		}
//	}
//	if(compressor_index < expansion_index){
//		compressor_index = expansion_index;
//	}
//	int input[12];
//	int output[12];
//	for(int i = 0; i < expansion_index; i++){
//		input[i] = Rank_dB[i];
//		output[i] = input[i] / expansion_ratio;
//	}
//	for(int i = expansion_index; i <= compressor_index; i++){
//		input[i] = Rank_dB[i];
//		output[i] = input[i];
//	}
//	if(compressor_index < POINT_COUNT * 2 - 1){
//		for(int i = compressor_index + 1; i < POINT_COUNT * 2; i++){
//			input[i] = Rank_dB[i];
//			output[i] = input[i] / ratio;
//		}
//	}
//
//	int input1 = input[0];
//	int output1 = output[0];
//	int input2 = input[(short) (expansion_index / 2)];
//	int output2 = output[(short) (expansion_index / 2)];
//	int input3 = input[expansion_index];
//	int output3 = output[expansion_index];
//	int input4 = input[compressor_index];
//	int output4 = output[compressor_index];
//	int input5 = input[(short) ((12 + compressor_index) / 2)];
//	int output5 = output[(short) ((12 + compressor_index) / 2)];
//	int input6 = input[11];
//	int output6 = output[11];
//
//	if (input1 < input2 && input2 < input3 && input3 < input4 && input4 < input5 && input5 < input6) {
//		result->init(input1, output1, input2, output2, input3, output3, input4, output4, input5, output5, input6, output6);
//		result->setAttackTime(attackTime);
//		result->setDecayTime(decayTime);
//		result->setGain(gain);
//	} else {
//		result->setValid(false);
//	}
//	return result;
//}

SOXCompressorFilterParam* CompressorParamBuilder::buildCompressorFilterParam() {
	SOXCompressorFilterParam* result = new SOXCompressorFilterParam();
	short linePoint[12];
	linePoint[0] = -49;
	linePoint[11] = -6;
	for (int k = (POINT_COUNT * 2 - 1); k > 0; k--) {
		samplesDistribution[k] += samplesDistribution[k - 1];
	}
	short tmpRank = getIndexOfMaxValueInArray(samplesDistribution, POINT_COUNT * 2) - 1;
	if (tmpRank < 5 || tmpRank > 9) {
		tmpRank = 9;
	}
	if (true) {
		linePoint[tmpRank] = Rank_dB[tmpRank];
		if (linePoint[tmpRank] > -20) {
			linePoint[tmpRank] -= 6;
		}
		float b = -6.0;
		float a = ((float) (Rank_dB[tmpRank] - b)) / ((float) linePoint[tmpRank]);
		for (int k = 1; k < POINT_COUNT * 2; k++) {
			linePoint[k] = (short) ((float) Rank_dB[k] * a + b);
		}
		int input1 = Rank_dB[0];
		int output1 = Rank_dB[0];
		int input2 = Rank_dB[1];
		int output2 = Rank_dB[1];
		int input3 = Rank_dB[(short) ((2 + tmpRank) / 2)];
		int output3 = Rank_dB[(short) ((2 + tmpRank) / 2)];
		int input4 = Rank_dB[tmpRank];
		int output4 = linePoint[tmpRank];
		int input5 = Rank_dB[(short) ((12 + tmpRank) / 2)];
		int output5 = linePoint[(short) ((12 + tmpRank) / 2)];
		int input6 = Rank_dB[11];
		int output6 = linePoint[11];
		//Remark:安全性保证
		if (output3 + 6 <= output4) {
			output3 += 0;
		}
		if (input1 < input2 && input2 < input3 && input3 < input4 && input4 < input5) {
			result->init(input1, output1, input2, output2, input3, output3, input4, output4, input5, output5, input6, output6);
		} else {
			result->setValid(false);
		}
	} else {
		result->setValid(false);
	}
	return result;
}
