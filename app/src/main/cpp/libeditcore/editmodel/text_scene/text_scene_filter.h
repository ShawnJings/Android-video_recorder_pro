#ifndef TEXT_SCENE_FILTER_H
#define TEXT_SCENE_FILTER_H

#include "../model_filter.h"
#include "../../platform_dependent/platform_gl.h"
#include "../../common/video_effect_params.h"
#include "../../gpu_texture_cache.h"

class TextSceneFilter:public ModelFilter {
protected:
	GPUTexture* textTextureCache;
	GLuint textTextureId;

	// fade in end time
	int64_t fadeInEndTime;
	// fade out start time
	int64_t fadeOutStartTime;
public:
	TextSceneFilter();
	TextSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~TextSceneFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // TEXT_SCENE_FILTER_H
