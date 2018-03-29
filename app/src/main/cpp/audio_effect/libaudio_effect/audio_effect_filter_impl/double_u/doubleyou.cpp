#include <inttypes.h>
#include <iostream>
#include <assert.h>
#include "./doubleyou.h"
#include <string.h>
#include <math.h>
#include "CommonTools.h"

using namespace std;

#define PI 3.141592653589793

DoubleYou::DoubleYou()
	:m_leftChordSamples(NULL), m_rightChordSamples(NULL), 
	m_tmpLeftBuf(NULL), m_tmpRightBuf(NULL),m_tmpOutputBuf(NULL),
	m_tmpLeftBuf1(NULL), m_tmpRightBuf1(NULL),m_tmpOutputBuf1(NULL),
	m_tmpLeftPanRateList(NULL), m_tmpRightPanRateList(NULL), m_tmpMidPanRateList(NULL),m_tmpMidPanRateList1(NULL),
	m_volumeRateList(NULL),
	m_tmpBuffer(NULL) ,
	m_eq(NULL), 
	m_pitchShifterLeft(NULL), m_pitchShifterRight(NULL)
{

}

DoubleYou::~DoubleYou()
{
	if (m_pitchShifterRight != NULL)
	{
		delete m_pitchShifterRight;
	}
	m_pitchShifterRight = NULL;

	if (m_pitchShifterLeft != NULL)
	{
		delete m_pitchShifterLeft;
	}
	m_pitchShifterLeft = NULL;

	if (m_eq != NULL)
	{
		delete m_eq;
	}
	m_eq = NULL;

	if (m_rightChordSamples != NULL)
	{
		delete[] m_rightChordSamples;
	}
	m_rightChordSamples = NULL;

	if (m_leftChordSamples != NULL)
	{
		delete[] m_leftChordSamples;
	}
	if (m_tmpRightBuf != NULL)
	{
		delete[] m_tmpRightBuf;
	}
	m_tmpRightBuf = NULL;

	if (m_tmpLeftBuf != NULL)
	{
		delete[] m_tmpLeftBuf;
	}
	m_tmpLeftBuf = NULL;

	if (m_leftPanRateDict != NULL)
	{
		delete[] m_leftPanRateDict;
	}
	m_leftPanRateDict = NULL;
	if (m_rightPanRateDict != NULL)
	{
		delete[] m_rightPanRateDict ;
	}
	m_rightPanRateDict = NULL;
	if (m_midPanRateDict != NULL)
	{
		delete[] m_midPanRateDict ;
	}
	m_midPanRateDict = NULL;
	if (m_volumeRateDict != NULL)
	{
		delete[] m_volumeRateDict ;
	}
	m_volumeRateDict = NULL;
	if (m_tmpBuffer)
	{
		delete[] m_tmpBuffer;
	}
	m_tmpBuffer = NULL;

	if (m_tmpLeftBuf)
	{
		delete[] m_tmpLeftBuf;
	}
	m_tmpLeftBuf = NULL;
	
	if (m_tmpRightBuf)
	{
		delete[] m_tmpRightBuf;
	}
	m_tmpRightBuf = NULL;
	
	if (m_tmpOutputBuf)
	{
		delete[] m_tmpOutputBuf;
	}
	m_tmpOutputBuf = NULL;
	
	if (m_tmpLeftBuf1)
	{
		delete[] m_tmpLeftBuf1;
	}
	m_tmpLeftBuf1 = NULL;
	
	if (m_tmpRightBuf1)
	{
		delete[] m_tmpRightBuf1;
	}
	m_tmpRightBuf1 = NULL;
	
	if (m_tmpOutputBuf1)
	{
		delete[] m_tmpOutputBuf1;
	}
	m_tmpOutputBuf1 = NULL;

	if (m_tmpLeftPanRateList)
	{
		delete[] m_tmpLeftPanRateList;
	}
	m_tmpLeftPanRateList = NULL;
	
	if (m_tmpRightPanRateList)
	{
		delete[] m_tmpRightPanRateList;
	}
	m_tmpRightPanRateList = NULL;
	
	if (m_tmpMidPanRateList)
	{
		delete[] m_tmpMidPanRateList;
	}
	m_tmpMidPanRateList = NULL;

	if (m_tmpMidPanRateList1)
	{
		delete[] m_tmpMidPanRateList1;
	}
	m_tmpMidPanRateList1 = NULL;

	if (m_volumeRateList)
	{
		delete[] m_volumeRateList;
	}
	m_volumeRateList = NULL;
	
}

