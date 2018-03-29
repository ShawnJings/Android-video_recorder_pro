#ifndef SELECTIVE_BLUR_SCENE_FILTER_H
#define SELECTIVE_BLUR_SCENE_FILTER_H

#include "../model_filter.h"
#include "../../platform_dependent/platform_gl.h"

class SelectiveBlurSceneFilter:public ModelFilter {
public:
	SelectiveBlurSceneFilter();
	SelectiveBlurSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~SelectiveBlurSceneFilter();

public:
	virtual bool isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context);
};

#endif // SELECTIVE_BLUR_SCENE_FILTER_H
