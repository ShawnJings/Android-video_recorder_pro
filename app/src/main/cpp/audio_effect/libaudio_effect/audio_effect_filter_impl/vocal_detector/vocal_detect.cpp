#include "vocal_detect.h"
#include <math.h>
#include <iostream>
using namespace std;
/** \brief 初始化
 *
 * \param rct 为RC低通滤波的时间常数
 * \return
 */
void VocalDetect::Init(int sampleRate, int channel, double maxSampleValue)
{
    m_rct = 190;
    m_old = 0.0;
	m_sampleRate = sampleRate;
	m_channel = channel;
	_SetThreshold(-30, -40);
	m_curMaxDb = -90;
	m_maxDb = -90;
	m_gainDb = 0;
	m_gain = -1;
	m_switchPartSamples = 0;
	m_maxSampleValue = maxSampleValue;
	m_dbSampleList.clear();
	m_dbSampleAbst.clear();
	for (int i = 0; i > -95; i--)
	{
		m_dbSampleAbst[i] = 0;
	}
	m_dbSampleCalc = 0;
	m_totalSamples = 0;
	m_offsetChannel = 0;
	m_envCycleSampleSum = 0;
	m_envCycleSampleCounter = 0;
	m_envCycleN = int(m_sampleRate / 1000);
	m_envNumPerMs = m_sampleRate / double(1000 * m_envCycleN );
	m_attackSample = int(0.03 * m_sampleRate / m_envCycleN); // 30毫秒
	m_releaseSample = int(0.03 * m_sampleRate / m_envCycleN); // 30毫秒
	m_status = -1;
	m_tmpStatus = -1;
	m_minEnvNumHoldTime[-1] = 1500 * m_envNumPerMs; //关闭时间最少不少于1.5秒
	m_minEnvNumHoldTime[1] = 300 * m_envNumPerMs;	//打开时间最少不少于300毫秒
	m_statusListRT.clear();
	m_statusListRT.push_back(make_pair(0, -1));
	m_flag4Recompute = false;

}

void VocalDetect::_SetThreshold(double attackThresDb, double releaseThresDb)
{
	m_attackThresDb = attackThresDb;
	m_releaseThresDb = releaseThresDb;
}

 /** \brief 全波包络检测
  *
  * \param in[] 输入波形
  * \param N 数组的点数
  * \param out[] 输出波形
  * \return
  */
double  VocalDetect::_EnvFull(double in)
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

double VocalDetect::_Liner2Db(double sample)
{
	double ampRate = fabs(sample) / m_maxSampleValue;
	if (ampRate < 0.00003)
	{
		return -90;
	}
	return 20 * log10(ampRate);
}

double VocalDetect::_Db2Liner(double db)
{
	if (db > 0)
	{
		db = 0;
	}
	if (db == -90)
	{
		return 0;
	}
	return pow(10, db /20);
}

void VocalDetect::_PushBackDb(double db)
{
	if (db > m_maxDb)
	{
		m_maxDb = db;
	}
	m_dbSampleList.push_back(db);
	m_linerSampleList.push_back(_Db2Liner(db));
	m_dbSampleAbst[round(db)]++;
	if (m_curMaxDb < db)
	{
		m_curMaxDb = db;
	}

}

void VocalDetect::_PopBackDbNum(int num)
{
	for (int i = 0; i < num; i++)
	{
		m_dbSampleList.pop_back();
	}
}

void VocalDetect::_FillDbSampleNum(int num)
{
	for (int i = 0; i < num; i++)
	{
		m_dbSampleList.push_back(-90);
		m_linerSampleList.push_back(0);
	}
}

