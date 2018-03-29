#ifndef BASE_AUDIOEFFECT_SOX_COMPRESSOR_FILTER_H_
#define BASE_AUDIOEFFECT_SOX_COMPRESSOR_FILTER_H_

#include "CommonTools.h"

class SOXCompressorFilterParam {
private:
	bool valid;
	//第一组参数
	float attack;
	float release;
	//第二组参数
	int soft_knee_db;
	int input;
	int input1;
	int output1;
	int input2;
	int output2;
	int input3;
	int output3;
	int input4;
	int output4;
	int input5;
	int output5;
	int input6;
	int output6;
	//第三组参数
	int gain;
	int initial_volume;
	float delay;

public:
	SOXCompressorFilterParam();
	SOXCompressorFilterParam(float attack, float release, int soft_knee_db,
			int input, int input1, int output1, int input2, int output2,
			int input3, int output3, int input4, int output4, int input5,
			int output5, int input6, int output6, int gain, int initial_volume, float delay);
	virtual ~SOXCompressorFilterParam();

	void init(int input1, int output1, int input2, int output2,
			int input3, int output3, int input4, int output4, int input5,
			int output5, int input6, int output6);

	void setAttackTime(float attack);
	void setDecayTime(float release);
	void setGain(int gain);

	void setValid(bool valid);
	bool isValid();

	void getAttackRelease(char** attackRelease);
	void getFunctionTransforTable(char** functionTransforTable);
	void getGain(char** gain);
	void getInitialVolume(char** initialVolume);
	void getDelay(char** delay);
	bool equals(SOXCompressorFilterParam* reverbFilterParam){
		return true;
	};
};

#endif /* BASE_AUDIOEFFECT_SOX_COMPRESSOR_FILTER_H_ */
