#ifndef VIDEO_OVERLAY_SCENE_FILTER_H
#define VIDEO_OVERLAY_SCENE_FILTER_H

#include "../../platform_dependent/platform_gl.h"
#include "../../common/video_effect_params.h"
#include "../model_filter.h"
#include "../../decoder/rgba_frame.h"
#include "../../decoder/png_decoder.h"
#include "../../decoder/scene_video_decoder.h"
#include "../../gpu_texture_cache.h"

class VideoOverlaySceneFilter: public ModelFilter {
protected:
	GPUTexture* blackBoardTexCache;
	GLuint blackBoardTexTd;
	SceneVideoDecoder* decoder;

	GPUTexture* overlayMapTexCache;
	GLuint overlayMapTexId;
	RGBAFrame*overlayMapFrame;

	GPUTexture* amaroTexCache;
	GLuint amaroMapTexId;
	RGBAFrame* amaroMapFrame;

	GPUTexture* buildTexture(RGBAFrame* frame);
public:
	VideoOverlaySceneFilter();
	VideoOverlaySceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~VideoOverlaySceneFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // VIDEO_OVERLAY_SCENE_FILTER_H
