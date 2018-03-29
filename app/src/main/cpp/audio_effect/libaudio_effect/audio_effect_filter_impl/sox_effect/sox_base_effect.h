#ifndef SOX_BASE_EFFECT_H_
#define SOX_BASE_EFFECT_H_
#include "../../audio_effect/sox_effect_param/sox_filter_chain_param.h"
#include "CommonTools.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
extern "C" {
#include "./sox.h"
};
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>

#define LOG_TAG "SoxBaseEffect"

class SoxBaseEffect {
protected:
	//由于我们是处理标准的pcm，所以这里就写成pcm的signal与encoding
	void initGlobalParams(int bufferSize);

	int inputSampleRate;
	int inputChannels;
	sox_signalinfo_t signal;
	void initSignal();

	sox_encodinginfo_t encoding;
	void initEncoding();
public:
	SoxBaseEffect();
	SoxBaseEffect(double sampleRate, int channels);
	~SoxBaseEffect();
	/** 初始化全局变量 **/
	virtual int init(int bufferSize);
	/** 销毁 **/
	virtual void destroy() = 0;
	/** init chain **/
	virtual int initChain(SOXFilterChainParam* filterChainParam) = 0;
	/** delete chain **/
	virtual void destroyChain() = 0;
	/** 由边录边和调用的方法 **/
	virtual int process(short* audioSample, int size) = 0;
};
#endif /* SOX_BASE_EFFECT_H_ */
