#ifndef AUDIOEFFECT_COMPRESSOR_FILTER_H_
#define AUDIOEFFECT_COMPRESSOR_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./sox_compressor_effect.h"

class CompressorEffectFilter:public AudioEffectFilter {
private:
	SoxCompressorEffect* compressorEffect;
	bool isInitialized;
	AudioEffect* audioEffect;

public:
	CompressorEffectFilter();
	~CompressorEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_COMPRESSOR_FILTER_H_ */
