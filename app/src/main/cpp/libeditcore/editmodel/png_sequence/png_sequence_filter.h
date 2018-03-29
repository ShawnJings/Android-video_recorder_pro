#ifndef PNG_SEQUENCE_FILTER_H
#define PNG_SEQUENCE_FILTER_H

#include "../../platform_dependent/platform_gl.h"
#include "../../common/video_effect_params.h"
#include "../model_filter.h"
#include "../../decoder/rgba_frame.h"
#include "../../decoder/png_decoder.h"
#include "../../gpu_texture_cache.h"
#include "../../3rdparty/jsoncpp/jsoncpp.h"

class PngSequenceFilter: public ModelFilter {
protected:
	PngDecoder* decoder;
	GPUTexture* texCache;
	GLuint texId;
	int currentIndex;
	string baseName;
	string imageName;
	int imageCount;
	float intervalInSec;
	float curPosition;
	int width;
	int height;

	bool parseScene(char* sceneDirPath);
	string readInputTestFile(const char* path);
public:
	PngSequenceFilter();
	PngSequenceFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~PngSequenceFilter();

public:
	virtual bool onInit();

	virtual void onRenderPre(float pos);

	virtual void clear();
};

#endif // PNG_SEQUENCE_FILTER_H
