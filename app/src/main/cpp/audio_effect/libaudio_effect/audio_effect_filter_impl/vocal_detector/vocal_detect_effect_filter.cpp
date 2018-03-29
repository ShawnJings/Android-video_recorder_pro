#include "./vocal_detect_effect_filter.h"

#define LOG_TAG "VocalDetectEffectFilter"

VocalDetectEffectFilter::VocalDetectEffectFilter() {
	vocalDetector = NULL;
	vocalState = 0;
	vocalContinueTimeMills = 0;
	audioSampleRate = 0;
}

VocalDetectEffectFilter::~VocalDetectEffectFilter() {
}

int VocalDetectEffectFilter::init(AudioEffect* audioEffect) {
	LOGI("enter VocalDetectEffectFilter::init()...");
	vocalDetector = new VocalDetect();
	audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	vocalDetector->Init(audioSampleRate, 1, 32768);
	return 1;
}

void VocalDetectEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != vocalDetector) {

		int* state = (int*)AudioEffectContext::GetInstance()->getResponseDataForKey(VOCAL_DETECT_STATE);
		if (NULL != state) {
			vocalState = *state;
			AudioEffectContext::GetInstance()->deleteResponseDataForKey(VOCAL_DETECT_STATE);
			delete state;
		}
		int* time = (int*)AudioEffectContext::GetInstance()->getResponseDataForKey(VOCAL_DETECT_CONTINUE_TIME);
		if (NULL != time) {
			vocalContinueTimeMills = *time;
			AudioEffectContext::GetInstance()->deleteResponseDataForKey(VOCAL_DETECT_CONTINUE_TIME);
			delete time;
		}

		if (vocalState == VOCAL_DETECT_STATE_STATR) {
			//LOGI("enter VocalDetectEffectFilter::doFilter()....VOCAL_DETECT_STATE_STATR");
			short* samples = request->getVocalBuffer();
			int sampleSize = request->getVocalBufferSize();
			vocalDetector->DetectFlow(samples, sampleSize);
			updateVocalCurrentDB(vocalDetector->GetCurDb());

		} else if (vocalState == VOCAL_DETECT_STATE_PAUSE) {
			//LOGI("enter VocalDetectEffectFilter::doFilter()....VOCAL_DETECT_STATE_PAUSE");

		} else if (vocalState == VOCAL_DETECT_STATE_CONTINUE) {

			short* samples = request->getVocalBuffer();
			int sampleSize = request->getVocalBufferSize();
			int frameNum = (float)vocalContinueTimeMills/1000*audioSampleRate;
			vocalDetector->Seekto(frameNum);
			//LOGI("enter VocalDetectEffectFilter::doFilter()....VOCAL_DETECT_STATE_CONTINUE, vocalContinueTimeMills=%d, frameNum=%d", vocalContinueTimeMills, frameNum);
			vocalDetector->DetectFlow(samples, sampleSize);
			updateVocalCurrentDB(vocalDetector->GetCurDb());

			vocalState = VOCAL_DETECT_STATE_STATR;
			vocalContinueTimeMills = 0;
		}
	}
}

void VocalDetectEffectFilter::updateVocalCurrentDB(double db) {
	//LOGI("enter VocalDetectEffectFilter::updateVocalCurrentDB()....db=%lf", db);
	//先删除之前数据
	double* vocaldb = (double*)AudioEffectContext::GetInstance()->getResponseDataForKey(VOCAL_CURRENT_DB);
	if (vocaldb != NULL) {
		delete vocaldb;
		vocaldb = NULL;
		AudioEffectContext::GetInstance()->deleteResponseDataForKey(VOCAL_CURRENT_DB);
	}

	//插入新数据
	std::map<std::string, void*> paramMap;
	vocaldb = new double;
	*vocaldb = db;
	paramMap.insert(std::pair<std::string, void*>(VOCAL_CURRENT_DB, vocaldb));
	AudioEffectContext::GetInstance()->copyData(&paramMap);
}

void VocalDetectEffectFilter::destroy(AudioResponse* response) {
	LOGI("enter VocalDetectEffectFilter::destroy()...");
	if (NULL != vocalDetector) {
		std::vector<VocalStatus> vocalStatusVector = vocalDetector->GetStatusList();
		LOGI("vocalStatusVector size is %d\n", vocalStatusVector.size());
		std::vector<VocalStatus*>* targetVocalStatus = new std::vector<VocalStatus*>();
		for (int i = 0; i < vocalStatusVector.size(); i++) {
			VocalStatus* vocalStatus = new VocalStatus(vocalStatusVector.at(i).beginMs, vocalStatusVector.at(i).status);
			targetVocalStatus->push_back(vocalStatus);
		}
		response->put(VOCAL_STATUS_LIST, targetVocalStatus);

		//获取人声声波数据
		int* vocalWaveWidth = (int*)AudioEffectContext::GetInstance()->getResponseDataForKey(VOCAL_WAVE_WIDTH);
		if (NULL != vocalWaveWidth) {
			std::vector<double> vocalWaves = vocalDetector->GetWaves(*vocalWaveWidth);
			LOGI("vocalWav.size()=%d, vocalWavWidth is %d\n", vocalWaves.size(), *vocalWaveWidth);
			std::vector<double>* targetVocalWavVector = new std::vector<double>();
			for (int i = 0; i < vocalWaves.size(); i++) {
				targetVocalWavVector->push_back(vocalWaves.at(i));
			}
			response->put(VOCAL_WAVE, targetVocalWavVector);

			AudioEffectContext::GetInstance()->deleteResponseDataForKey(VOCAL_WAVE_WIDTH);
			delete vocalWaveWidth;
		}

		float* agcStatGain = new float;
		*agcStatGain = (float) vocalDetector->GetVocalGain();
		response->put(VOCAL_AGC_STAT_GAIN, agcStatGain);
		LOGI("vocal gain : %f \n", *agcStatGain);

		//处理结束时，设置为最小值-30
		double* minVocalCurDb = new double(-30);
		response->put(VOCAL_CURRENT_DB, minVocalCurDb);

		delete vocalDetector;
		vocalDetector = NULL;
	}
}
