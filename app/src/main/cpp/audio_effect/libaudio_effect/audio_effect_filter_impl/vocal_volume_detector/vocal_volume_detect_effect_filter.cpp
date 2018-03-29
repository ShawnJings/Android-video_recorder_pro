#include "vocal_volume_detect_effect_filter.h"

#define LOG_TAG "VocalVolumeDetectEffectFilter"

VocalVolumeDetectEffectFilter::VocalVolumeDetectEffectFilter() {
}

VocalVolumeDetectEffectFilter::~VocalVolumeDetectEffectFilter() {
}

int VocalVolumeDetectEffectFilter::init(AudioEffect* audioEffect) {
	volumeMonitor = new VolumeMonitor();
	audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	volumeMonitor->Init(audioSampleRate, 1, 32768);
	LOGI("enter VocalVolumeDetectEffectFilter::init()...audioSampleRate=%d", audioSampleRate);
	return 1;
}

void VocalVolumeDetectEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	if (NULL != volumeMonitor) {
		short* samples = request->getVocalBuffer();
		int sampleSize = request->getVocalBufferSize();
		double db = volumeMonitor->MonitorFlow(samples, sampleSize);
		updateVocalCurrentDB(db);
		LOGI("enter VocalDetectEffectFilter::doFilter()....current db=%lf", db);
	}
}

void VocalVolumeDetectEffectFilter::updateVocalCurrentDB(double db) {
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

void VocalVolumeDetectEffectFilter::destroy(AudioResponse* response) {
	LOGI("enter VocalVolumeDetectEffectFilter::destroy()...");
	if (NULL != volumeMonitor) {
		delete volumeMonitor;
		volumeMonitor = NULL;
	}
}
