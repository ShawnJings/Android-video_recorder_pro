#define _VOCAL_DETECT_H_
#include <stdlib.h>
#include <vector>
#include <map>

#define AGC_AUDIO_VOLUME_DB         -6
#define AGC_ACCOMPANY_VOLUME_DB     -12

typedef struct vocal_status 
{
	int beginMs;
	int status;

	vocal_status(int p_beginMs, int p_status)
	{
		beginMs = p_beginMs;
		status = p_status;
	}
}VocalStatus;
/**
 * 包络检波功能，模拟了硬件半波检波和全波检波功能
 */
class VocalDetect
{
private:
    double m_rct;
    double m_old;
	int m_sampleRate;
	int m_attackSample;
	int m_releaseSample;
	double m_attackThresDb;
	double m_releaseThresDb;
	double m_switchPartSamples;
	double m_maxSampleValue;
	bool m_flag4Recompute;
	double m_curMaxDb;
	double m_maxDb;
	double m_gainDb;
	double m_gain;
	std::vector<double> m_dbSampleList;
	std::map<int, int> m_dbSampleAbst;
	std::vector<double> m_linerSampleList;
	int m_dbSampleCalc;
	int m_totalSamples;
	int m_offsetChannel;
	double m_envCycleSampleSum;
	int m_envCycleSampleCounter;
	int m_envCycleN;
	double m_envNumPerMs;
	int m_status;
	int m_tmpStatus;
	std::map<int, int> m_minEnvNumHoldTime;
	int m_channel;
	std::vector<VocalStatus> m_statusList;
	std::vector<std::pair<int, int> > m_statusListRT;

	double _Db2Liner(double db);
	double _Liner2Db(double sample);
	void _PushBackDb(double db);
	int _JudgeStatus(double db);
	void _SetThreshold(double attackThresDb, double releaseThresDb);
    double _EnvFull(double in);
	void _ReComputeThresDb();

	void _PopBackDbNum(int num);
	void _FillDbSampleNum(int num);
	void _ComputeGain(double expectDb);


public:

    VocalDetect(){m_rct = 190.0, m_old = 0.0;};
    void Init(int sampleRate, int channel, double maxSampleValue);
	int DetectFlow(int16_t inBuffer[], int nSamples);
	double GetCurDb();
	std::vector<double> GetWaves(int number);
	double GetVocalGain();
	double GetAttackThresDb();
	double GetReleaseThresDb();
	int Seekto(int frameNum);
	std::vector<VocalStatus> GetStatusList();
	std::map<int, int> GetDbAbst();
};

