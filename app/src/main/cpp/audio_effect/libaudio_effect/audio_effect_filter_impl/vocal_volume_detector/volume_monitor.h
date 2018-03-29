#ifndef VOLUME_MONITOR_H_
#define VOLUME_MONITOR_H_

#include <stdlib.h>
#include <vector>
#include <map>

class VolumeMonitor
{
public:
	void   Init(int sampleRate, int channel, double maxSampleValue);
	double MonitorFlow(int16_t inBuffer[], int nSamples);
private:
	double _EnvFull(double in);
	double _Liner2Db(double sample);
	
	double m_rct;
	double m_old;
	int m_sampleRate;
	double m_maxSampleValue;
	int m_channel;
	double m_envCycleSampleSum;
	int m_envCycleSampleCounter;
	int m_offsetChannel;
	int m_envCycleN;
	double m_meanDb;
	double m_lastDb;
	double m_dbDampRatio;
};

#endif