int VocalDetect::Seekto(int frameNum)
{
	int offsetFrameNum = frameNum - m_totalSamples;

	m_envCycleSampleCounter += offsetFrameNum;
	int n = 0;
	while (m_envCycleSampleCounter > 0)
	{
		m_envCycleSampleCounter -= m_envCycleN;
		n++;
	}
	while (m_envCycleSampleCounter < 0)
	{
		m_envCycleSampleCounter += m_envCycleN;
		n--;
	}
	m_totalSamples = frameNum;
	if (n < 0)
	{
		_PopBackDbNum(-n);
		int tmpStatus = 0;
		for (vector<pair<int, int> >::reverse_iterator rit = m_statusListRT.rbegin();
				rit != m_statusListRT.rend(); 
				rit++)
		{
			if (rit->first <=m_totalSamples)
			{
				tmpStatus = rit->second;
				break;
			}
			else
			{
				m_statusListRT.pop_back();
			}
		}
		if (m_statusListRT.size() == 0)
		{
			m_statusListRT.push_back(make_pair(0, -1));
			tmpStatus = -1;
		}
		if (tmpStatus != m_status)
		{
			m_status = tmpStatus;
			return m_status;
		}

	}
	else
	{
		_FillDbSampleNum(n);
		if ((m_status >0) && (offsetFrameNum > m_sampleRate))
		{
			m_status = -1;
			return m_status;
		}
	}
	return 0;
}

int VocalDetect::_JudgeStatus(double db)
{
	int ret = m_status;
	m_switchPartSamples++;
	if (m_tmpStatus == -1) // off 
	{
		if (db > m_attackThresDb) // off -> on temporarily
		{
			m_tmpStatus = 1;
			m_switchPartSamples = 0;
		}
	}
	else // on
	{
		if (db < m_releaseThresDb) // on -> off temporarily
		{
			m_tmpStatus = -1;
			m_switchPartSamples = 0;
		}
	}

	if (m_tmpStatus != m_status) // 判断attack/release时间
	{
		if (m_tmpStatus == 1)  // on
		{
			if (m_switchPartSamples > m_attackSample) // turn on
			{
				ret = 1;
			}
		}
		else // off
		{
			if (m_switchPartSamples > m_releaseSample) // turn off
			{
				ret = -1;
			}
		}

	}
	return ret;

}

int VocalDetect::DetectFlow(int16_t inBuffer[], int nSamples)
{

	int orgStatus = m_status;
	double tmpSample;
	int tmpTotalSampleNum = m_totalSamples;
	for (int i = m_offsetChannel; i < nSamples; i+=m_channel)
	{
		tmpSample = inBuffer[i];

		double envSample = _EnvFull(tmpSample);
		double db = _Liner2Db(envSample); //20 * log10(envSample / m_maxSampleValue);
		m_envCycleSampleSum += db;
		m_envCycleSampleCounter++;
		m_totalSamples++;
		if (m_envCycleSampleCounter >= m_envCycleN)
		{
			//m_envCycleN 个db的平均值
			double meanDb = m_envCycleSampleSum / m_envCycleSampleCounter;
			m_envCycleSampleSum = 0;
			m_envCycleSampleCounter = 0;
			//push to m_dbSampleAbst  能量分布统计
			_PushBackDb(meanDb);
			//m_dbSampleAbst.push_back(db);

			m_status = _JudgeStatus(meanDb);
		}

	}

	m_offsetChannel = (m_offsetChannel + nSamples) % m_channel;
	if (m_status == orgStatus)
	{
		return 0;
	}
	m_statusListRT.push_back(make_pair(tmpTotalSampleNum, m_status));
	return m_status;
}

double VocalDetect::GetCurDb()
{
	/*
	double ret = m_curMaxDb;
	m_curMaxDb = -90;
	return m_curMaxDb;
	*/

	if (m_dbSampleList.size() == 0)
	{
		return 0;
	}
	return *(m_dbSampleList.rbegin());
}

