#ifndef BASE_COMPRESSOR_PARAM_BUILDER_H_
#define BASE_COMPRESSOR_PARAM_BUILDER_H_

#include "./sox_filter_compressor_param.h"
#include "CommonTools.h"

#define POINT_COUNT 6
/*
 * 单例设计，在录制过程中（PacketConsumer），算出分贝的分布，在完成录音界面（RealtimeEcho），计算出压缩效果器参数
 */
class CompressorParamBuilder {
private:
	long* samplesDistribution;
	CompressorParamBuilder();
	static CompressorParamBuilder* instance;

public:
	static CompressorParamBuilder* GetInstance();
	virtual ~CompressorParamBuilder();

	void clearSamplesDistribution();
	void statisticSampleDistribution(short* samples, int audioSampleSize);
	SOXCompressorFilterParam* buildCompressorFilterParam();
	SOXCompressorFilterParam* buildCompressorFilterParamWithFactor(float attackTime, float decayTime, int threshold, float ratio, int expansion_threshold, float expansion_ratio, int gain);
};

#endif /* BASE_COMPRESSOR_PARAM_BUILDER_H_ */
