#ifndef MODEL_MASK_ALPHA_FILTER_H
#define MODEL_MASK_ALPHA_FILTER_H

#include "../video_decoder_scene/model_decoder_filter.h"
#include "../../gpu_texture_cache.h"

class ModelMaskAlphaFilter:public ModelDecoderFilter {
protected:
	SceneVideoDecoder* maskAlphaDecoder;
	GLuint maskAlphaTexId;
	GPUTexture* textureFromCache;

public:
	ModelMaskAlphaFilter();
	ModelMaskAlphaFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~ModelMaskAlphaFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // MODEL_MASK_ALPHA_FILTER_H
