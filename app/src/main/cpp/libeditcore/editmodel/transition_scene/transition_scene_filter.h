#ifndef TRANSITION_SCENE_FILTER_H
#define TRANSITION_SCENE_FILTER_H

#include "../model_filter.h"
#include "../../platform_dependent/platform_gl.h"
#include "../../decoder/scene_video_decoder.h"
#include "../../gpu_texture_cache.h"

class TransitionSceneFilter:public ModelFilter {
protected:
	GPUTexture* fadeInTextureFromCache;
	GPUTexture* fadeOutTextureFromCache;
	RGBAFrame* fadeInFrame;
	RGBAFrame* fadeOutFrame;
	float duration;
	float splitSeconds;

public:
	TransitionSceneFilter();
	TransitionSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~TransitionSceneFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // TRANSITION_SCENE_FILTER_H
