#ifndef VIDEO_EFFECT_PROCESSOR_H
#define VIDEO_EFFECT_PROCESSOR_H

#include <string>
#include <list>
using namespace std;

#include "./platform_dependent/platform_gl.h"
#include "./platform_dependent/platform_common.h"
#include "./editmodel/model_timeline.h"
#include "./videoeffect/video_effect_cache.h"
#include "./videoeffect/base_video_effect.h"
#include "./common/opengl_video_frame.h"
#include "./common/theme_parser.h"
#include "./editmodel/model_timeline.h"
#include "./common/video_effect_def.h"
#include "./videoeffect/direct_pass_effect.h"
#include "./gpu_texture_cache.h"

#define EFFECT_PROCESSOR_VIDEO_TRACK_INDEX				0

class VideoEffectProcessor {
public:
	VideoEffectProcessor();
	virtual ~VideoEffectProcessor();

private:
	ModelTimeline* timeLine;

	VideoEffectCache* 	effectCache;
	DirectPassEffect* 	directPassEffect;

	// Switching framebuffer-attachable images is much faster than switching between FBOs
	// so only one FBO existed
	OpenglVideoFrame * 	inputVideoFrame;
	OpenglVideoFrame * 	outputVideoFrame;

	bool				mNeedChangeTimeline;
	char *				mThemeProtocolFile;
	pthread_rwlock_t 	mRwlock;
	int					mFrameWidth;
	int					mFrameHeight;
	float 				mDurationInSec;
	void * 				mContext;

	pfnGetTextPixelsByPlatforms getTextPixelsByPlatforms;
	pfnGetOverlayPixelsByPlatforms getOverlayPixelsByPlatforms;
	pfnDetectGPUSurpportedEffect detectGPUSurpportedEffectByPlatforms;

	bool readThemeProtocol(char* protocolFile, int frameWidth, int frameHeight,
			float durationInSec,
			bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam,
					byte* buffer, void* context),
			bool (*pfnGetOverlayPixels)(char* tailerLayerPath,
					TextCanvasParam* textCanvasParam, byte* buffer,
					void *context), void* context);
public:
	bool init();
	void process(OpenglVideoFrame* sourceVideoFrame, float position, OpenglVideoFrame* targetVideoFrame);
	void dealloc();

	// clear theme
	void clearTheme();
	// apply theme function will clear theme first, so there is no need to call clearTheme() before applyTheme()
	bool applyTheme(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
			bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
			bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
			void* context);
	bool applyTheme(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
			bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
			bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
			bool (*pfnDetectGPUSurpportedEffect)(void* context),
			void* context);

	void applyThemeAsync(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
				bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
				bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
				void* context);

	void applyThemeAsync(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
				bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
				bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
				bool (*pfnDetectGPUSurpportedEffect)(void* context),
				void* context);

	bool addVideoTransition(int64_t splitTimeInMicroSecs, int64_t durationInMicroSecs, int type, char* videoPath);

	int addFilter(int trackIndex, int64_t sequenceIn, int64_t sequenceOut, string filterName);
	void setFilterParamValue(int trackIndex, int filterId, string paramName, ParamVal value);
	bool invokeFilterOnReady(int trackIndex, int filterId);

	void removeAllFilters();
	void removeFilter(int trackIndex, int filterId);
};

#endif // VIDEO_EFFECT_PROCESSOR_H
