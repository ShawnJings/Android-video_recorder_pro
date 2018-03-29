#ifndef THEME_PARSER_H
#define THEME_PARSER_H
#include <unistd.h>
#include "../3rdparty/markup/Markup.h"
#include "../platform_dependent/platform_common.h"
#include "../common/video_effect_params.h"
#include "../editmodel/model_timeline.h"

class CMarkup;

typedef bool (*pfnGetTextPixelsByPlatforms)(TextCanvasParam* textCanvasParam, byte* buffer, void *context);
typedef bool (*pfnGetOverlayPixelsByPlatforms)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context);
typedef bool (*pfnDetectGPUSurpportedEffect)(void *context);

static inline bool isFileSuffix(const char* picFilePath){
	bool ret = false;
	const char *suffix = strrchr(picFilePath, '.');
	if (0 == strcmp(suffix, ".png") || 0 == strcmp(suffix, ".mp4")) {
		ret = true;
	}
	return ret;
}

class ThemeParser {
public:
	static ThemeParser* GetInstance(); //工厂方法(用来获得实例)
	virtual ~ThemeParser();

	bool readThemeProtocol(char* themePath, ModelTimeline* timeline, int frameWidth, int frameHeight, float durationInSec,
			bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void *context),
			bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
			void *context);

private:
	ThemeParser(); //注意:构造方法私有
	static ThemeParser* instance; //惟一实例

	bool readFilter(CMarkup& xmlParser, ModelTimeline* timeline, char* themePath, int frameWidth, int frameHeight, float durationInSec);

	bool buildTrialerSceneByPlatforms(ModelTimeline* timeline, int videoTrackIndex, int filterIndex, int frameWidth, int frameHeight);
	bool buildTextSceneByPlatforms(ModelTimeline* timeline, int videoTrackIndex, int filterIndex);

	void fillParamValue(string paramValue, string paramType, ParamVal & param, bool isAppendThemePath, char* themePath);
	pfnGetTextPixelsByPlatforms getTextPixelsByPlatforms;
	pfnGetOverlayPixelsByPlatforms getOverlayPixelsByPlatforms;
	void* getTextPixelsContext;
};

#endif //THEME_PARSER_H
