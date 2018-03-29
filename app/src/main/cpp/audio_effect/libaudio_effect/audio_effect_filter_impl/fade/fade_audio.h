#ifndef CB_FADE_AUDIO_H
#define CB_FADE_AUDIO_H

#include <math.h>

#define FADE_IN_TYPE			0
#define FADE_OUT_TYPE		1

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

template <typename SampleIntType>
class FadeAudio
{

public:
	/* 
	 * @param[in] type 0: fade in; 1: fade out
	 * @param[in] range sample number to fade
	 */
	FadeAudio(int type, int range); 

	void Init();
	
	/*
	 * @param[in] buf input sample buffer
	 * @param[in] isamp input sample number
	 */
	size_t FadeFlow(SampleIntType * buf, size_t isamp);

private: 
	int mIndex;
	int mType;
	int mRange;
};

template <typename SampleIntType>
FadeAudio<SampleIntType>::FadeAudio(int type, int range)
	:mIndex(0)
{
	mType = type;
	mRange = range;
}

template <typename SampleIntType>
void FadeAudio<SampleIntType>::Init()
{
	mIndex = 0;
}

template <typename SampleIntType>
size_t FadeAudio<SampleIntType>::FadeFlow(SampleIntType * buf, size_t isamp)
{
	if (buf == NULL)
	{
		return 0;
	}
	size_t i;
	double findex;
	double rate;
	for (i = 0; (i < isamp) && (mIndex < mRange); i++, mIndex++)
	{
		findex = mIndex;
		findex = max(0.0, min(1.0, findex / mRange));
		if (!mType)
		{
			findex = 1 - mType + findex;
		}
		/* quarter of sinewave */
		rate = 1 - sin(findex * M_PI / 2);
		buf[i] = buf[i] * rate;	
	}
	for (; i< isamp; i++)
	{
		buf[i] = (1 - mType) * buf[i];
	}

	return isamp;
}

#undef max
#undef min

#endif

