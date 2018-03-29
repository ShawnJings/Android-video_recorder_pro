#include "./selective_blur_scene_filter.h"

#define LOG_TAG "SelectiveBlurSceneFilter"

/* BlurSceneFilter class */
SelectiveBlurSceneFilter::SelectiveBlurSceneFilter() {
}

SelectiveBlurSceneFilter::SelectiveBlurSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
}

SelectiveBlurSceneFilter::~SelectiveBlurSceneFilter() {
	clear();
}

bool SelectiveBlurSceneFilter::isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context) {
	if (ModelFilter::isAvailable(pos, pfnDetectGPUSurpportedEffect, context)) {
		if (NULL != pfnDetectGPUSurpportedEffect) {
			return pfnDetectGPUSurpportedEffect(context);
		}
		return true;
	} else {
		return false;
	}
}
