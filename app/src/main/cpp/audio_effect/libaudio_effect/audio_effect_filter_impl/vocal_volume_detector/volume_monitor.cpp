#include "volume_monitor.h"
#include <math.h>
#include <iostream>
void VolumeMonitor::Init(int sampleRate, int channel, double maxSampleValue)
{
	m_rct = 190;
	m_old = 0.0;
	m_sampleRate = sampleRate;
	m_maxSampleValue = maxSampleValue;
	m_channel = channel;
	m_envCycleSampleSum = 0;
	m_envCycleSampleCounter = 0;
	m_offsetChannel = 0;
	m_envCycleN = int(m_sampleRate / 1000);
	m_meanDb = -90;
	m_lastDb = -90;
	m_dbDampRatio = 0.1;
}


double VolumeMonitor::MonitorFlow(int16_t inBuffer[], int nSamples)
{
	double tmpSample;
	for (int i = m_offsetChannel; i < nSamples; i+=m_channel)
	{
		tmpSample = inBuffer[i];
		double envSample = _EnvFull(tmpSample);
		double db = _Liner2Db(envSample);
		m_envCycleSampleSum += db;
		m_envCycleSampleCounter++;
		if (m_envCycleSampleCounter >= m_envCycleN)
		{
			m_meanDb = m_envCycleSampleSum / m_envCycleSampleCounter;
			if (m_meanDb < -90)
			{
				m_meanDb = -90;
			}
			m_envCycleSampleSum = 0;
			m_envCycleSampleCounter =0;
		}

	}
	m_offsetChannel = (m_offsetChannel + nSamples) % m_channel;
	//平滑
	double curDb = m_meanDb;
	double diff = m_lastDb - curDb;
	if (diff > 0)
	{
		m_dbDampRatio *= 1.2;
		diff *= m_dbDampRatio;
	}
	else
	{
		m_dbDampRatio = 0.01;
	}

	curDb = m_lastDb - diff;
	m_lastDb = curDb;

	return curDb;
}

double VolumeMonitor::_EnvFull(double in)
{
    double abs_in;
	abs_in = fabs(in);
	double out;
	if( abs_in > m_old)
	{
		m_old = abs_in;
		out = m_old;
	}
	else
	{
		m_old *= m_rct / ( m_rct + 1 );
		out = m_old;
	}
	return out;
}

double VolumeMonitor::_Liner2Db(double sample)
{
	double ampRate = fabs(sample) / m_maxSampleValue;
	if (ampRate < 0.00003)
	{
		return -90;
	}
	return 20 * log10(ampRate);
}