vector<double> VocalDetect::GetWaves(int number)
{
	if (m_gain < 0)
	{
		_ComputeGain(AGC_AUDIO_VOLUME_DB);
	}
	vector<double> ret;
	double rate;
	if (m_linerSampleList.size() > number)
	{
		rate = m_linerSampleList.size() / double(number);
	}
	else
	{
		rate = 1;
	}
	double tmpGain = m_gain;
	tmpGain *= pow(10, -double(AGC_AUDIO_VOLUME_DB)/20);
	
	double s = 0;
	int j = 0;
	int k = 0;
	int endPos = 0;
	double it = 0;
	for (int i = 0; i < number; i++)
	{
		s = 0;
		k = 0;
		endPos = round((i + 1)* rate);
		endPos = endPos < m_linerSampleList.size() ? endPos : m_linerSampleList.size();
		for (; j < endPos; j++)
		{
			s += m_linerSampleList[j];
			k++;
		}
		if (k == 0)
		{
			it = 0;
		}
		else
		{
			it = tmpGain * s / k;
		}
		if (it > 1.0)
		{
			it = 1.0;
		}
		ret.push_back(it);
	}
	return ret;

}

double VocalDetect::GetVocalGain()
{
	if (m_gain < 0)
	{
		_ComputeGain(AGC_AUDIO_VOLUME_DB);
	}
	return m_gain;
}

void VocalDetect::_ComputeGain(double expectDb)
{
	int totalAvailabeSample = 0;
	int i ;
	for (i = 0; i > m_attackThresDb; i--)
	{
		totalAvailabeSample+= m_dbSampleAbst[i];
	}
	int partMax = totalAvailabeSample / 50;
	int partRecommand = totalAvailabeSample / 5;
	int tCount = 0;
	int maxDb = 0;	
	for (i = 0; i >  m_attackThresDb; i--)
	{
		tCount += m_dbSampleAbst[i];
		if (tCount > partMax)
		{
			maxDb = i;
			break;
		}
	}
	if (maxDb > m_maxDb)
	{
		maxDb = round(m_maxDb);
	}
	int recommandDb = maxDb - 3;
	for (; i > recommandDb; i-- )
	{
		tCount += m_dbSampleAbst[i];
		if (tCount > partRecommand)
		{
			recommandDb = i;
			break;
		}
	}
	m_gainDb = expectDb - recommandDb;
	if (m_gainDb > 30)
	{
		m_gainDb = 30;
	}
	if (m_gainDb > 8)
	{
		m_gainDb = 8 + (m_gainDb - 8) / 2;
	}
	if (m_gainDb > 12)
	{
		m_gainDb = 12 + (m_gainDb - 12) / 2;
	}
	m_gain = pow(10.0, m_gainDb / 20);
}

vector<VocalStatus > VocalDetect::GetStatusList()
{
	if (m_statusList.size() != 0)
	{
		return m_statusList;
	}
	_ReComputeThresDb();
	m_tmpStatus = -1;
	m_switchPartSamples = 0;
	m_status = -1;
	int precStatus = -1;
	int precMs = m_minEnvNumHoldTime[-1];
	precMs = precMs > m_minEnvNumHoldTime[1] ? precMs: m_minEnvNumHoldTime[1];
	precMs = -precMs -1;
	int initPrecMs = precMs;
	//vector<VocalStatus> ret;
	m_statusList.clear();
	for (vector<double>::iterator it = m_dbSampleList.begin();
			it != m_dbSampleList.end();
			it++)
	{
		int org_status = m_status;
		m_status = _JudgeStatus(*it);
		if (m_status != org_status)
		{
			int envNum = it-m_dbSampleList.begin();


			if ((envNum - precMs) > m_minEnvNumHoldTime[precStatus])
			{
				precMs = int((envNum - m_attackSample)/ m_envNumPerMs) ;
				precStatus = m_status;
				m_statusList.push_back(VocalStatus(precMs, m_status));
			}
			else
			{
				if (m_statusList.size() > 0)
				{
					if (m_statusList.size() == 1)
					{
						precMs = initPrecMs;
						precStatus = -1;
					}
					else
					{
						precMs = (m_statusList.rbegin()+1)->beginMs;
						precStatus = (m_statusList.rbegin()+1)->status;
					}
					m_statusList.pop_back();
				}
			}
		}
	}
	if (m_statusList.size() == 0)
	{
		m_statusList.push_back(VocalStatus(0, -1));
	}

	return m_statusList;
	
}