void DoubleYou::Init(int sampleRate, int channels)
{
	m_fs = sampleRate;
	m_channels = channels;
	m_tmpBuffer = NULL;
	m_tmpBufSize = 0;

	m_delaySampleRight = 828; //TODO 左和声延迟时间
	m_delaySampleLeft = 2901; //TODO 右和声延迟时间

	m_cbPos = 0;
	m_stride = (m_delaySampleLeft > m_delaySampleRight? m_delaySampleLeft : m_delaySampleRight) + 1;
	m_cbSize = m_stride * 3;
	m_stride *= m_channels;

	m_leftChordSamples = new int16_t[m_cbSize];
	memset(m_leftChordSamples, 0, sizeof(int16_t) * m_cbSize);
	m_rightChordSamples = new int16_t[m_cbSize];
	memset(m_rightChordSamples, 0, sizeof(int16_t) * m_cbSize);
	m_tmpLeftBuf = new int16_t[m_cbSize];
	m_tmpRightBuf = new int16_t[m_cbSize];
	m_tmpOutputBuf = new int16_t[m_cbSize];
	m_tmpLeftBuf1 = new int16_t[m_cbSize];
	m_tmpRightBuf1 = new int16_t[m_cbSize];
	m_tmpOutputBuf1 = new int16_t[m_cbSize];

	m_tmpLeftPanRateList = new double[m_cbSize];
	m_tmpRightPanRateList = new double[m_cbSize];
	m_tmpMidPanRateList = new double[m_cbSize];
	m_tmpMidPanRateList1 = new double[m_cbSize];
	m_volumeRateList = new double[m_cbSize];

	//TODO init eq;
	m_eq = new HSFEQ(6, 1306, m_fs);
	m_pitchShifterLeft = new MicroPitchShift();
	m_pitchShifterLeft->Init(-15, sampleRate);
	m_pitchShifterRight = new MicroPitchShift();
	m_pitchShifterRight->Init(15, sampleRate);
	
	m_maxPanPos = sampleRate * 8;
	m_panPos = -m_maxPanPos;

	int dictSize = m_maxPanPos +1;
	m_leftPanRateDict = new  double[dictSize];
	m_rightPanRateDict = new double[dictSize];
	m_midPanRateDict = new   double[dictSize];
	m_volumeRateDict = new   double[dictSize];
	memset(m_leftPanRateDict, 0xffff, sizeof(double) *  dictSize);
	memset(m_rightPanRateDict, 0xffff, sizeof(double) * dictSize);
	memset(m_midPanRateDict, 0xffff, sizeof(double) *   dictSize);
	memset(m_volumeRateDict, 0xffff, sizeof(double) *   dictSize);

}

void DoubleYou::ResetShiftDeposit()
{
    m_pitchShifterLeft->ResetDeposit();
    m_pitchShifterRight->ResetDeposit();
}

int DoubleYou::DYFlow(int16_t inBuffer[], int16_t outBuffer[], int nSamples)
{
	assert((nSamples % m_channels) == 0);
	if (m_tmpBufSize < nSamples)
	{
		if (m_tmpBuffer)
		{
			delete[] m_tmpBuffer;
			m_tmpBuffer = NULL;
		}
		m_tmpBuffer = new int16_t[nSamples];
		m_tmpBufSize = nSamples;
	}

	for (int i = 0; i < nSamples; i+=m_channels)
	{

		int16_t tmpSample = inBuffer[i];
		tmpSample *= 0.5;
		tmpSample = m_eq->BiQuad(tmpSample);
		m_tmpBuffer[i/m_channels] = tmpSample;
	}

	int size = nSamples;
	int todoSize = 0;

	int i; 
	for (i = 0; i < nSamples; i += todoSize)
	{
		if ((m_cbPos + size/m_channels) < m_cbSize)
		{
			todoSize = size;
		}
		else
		{
			todoSize = (m_cbSize - m_cbPos) * m_channels;
		}
		todoSize = todoSize < m_stride? todoSize: m_stride;
		size -= todoSize;

		m_pitchShifterLeft->Shift(m_tmpBuffer + i/m_channels, m_leftChordSamples + m_cbPos, todoSize/m_channels);
		m_pitchShifterRight->Shift(m_tmpBuffer + i/m_channels, m_rightChordSamples + m_cbPos, todoSize/m_channels);

		_MixDelayedChordBatch(inBuffer + i, outBuffer + (2 * i/m_channels), todoSize);//TODO cur position delay
		m_cbPos += todoSize / m_channels;
		if (m_cbPos >= m_cbSize)
		{
			m_cbPos -= m_cbSize;
		}


	}

	return (nSamples * 2 / m_channels);
}

