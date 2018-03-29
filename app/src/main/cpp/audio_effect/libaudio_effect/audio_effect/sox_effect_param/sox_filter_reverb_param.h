#ifndef BASE_AUDIOEFFECT_SOX_REVERB_FILTER_H_
#define BASE_AUDIOEFFECT_SOX_REVERB_FILTER_H_

#include "CommonTools.h"

class SOXReverbFilterParam {

private:
	int reverberance;
	int damping;
	int roomScale;
	int stereoDepth;
	int preDelay;
	int wetGain;

	int echoDelay;
	int echoPredecay;
	int echoPeriod;
	int echoDecayRatio;
	int echoPhaseDiff;
	int echoNum;
public:
	SOXReverbFilterParam();
	SOXReverbFilterParam(int reverberance, int damping, int roomScale, int stereoDepth, int preDelay, int wetGain,
			int echoDelay, int echoPredecay, int echoPeriod, int echoDecayRatio, int echoPhaseDiff, int echoNum);
	virtual ~SOXReverbFilterParam();

	void setStereoDepth(int stereoDepth){
		this->stereoDepth = stereoDepth;
	};
	bool equals(SOXReverbFilterParam* reverbFilterParam);
	void getWetOnly(char** wetOnly);
	void getReverbrance(char** reverbRance);
	int getReverbranceIntegerType(){
		return reverberance;
	};
	void getDamping(char** damping);
	int getDampingIntegerType(){
		return damping;
	};
	void getRoomScale(char** roomScale);
	int getRoomScaleIntegerType(){
		return roomScale;
	};
	void getStereoDepth(char** stereoDepth);
	int getStereoDepthIntegerType(){
		return stereoDepth;
	};
	void getPreDelay(char** preDelay);
	int getPreDelayIntegerType(){
		return preDelay;
	};
	void getWetGain(char** wetGain);
	int getWetGainIntegerType(){
		return wetGain;
	};
	void getEchoDelay(char** echoDelay);
	int getEchoDelayIntegerType(){
		return echoDelay;
	};
	void getEchoPredecay(char** echoPredecay);
	int getEchoPredecayIntegerType(){
		return echoPredecay;
	};
	void getEchoPeriod(char** echoPeriod);
	int getEchoPeriodIntegerType(){
		return echoPeriod;
	};
	void getEchoDecayRatio(char** echoDecayRatio);
	int getEchoDecayRatioIntegerType(){
		return echoDecayRatio;
	};
	void getEchoPhaseDiff(char** echoPhaseDiff);
	int getEchoPhaseDiffIntegerType(){
		return echoPhaseDiff;
	};
	void getEchoNum(char** echoNum);
	int getEchoNumIntegerType(){
		return echoNum;
	};
};

#endif /* BASE_AUDIOEFFECT_SOX_REVERB_FILTER_H_ */
