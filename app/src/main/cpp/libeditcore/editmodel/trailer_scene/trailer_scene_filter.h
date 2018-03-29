#ifndef TRAILER_SCENE_FILTER_H
#define TRAILER_SCENE_FILTER_H

#include "../model_filter.h"
#include "../../platform_dependent/platform_gl.h"
#include "../../gpu_texture_cache.h"
#include "../../decoder/rgba_frame.h"
#include "../../decoder/png_decoder.h"

class TrailerSceneFilter:public ModelFilter {
protected:
	GPUTexture* textureFromCache;
	GLuint texId;
	float fadeInDuration;
	float duration;
	float endTimeInSecs;

public:
	TrailerSceneFilter();
	TrailerSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~TrailerSceneFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();

	virtual bool isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context);

};

#endif // TRAILER_SCENE_FILTER_H