//判断能量分布： 找到最低点的排序，依次判断，左右差相加最大

void VocalDetect::_ReComputeThresDb()
{
	if (m_flag4Recompute)
	{
		return ;
	}
	map<double, int> sortedDb;
	int totalNum = 0;

	for (map<int, int>::reverse_iterator mit = m_dbSampleAbst.rbegin();
			mit != m_dbSampleAbst.rend();
			mit++)
	{
		totalNum += mit->second;
	}
	int tmpTotalNum = 0;
	for (map<int, int>::reverse_iterator mit = m_dbSampleAbst.rbegin();
			mit != m_dbSampleAbst.rend();
			mit++)
	{
		tmpTotalNum += mit->second;
		if (tmpTotalNum * 100 < totalNum)
		{
			continue;
		}
		if (mit->second < totalNum / 100)
		{
			mit->second = 0; //TODO
		}

		double num = mit->second;
		while (sortedDb.find(num) != sortedDb.end())
		{
			num += 0.00001;
		}
		sortedDb[num] = mit->first;
	}

	double maxVDiff = 0;
	int attackThresDb = m_attackThresDb;
	int releaseThresDb = m_releaseThresDb;
	for (map<double, int>::iterator mit = sortedDb.begin();
			mit != sortedDb.end();
			mit++)
	{
		double leftMax = -1;
		int leftMaxDb = -1;
		double rightMax = -1;
		int rightMaxDb = -1;
		for (map<double, int>::reverse_iterator rit = sortedDb.rbegin();
				rit != sortedDb.rend();
				rit++)
		{
			if ((leftMax == -1) && ((mit->second - rit->second) >= 5))
			{
				leftMax = rit->first;
				leftMaxDb = rit->second;
			}
			if ((rightMax == -1) && ((mit->second - rit->second) <= -5))
			{
				rightMax = rit->first;
				rightMaxDb = rit->second;
			}
			if ((leftMax > -1) && (rightMax > -1))
			{
				break;
			}
		}
		if ((leftMax < totalNum / 300) || (rightMax < totalNum / 300))
		{
			continue;
		}
		//vDiff: 将leftMax和rightMax两点连成直线l，l与mit的垂直距离
	//	double vDiff = leftMax + (mit->second - leftMaxDb) * (rightMax - leftMax) / (rightMaxDb - leftMaxDb) - mit->first;
		double vDiff = leftMax + rightMax - 2 * mit->first;
		if ((vDiff > 0) && (vDiff > maxVDiff))
		{
			maxVDiff = vDiff;
			attackThresDb = mit->second;
			//releaseThresDb = (leftMaxDb - mit->second) / 2;
			double firstLaggerDb;
            for (firstLaggerDb = attackThresDb;
                 (m_dbSampleAbst[firstLaggerDb] <= m_dbSampleAbst[attackThresDb]) && (firstLaggerDb > -90);
                 firstLaggerDb-- );
			releaseThresDb = (firstLaggerDb - mit->second);
			releaseThresDb = (releaseThresDb < -5) ? releaseThresDb : -5;
			releaseThresDb = (releaseThresDb > - 20) ? releaseThresDb: -20;
			releaseThresDb += mit->second;
		}
	}
	if ((attackThresDb < -8) && (attackThresDb > -60))
	{
		_SetThreshold(attackThresDb, releaseThresDb);
	}
	m_flag4Recompute = true;
}

double VocalDetect::GetAttackThresDb()
{
	return m_attackThresDb;
}

double VocalDetect::GetReleaseThresDb()
{
	return m_releaseThresDb;
}

map<int, int> VocalDetect::GetDbAbst()
{
	return m_dbSampleAbst;
}
