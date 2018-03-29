#include "./pitch_shift_mix_effect_filter.h"

#define LOG_TAG "PitchShiftMixEffectFilter"

PitchShiftMixEffectFilter::PitchShiftMixEffectFilter(){
	accompanyBufferQueue = NULL;
	vocalBufferQueue = NULL;
	bufferQueueSize = 0;
}

PitchShiftMixEffectFilter::~PitchShiftMixEffectFilter(){
}

int PitchShiftMixEffectFilter::init(AudioEffect* audioEffect){
	audioSampleRate = audioEffect->getAudioInfo()->getAudioSampleRate();
	channels = 2;
	accompanyBufferCursor = 0;
	vocalBufferCursor = 0;
	return 1;
}

void PitchShiftMixEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	//1:拿出两段的数据量
	int accompanySampleSize = request->getAccompanyBufferSize();
	int vocalSampleSize = request->getVocalBufferSize();
	if(0 == bufferQueueSize && NULL == accompanyBufferQueue && NULL == vocalBufferQueue){
		bufferQueueSize = MAX(vocalSampleSize * 4, audioSampleRate * channels * 1.0f);
		accompanyBufferQueue = new short[bufferQueueSize];
		vocalBufferQueue = new short[bufferQueueSize];
	}
	int *accompanyReceiveSamples = (int *) response->get(ACCOMPANYRESPONSE_KEY_RECEIVESAMPLES_SIZE);
	if (accompanyReceiveSamples != NULL) {
		accompanySampleSize = *accompanyReceiveSamples;
		delete accompanyReceiveSamples;
	}
	int *vocalReceiveSamples = (int *) response->get(AUDIORESPONSE_KEY_RECEIVESAMPLES_SIZE);
	if (vocalReceiveSamples != NULL) {
		vocalSampleSize = *vocalReceiveSamples;
		delete vocalReceiveSamples;
	}
	//2:填充两个buffer到Queue里面
	short* vocalSamples = request->getVocalBuffer();
	short* accompanySamples = request->getAccompanyBuffer();
//	LOGI("accompanyBufferCursor %d accompanySampleSize %d bufferQueueSize is %d", accompanyBufferCursor, accompanySampleSize, bufferQueueSize);
	memcpy(accompanyBufferQueue + accompanyBufferCursor, accompanySamples, accompanySampleSize * sizeof(short));
	accompanyBufferCursor += accompanySampleSize;
//	LOGI("vocalBufferCursor %d vocalSampleSize %d bufferQueueSize is %d", vocalBufferCursor, vocalSampleSize, bufferQueueSize);
	memcpy(vocalBufferQueue + vocalBufferCursor, vocalSamples, vocalSampleSize * sizeof(short));
	vocalBufferCursor += vocalSampleSize;
	//3:找一个最小的并且小于等于期望返回的输入buffer的大小
	int alignmentSampleSize = MIN(accompanyBufferCursor, vocalBufferCursor);
	int expectedSampleSize = request->getVocalBufferSize();
	int targetSampleSize = alignmentSampleSize < expectedSampleSize ? alignmentSampleSize : expectedSampleSize;
	//4:处理Queue以及要返回的值
	memcpy(accompanySamples, accompanyBufferQueue, targetSampleSize * sizeof(short));
	int accompanyProtectedSampleSize = accompanyBufferCursor - targetSampleSize;
	memmove(accompanyBufferQueue, accompanyBufferQueue + targetSampleSize, accompanyProtectedSampleSize * sizeof(short));
	accompanyBufferCursor -= targetSampleSize;
	memcpy(vocalSamples, vocalBufferQueue, targetSampleSize * sizeof(short));
	int vocalProtectedSampleSize = vocalBufferCursor - targetSampleSize;
	memmove(vocalBufferQueue, vocalBufferQueue + targetSampleSize, vocalProtectedSampleSize * sizeof(short));
	vocalBufferCursor -= targetSampleSize;
	//5:设置request大小(下一个filter使用)与response的大小(客户端使用)
	request->setVocalBufferSize(targetSampleSize);
	response->put(PITCH_SHIFT_MIX_RESPONSE_KEY_RECEIVE_SAMPLES_SIZE, new int(targetSampleSize));
	MixEffectFilter::doFilter(request, response);
}

void PitchShiftMixEffectFilter::destroy(AudioResponse* response){
	if(NULL != accompanyBufferQueue){
		delete[] accompanyBufferQueue;
		accompanyBufferQueue = NULL;
	}
	if(NULL != vocalBufferQueue){
		delete[] vocalBufferQueue;
		vocalBufferQueue = NULL;
	}
	bufferQueueSize = 0;
	accompanyBufferCursor = 0;
	vocalBufferCursor = 0;
}
