#ifndef SOX_EQUALIZER_EFFECT_H_
#define SOX_EQUALIZER_EFFECT_H_
#include "./sox_base_effect.h"

class SoxEqualizerEffect: public SoxBaseEffect {
public:
	static short* preProcessSamples;
	static int size;
	static short* responseData;

protected:
	sox_effects_chain_t * equalizerChain;
	void initEqualizerChain(std::list<SOXEqualizerFilterParam*>* equalizerFilterParams);

public:
	SoxEqualizerEffect(double sampleRate, int channels);
	~SoxEqualizerEffect();
	/** 初始化全局变量 **/
	virtual int init(int bufferSize);
	/** 销毁 **/
	virtual void destroy();
	/** init chain **/
	virtual int initChain(SOXFilterChainParam* filterChainParam);
	/** delete chain **/
	virtual void destroyChain();
	/** 由边录边和调用的方法 **/
	virtual int process(short* audioSample, int size);
};
#endif /* SOX_EQUALIZER_EFFECT_H_ */
