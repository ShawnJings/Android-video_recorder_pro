#ifndef CB_AGC_STAT_H
#define CB_AGC_STAT_H

#include "CommonTools.h"
#define AGC_AUDIO_TYPE                0
#define AGC_ACCOMPANY_TYPE            1
#define AGC_AUDIO_VOLUME_RATE         0.9
#define AGC_ACCOMPANY_VOLUME_RATE     0.35

/** 
 * @brief 这个类用来计算AGC的音频振幅增益比
 * @tparam SampleIntType sample的类型，这里只支持整形类别，如short
 * @example 
 *		AGCStat<short>* a = new AGCStat<short>(0, 44100, 1, 0.7);
 *		FILE * fin = fopen("xxx.pcm", "r");
 *		short samples[2048];
 *		while(!feof(fin) )
 *		{
 *			size_t k = fread(samples, sizeof(short), 2048, fin);
 *			if (k == 2048)
 *			{
 *				a->StatFlow(samples, 2048);
 *			}
 *		}
 *		double gain = a->GetGain();
 *	
 *		cout<<gain<<endl;
 *		fclose(fin);
 *		delete a;
 */ 
template <typename SampleIntType>
class AGCStat
{
public:
	/**
	 * @param[in] type 0:人声 1: 伴奏
	 * @param[in] samplerate 输入音频的采样率
	 * @param[in] channel 输入音频的声道数
	 * @param[in] volumeRate (0.0f, 1.0f] 最大音量的音量比，0为静音，1为满载。
	 */
	AGCStat(int type, int samplerate, int channel, double volumeRate);
	~AGCStat();
	/**
	 * @brief 输入音频流
	 * @param[in] iBuf 音频sample buffer
	 * @param[in] isamp buffer中sample的个数
	 */
	void StatFlow(const SampleIntType * iBuf, size_t isamp);
	/**
	 * @brief 音频流结束后调用，计算音频应该放大多少倍
	 * @return 振幅增益比，浮点数
	 */
	double GetGain();
private:
	SampleIntType _FindSmoothMax(SampleIntType samp);
	SampleIntType _GetSampByGlobalDistCurve(SampleIntType samp);
	SampleIntType _GetSampByPartDistCurve(SampleIntType samp);

private:
	int mGlobalDistCurveCutIdx, mPartDistCurveCutIdx;
	int mGlobalDistCurveCutCnt, mPartDistCurveCutCnt;
	int mGlobalDistCurveCutLimit, mPartDistCurveCutLimit;
	int mPartDistCurveQueuesize, mPartDistCurveQueueIdx;
	int mIdx;

	int *mGlobalDistCurve;
	int *mPartDistCurve;

	SampleIntType *mPartDistCurveQueue;

	int mType; //0: voice; 1: music
	int mPartDistCurveCap;
	double mVolumeRate;

	SampleIntType mMinIntPerSample;
	SampleIntType mMaxSample;

	double mOrgEnvelope;
	double mSmoothEnvelope;
	int mSliceSampleIdx;
	int mSliceSamplNum;

};


#define MAX_VOLUME (1<<(sizeof(SampleIntType) * 8 -1))
#define MIN_VOLUME_RATE 0.0015f
#define BIT_DIFF (sizeof(SampleIntType) * 8 - 16)

template <typename SampleIntType>
AGCStat<SampleIntType>::AGCStat(int type, int samplerate, int channel, double volumeRate):
	mGlobalDistCurveCutIdx(0), mPartDistCurveCutIdx(0),
	mGlobalDistCurveCutCnt(0), mPartDistCurveCutCnt(0),
	mPartDistCurveQueuesize(0), mPartDistCurveQueueIdx(0),
	mIdx(0),
	mGlobalDistCurve(NULL), mPartDistCurve(NULL),
	mPartDistCurveQueue(NULL)


{
	type = AGC_AUDIO_TYPE;
	mType = type;
	mVolumeRate = volumeRate;
	mPartDistCurveCap = samplerate * channel;
	mMinIntPerSample = MIN_VOLUME_RATE * MAX_VOLUME;
	mMaxSample = mMinIntPerSample;
	mOrgEnvelope = mMinIntPerSample;
	mSmoothEnvelope = mMinIntPerSample;
	mSliceSampleIdx = 0;
	mSliceSamplNum = 44;

	mGlobalDistCurve = new int[32800];
	memset(mGlobalDistCurve, 0, sizeof(int) * 32800);
	mPartDistCurve = new int[32800];
	memset(mPartDistCurve, 0, sizeof(int) * 32800);
	mPartDistCurveCutLimit = mPartDistCurveCap / 100;
	mGlobalDistCurveCutLimit = mPartDistCurveCutLimit * 20 ;
	mPartDistCurveQueue = new SampleIntType[mPartDistCurveCap];
}