void DoubleYou::_MixDelayedChordBatch(int16_t inBuffer[], int16_t outBuffer[], int nSamples)
{
	//left
	int nDiv2 = nSamples /2;

	int t = 0;
	int t1 = m_cbPos + 1 - m_delaySampleLeft;
	if (t1 < 0)
	{
		memcpy( m_tmpLeftBuf, m_leftChordSamples + (t1 + m_cbSize), -t1 * sizeof(int16_t)); 
		t = -t1;
	}
	if (t < nDiv2)
	{
		memcpy(m_tmpLeftBuf + t, m_leftChordSamples + t1 + t, (nDiv2 - t) * sizeof(int16_t));
	}

	
	t = 0;
	t1 = m_cbPos + 1 - m_delaySampleRight;
	if (t1 < 0)
	{
		memcpy( m_tmpRightBuf, m_rightChordSamples + (t1 + m_cbSize), -t1 * sizeof(int16_t)); 
		t = -t1;
	}
	if (t < nDiv2)
	{
		memcpy(m_tmpRightBuf + t, m_rightChordSamples + t1 + t , (nDiv2 - t) * sizeof(int16_t));
	}

	_GetVolumeRateBatch(m_tmpLeftPanRateList, m_tmpRightPanRateList, m_tmpMidPanRateList, m_volumeRateList, nDiv2);

	_VectorMul(m_tmpLeftBuf, 1, m_volumeRateList, m_tmpLeftBuf, nDiv2);
	_VectorMul(m_tmpRightBuf, 1, m_volumeRateList, m_tmpRightBuf, nDiv2);
	_VectorMul(inBuffer, 2, m_volumeRateList, m_tmpOutputBuf, nDiv2);
	_VectorScale(m_tmpOutputBuf, 0.8, nDiv2);
	_ConstSub(1, m_tmpMidPanRateList, m_tmpMidPanRateList1, nDiv2);
	_MulClipVector(1, 1.5, m_tmpMidPanRateList, m_tmpMidPanRateList, nDiv2);
	_MulClipVector(1, 1.5, m_tmpMidPanRateList1, m_tmpMidPanRateList1, nDiv2);

	_VectorMul(m_tmpLeftBuf, 1, m_tmpLeftPanRateList, m_tmpLeftBuf1, nDiv2);
	_VectorMul(m_tmpRightBuf, 1, m_tmpRightPanRateList, m_tmpRightBuf1, nDiv2);
	_VectorMul(m_tmpOutputBuf, 1, m_tmpMidPanRateList, m_tmpOutputBuf1, nDiv2); 
	_MixBatch(m_tmpLeftBuf1, 1, m_tmpRightBuf1, 1, outBuffer, 2, nDiv2);
	_MixBatch(outBuffer, 2, m_tmpOutputBuf1, 1, outBuffer, 2, nDiv2);

	_ConstSub(1, m_tmpLeftPanRateList, m_tmpLeftPanRateList, nDiv2);
	_ConstSub(1, m_tmpRightPanRateList, m_tmpRightPanRateList, nDiv2);
	//_ConstSub(1, m_tmpMidPanRateList, m_tmpMidPanRateList, nDiv2);
	//_MulClipVector(1, 1.5, m_tmpMidPanRateList, m_tmpMidPanRateList, nDiv2);
	_VectorMul(m_tmpLeftBuf, 1, m_tmpLeftPanRateList, m_tmpLeftBuf1, nDiv2);
	_VectorMul(m_tmpRightBuf, 1, m_tmpRightPanRateList, m_tmpRightBuf1, nDiv2);
	_VectorMul(m_tmpOutputBuf, 1, m_tmpMidPanRateList1, m_tmpOutputBuf1, nDiv2);
	_MixBatch(m_tmpLeftBuf1, 1, m_tmpRightBuf1, 1, outBuffer + 1, 2, nDiv2);
	_MixBatch(outBuffer + 1, 2, m_tmpOutputBuf1, 1, outBuffer + 1, 2, nDiv2);


}

