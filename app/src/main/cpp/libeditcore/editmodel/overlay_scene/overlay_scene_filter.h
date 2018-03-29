#ifndef OVERLAY_SCENE_FILTER_H
#define OVERLAY_SCENE_FILTER_H

#include "../../platform_dependent/platform_gl.h"
#include "../../common/video_effect_params.h"
#include "../model_filter.h"
#include "../../decoder/rgba_frame.h"
#include "../../decoder/png_decoder.h"
#include "../../gpu_texture_cache.h"

class OverlaySceneFilter: public ModelFilter {
protected:
	GPUTexture* blackBoardTexCache;
	GLuint blackBoardTexTd;
	RGBAFrame* blackBoardFrame;

	GPUTexture* overlayMapTexCache;
	GLuint overlayMapTexId;
	RGBAFrame*overlayMapFrame;

	GPUTexture* amaroTexCache;
	GLuint amaroMapTexId;
	RGBAFrame* amaroMapFrame;

	GPUTexture* buildTexture(RGBAFrame* frame);
public:
	OverlaySceneFilter();
	OverlaySceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~OverlaySceneFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // OVERLAY_SCENE_FILTER_H
