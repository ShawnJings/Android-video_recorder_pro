#ifndef MODEL_DECODER_FILTER_H
#define MODEL_DECODER_FILTER_H

#include "../model_filter.h"
#include "../../platform_dependent/platform_gl.h"
#include "../../decoder/scene_video_decoder.h"
#include "../../gpu_texture_cache.h"

class ModelDecoderFilter:public ModelFilter {
protected:
	SceneVideoDecoder* decoder;
	GPUTexture* textureFromCache;
	GLuint texId;

	float fps;
	float destFPS;
	bool bNeedSync;
	float curPosition;

public:
	ModelDecoderFilter();
	ModelDecoderFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~ModelDecoderFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // MODEL_DECODER_FILTER_H
