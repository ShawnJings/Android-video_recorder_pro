#ifndef BASE_AUDIOEFFECT_SOX_FILTER_H_
#define BASE_AUDIOEFFECT_SOX_FILTER_H_

#include "./compressor_param_builder.h"
#include "./sox_filter_compressor_param.h"
#include "./sox_filter_equalizer_param.h"
#include "./sox_filter_reverb_param.h"
#include "CommonTools.h"
#include <list>

using namespace std;

class SOXFilterChainParam {

private:
	/** volume参数 **/
	int volume;
	/** compressor **/
	SOXCompressorFilterParam* compressorFilterParam;
	/** equalizer **/
	std::list<SOXEqualizerFilterParam*> *equalizerFilterParams;
	/** reverb参数 **/
	SOXReverbFilterParam* reverbFilterParam;
	/** 混响所占比例 **/
	float reverbRatio;

public:
	SOXFilterChainParam();
	SOXFilterChainParam(int volume, SOXCompressorFilterParam* compressorFilterParam, std::list<SOXEqualizerFilterParam*> *equalizerFilterParams, SOXReverbFilterParam* reverbFilterParam, float reverbRatio);
	virtual ~SOXFilterChainParam();
	static SOXFilterChainParam* buildDefaultParam();
	static SOXFilterChainParam* buildRecordingParam();

	void setCompressorFilterParam(float attackTime, float decayTime, int threshold, float ratio, int expansion_threshold, float expansion_ratio, int gain);
	void setEqualizerFilterParam(bool enable1, int frequency1, float bandWidth1, float gain1,
			bool enable2, int frequency2, float bandWidth2, float gain2,
			bool enable3, int frequency3, float bandWidth3, float gain3,
			bool enable4, int frequency4, float bandWidth4, float gain4,
			bool lowpassEnabled, int lowpassFrequency, float lowpassBandWidth,
			bool highpassEnabled, int highpassFrequency, float highpassBandWidth);
	void setReverbFilterParam(int reverberance, int hf_damping, int roomScale, int preDelay, int wetGain, int stereoDepth,
			int echoDelay, int echoPredecay, int echoPeriod, int echoDecayRatio, int echoPhaseDiff, int echoNum);

	SOXCompressorFilterParam* getCompressorFilterParam();
	std::list<SOXEqualizerFilterParam*>* getEqualizerFilterParam();
	SOXReverbFilterParam* getReverbFilterParam();
	char* getVolume();
	float getReverbRatio();
	void update(int echoLevel);
	bool equals(SOXFilterChainParam* filterChainParam);

	void setCompressorFilterParam(SOXCompressorFilterParam* compressorFilterParam){
		this->compressorFilterParam = compressorFilterParam;
	};
	void setReverbFilterParam(SOXReverbFilterParam* reverbFilterParam){
		this->reverbFilterParam = reverbFilterParam;
	};
	void setReverbRatio(float reverbRatio){
		this->reverbRatio = reverbRatio;
	};
};


#endif /* BASE_AUDIOEFFECT_SOX_FILTER_H_ */
