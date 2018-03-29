#ifndef HEADER_FADE_SCENE_FILTER_H
#define HEADER_FADE_SCENE_FILTER_H

#include "../../platform_dependent/platform_gl.h"
#include "../../common/video_effect_params.h"
#include "../model_filter.h"
#include "../../decoder/rgba_frame.h"
#include "../../decoder/png_decoder.h"
#include "../../decoder/scene_video_decoder.h"
#include "../../gpu_texture_cache.h"

class HeaderFadeSceneFilter: public ModelFilter {
protected:
	int beginFadeOutTime;

	SceneVideoDecoder* decoder;
	GPUTexture* headerVideoTexCache;
	GLuint headerVideoTexId;

	GPUTexture* buildTexture(RGBAFrame* frame);
public:
	HeaderFadeSceneFilter();
	HeaderFadeSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~HeaderFadeSceneFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // HEADER_FADE_SCENE_FILTER_H
