#include "./mix_effect_filter.h"

#define LOG_TAG "MixEffectFilter"

MixEffectFilter::MixEffectFilter(){
}

MixEffectFilter::~MixEffectFilter(){
}

int MixEffectFilter::init(AudioEffect* audioEffect){

	return 1;
}

void MixEffectFilter::doFilter(AudioRequest* request, AudioResponse* response){
	short* vocalSamples = request->getVocalBuffer();
	short* accompanySamples = request->getAccompanyBuffer();
	int sampleSize = request->getVocalBufferSize();
	mixtureAccompanyAudio(accompanySamples, vocalSamples, sampleSize, vocalSamples);
}

void MixEffectFilter::destroy(AudioResponse* response){

}