template <typename SampleIntType>
AGCStat<SampleIntType>::~AGCStat()
{
	if (mGlobalDistCurve)
	{
		delete[] mGlobalDistCurve;
		mGlobalDistCurve = NULL;
	}
	if (mPartDistCurve)
	{
		delete[] mPartDistCurve;
		mPartDistCurve = NULL;
	}
	if (mPartDistCurveQueue)
	{
		delete[] mPartDistCurveQueue;
		mPartDistCurveQueue = NULL;
	}
}


template <typename SampleIntType>
void AGCStat<SampleIntType>::StatFlow(const SampleIntType * ibuf, size_t isamp)
{
	SampleIntType samp;
	SampleIntType tsamp1, tsamp2, tsamp3;
	for (size_t i = 0; i < isamp; i++)
	{
		mIdx ++;
		samp = abs(ibuf[i]);
		if (samp < 0)
		{
			samp = 0;
		}
		if (mType == 0) //人声
		{
			tsamp1 = _FindSmoothMax(samp);
			tsamp2 = _GetSampByGlobalDistCurve(samp);
			tsamp3 = _GetSampByPartDistCurve(samp);
			samp = tsamp1;
			if (tsamp2 > samp)
			{
				samp = tsamp2;
			}
			if (tsamp3 > samp)
			{
				samp = tsamp3;
			}
		}
		if (mMaxSample < samp)
		{
			mMaxSample = samp;
		}

	}
}

template <typename SampleIntType>
double AGCStat<SampleIntType>::GetGain()
{
	long long maxVolume = MAX_VOLUME;
	maxVolume *= mVolumeRate;
	double gain = double(maxVolume) / (double)mMaxSample;

	if (gain > 10)
	{
		return -1;
	}

	return gain;
}

template <typename SampleIntType>
SampleIntType AGCStat<SampleIntType>::_FindSmoothMax(SampleIntType samp)
{
	mSliceSampleIdx ++;
	if (mOrgEnvelope < samp)
	{
		mOrgEnvelope = samp;
	}
	if (mSliceSampleIdx > mSliceSamplNum)
	{
		mSmoothEnvelope /= 0.8;
		if (mSmoothEnvelope > mOrgEnvelope)
		{
			mSmoothEnvelope = mOrgEnvelope;
		}
		mOrgEnvelope = mMinIntPerSample;
		mSliceSampleIdx = 0;
	}

	return mSmoothEnvelope;
}

template <typename SampleIntType>
SampleIntType AGCStat<SampleIntType>::_GetSampByGlobalDistCurve(SampleIntType samp)
{
	int sampIdx = samp;
	sampIdx = sampIdx >> BIT_DIFF;
	if (sampIdx >= mGlobalDistCurveCutIdx)
	{
		mGlobalDistCurve[sampIdx] ++;
		mGlobalDistCurveCutCnt ++;
		for (;mGlobalDistCurveCutCnt > mGlobalDistCurveCutLimit;
				mGlobalDistCurveCutCnt -= mGlobalDistCurve[mGlobalDistCurveCutIdx++]);
	}
	return mGlobalDistCurveCutIdx << BIT_DIFF;
}

template <typename SampleIntType>
SampleIntType AGCStat<SampleIntType>::_GetSampByPartDistCurve(SampleIntType samp)
{
	int sampIdx = samp;
	int popSampIdx;
	sampIdx = sampIdx >> BIT_DIFF;
	if (mPartDistCurveQueuesize == mPartDistCurveCap)
	{
		popSampIdx = mPartDistCurveQueue[mPartDistCurveQueueIdx];
		if (popSampIdx >= mPartDistCurveCutIdx)
		{
			mPartDistCurve[popSampIdx] --;
			mPartDistCurveCutCnt --;
		}
	}
	else
	{
		mPartDistCurveQueuesize ++;
	}
	mPartDistCurveQueue[mPartDistCurveQueueIdx++] = sampIdx;
	if (sampIdx >= mPartDistCurveCutIdx)
	{
		mPartDistCurve[sampIdx] ++;
		mPartDistCurveCutCnt ++;
		for (;mPartDistCurveCutCnt > mPartDistCurveCutLimit;
				mPartDistCurveCutCnt -= mPartDistCurve[mPartDistCurveCutIdx++]);
	}

	if (mPartDistCurveQueueIdx >= mPartDistCurveCap)
	{
		mPartDistCurveQueueIdx -= mPartDistCurveCap;
	}

	return mPartDistCurveCutIdx << BIT_DIFF;
}

#undef MAX_VOLUME
#undef MIN_VOLUME_RATE
#undef mPartDistCurveCap

#endif
