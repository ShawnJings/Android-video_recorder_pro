#ifndef VIDEO_EFFECT_CACHE_H
#define VIDEO_EFFECT_CACHE_H

#include <map>
#include <string>
using namespace std;

#include "./base_video_effect.h"
#include "../common/video_filter_utils.h"

class VideoEffectCache {
private:
	map<string, BaseVideoEffect *> mMapEffectCache;

public:
	VideoEffectCache();
	virtual ~VideoEffectCache();

	BaseVideoEffect* getVideoEffectFromCache(string effectName);
};

#endif // VIDEO_EFFECT_CACHE_H
