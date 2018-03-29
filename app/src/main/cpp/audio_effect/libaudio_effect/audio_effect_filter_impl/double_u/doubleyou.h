#ifndef DOUBLEYOU_H
#define DOUBLEYOU_H

#include "./hsfeq.h"
#include "./micro_pitchshift.h"
#include <stdlib.h>

class DoubleYou
{
private:
	int m_fs;
	int m_channels;

	int m_delaySampleLeft; // 左和声延迟时间
	int m_delaySampleRight; // 右和声延迟时间

	int m_maxShiftSampleLeft; // 左和声最大容许误差
	int m_maxShiftSampleRight; //

	int64_t m_totalSamples;
	int m_cbPos;
	int m_cbSize;

	int m_stride;

	int16_t * m_leftChordSamples;
	int16_t * m_rightChordSamples;

	int16_t * m_tmpLeftBuf;
	int16_t * m_tmpRightBuf;
	int16_t * m_tmpOutputBuf;
	int16_t * m_tmpLeftBuf1;
	int16_t * m_tmpRightBuf1;
	int16_t * m_tmpOutputBuf1;

	double * m_tmpLeftPanRateList;
	double * m_tmpRightPanRateList;
	double * m_tmpMidPanRateList;
	double * m_tmpMidPanRateList1;
	double * m_volumeRateList;
	
	int16_t * m_tmpBuffer;

	int m_tmpBufSize;
	int m_panPos;
	//double  m_maxPanPos;
	int  m_maxPanPos;
	int m_volumePos;
	//double  m_maxVolumePos;
	int  m_maxVolumePos;

	//double * m_basePanRateDict ;
	double * m_leftPanRateDict ;
	double * m_rightPanRateDict ;
	double * m_midPanRateDict ;
	double * m_volumeRateDict ;
	HSFEQ * m_eq;
	MicroPitchShift * m_pitchShifterLeft;
	MicroPitchShift * m_pitchShifterRight;


public:
	DoubleYou();
	~DoubleYou();
	void Init(int sampleRate, int channels);
	int DYFlow(int16_t inBuffer[], int16_t outBuffer[], int nSamples);
    void ResetShiftDeposit();

private: 
	void _MixDelayedChordBatch(int16_t inBuffer[], int16_t outBuffer[], int nSamples);
	int16_t _Mix(int16_t a, int16_t b);
	void _GetVolumeRateBatch(double * leftPanRateList, double * rightPanRateList, double * midPanRateList, double * volumeRate, int nSamples);
	void _VectorMul(int16_t * inBufferA, int strideA, double * inBufferB, int16_t * outBuffer, int nSamples);
	void _VectorScale(int16_t * inBuffer, double scale, int nSamples);
	void _ConstSub(int16_t  a, double*  inBuffer, double * outBuffer, int nSamples);
	void _MulClipVector(double max, double mul, double* inBuffer, double * outBuffer, int nSamples);
	void _MixBatch(int16_t * inBufferA, int strideA, int16_t * inBufferB, int strideB, int16_t * outBuffer, int strideC, int nSamples);
	

};

#endif