void DoubleYou::_VectorMul(int16_t * inBufferA, int strideA, double * inBufferB, int16_t * outBuffer, int nSamples)
{
	for (int i = 0; i< nSamples; i++)
	{
		outBuffer[i] = inBufferA[i * strideA] * inBufferB[i];
	}
}

void DoubleYou::_VectorScale(int16_t * inBuffer, double scale, int nSamples)
{
	for (int i = 0; i < nSamples; i++)
	{
		inBuffer[i] *= scale;	
	}
}

void DoubleYou::_ConstSub(int16_t a, double*  inBuffer, double * outBuffer, int nSamples)
{
	for (int i = 0; i< nSamples; i++)
	{
		outBuffer[i] = a - inBuffer[i] ;
	}
}

void DoubleYou::_MulClipVector(double max, double mul, double* inBuffer, double * outBuffer, int nSamples)
{
	for (int i = 0; i < nSamples; i++)
	{
		outBuffer[i] = mul * inBuffer[i];
		if (outBuffer[i] > max)
		{
			outBuffer[i] = max;
		}
	}
}

void DoubleYou::_MixBatch(int16_t * inBufferA, int strideA, int16_t * inBufferB, int strideB, int16_t * outBuffer, int strideC, int nSamples)
{
	for (int i = 0; i< nSamples; i++)
	{
		outBuffer[i * strideC] = _Mix(inBufferA[i * strideA],  inBufferB[i * strideB]);
	}
}

void DoubleYou::_GetVolumeRateBatch(double * leftPanRateList, double * rightPanRateList, double * midPanRateList, double * volumeRateList, int nSamples)
{
	for (int i = 0; i < nSamples; )
	{
		int n = nSamples - i;
		if (m_panPos >= 0)
		{
			int tn = m_maxPanPos - m_panPos;
			if (n > tn)
			{
				n = tn;
			}
			memcpy(leftPanRateList + i, m_leftPanRateDict + m_panPos, sizeof(double) * n);
			memcpy(rightPanRateList + i, m_rightPanRateDict + m_panPos, sizeof(double) * n);
			memcpy(midPanRateList + i, m_midPanRateDict + m_panPos, sizeof(double) * n);
			memcpy(volumeRateList + i, m_volumeRateDict + m_panPos, sizeof(double) * n);
		}
		else
		{
			int tn = 0 - m_panPos;
			if (n > tn)
			{
				n = tn;
			}
			for ( int j = 0 ; j < n ; j++)
			{
				int k = m_maxPanPos + m_panPos + j;
				double deg = 2 * PI * (k / double(m_maxPanPos));
				double basePan = cos(deg);
				midPanRateList[i + j] = basePan * 0.48 + 0.5;
				leftPanRateList[i + j] = midPanRateList[i + j] - 0.02;
				rightPanRateList[i + j] = midPanRateList[i + j] + 0.02;
				volumeRateList[i + j] = fabs(sinf(deg) * 0.5) + 0.5;

				m_leftPanRateDict[m_maxPanPos + m_panPos + j] = leftPanRateList[i + j];
				m_rightPanRateDict[m_maxPanPos + m_panPos + j] = rightPanRateList[i + j];
				m_midPanRateDict[m_maxPanPos + m_panPos + j] = midPanRateList[i + j];
				m_volumeRateDict[m_maxPanPos + m_panPos + j] = volumeRateList[i + j];
			}

		}
		m_panPos += n;
		if (m_panPos >= m_maxPanPos)
		{
			m_panPos -= m_maxPanPos;
		}
		i+= n;
	}

}

int16_t DoubleYou::_Mix(int16_t a, int16_t b)
{
	int tmp = a < 0 && b < 0 ? ((int) a + (int) b) - (((int) a * (int) b) / INT16_MIN) : (a > 0 && b > 0 ? ((int) a + (int) b) - (((int) a * (int) b) / INT16_MAX) : a + b);
	return tmp > INT16_MAX ? INT16_MAX : (tmp < INT16_MIN ? INT16_MIN : tmp);
}

