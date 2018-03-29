#ifndef AUDIOEFFECT_PITCH_SHIFT_MIX_FILTER_H_
#define AUDIOEFFECT_PITCH_SHIFT_MIX_FILTER_H_

#include "./mix_effect_filter.h"

class PitchShiftMixEffectFilter: public MixEffectFilter {
private:
	short* accompanyBufferQueue;
	short* vocalBufferQueue;
	int vocalBufferCursor;
	int accompanyBufferCursor;
	int bufferQueueSize;

	int audioSampleRate;
	int channels;

public:
	PitchShiftMixEffectFilter();
	~PitchShiftMixEffectFilter();

	int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_PITCH_SHIFT_MIX_FILTER_H_ */
