#include "./mono_2_stereo_effect_filter.h"

#define LOG_TAG "Mono2StereoEffectFilter"

Mono2StereoEffectFilter::Mono2StereoEffectFilter() {
}

Mono2StereoEffectFilter::~Mono2StereoEffectFilter() {
}

int Mono2StereoEffectFilter::init(AudioEffect* audioEffect) {
	return 1;
}

void Mono2StereoEffectFilter::doFilter(AudioRequest* request, AudioResponse* response) {
	short* samples = request->getVocalBuffer();
	int sampleSize = request->getVocalBufferSize();
	for (int i = (sampleSize - 1); i >= 0; i--) {
		samples[i] = samples[i / 2];
	}
}

void Mono2StereoEffectFilter::destroy(AudioResponse* response) {

}
