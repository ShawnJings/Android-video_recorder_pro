#ifndef BLUR_SCENE_FILTER_H
#define BLUR_SCENE_FILTER_H

#include "../model_filter.h"
#include "../../platform_dependent/platform_gl.h"
#include "../../gpu_texture_cache.h"
#include "../../decoder/rgba_frame.h"
#include "../../decoder/png_decoder.h"

class BlurSceneFilter:public ModelFilter {
protected:
	float duration;
	float endTimeInSecs;

public:
	BlurSceneFilter();
	BlurSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~BlurSceneFilter();

public:
	virtual bool onInit();

	virtual bool isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context);

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // BLUR_SCENE_FILTER_H
