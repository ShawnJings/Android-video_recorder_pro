#include "./video_effect_cache.h"

#define LOG_TAG "VideoEffectProcessor"

VideoEffectCache::VideoEffectCache() {
	mMapEffectCache.clear();
}

VideoEffectCache::~VideoEffectCache() {
	map<string, BaseVideoEffect *>::iterator itor = mMapEffectCache.begin();
	for (; itor != mMapEffectCache.end(); itor++) {
		itor->second->destroy();
		delete itor->second;
	}
	mMapEffectCache.clear();
}

BaseVideoEffect* VideoEffectCache::getVideoEffectFromCache(string effectName) {
	BaseVideoEffect* effect = NULL;
	map<string, BaseVideoEffect *>::iterator itor;
	itor = mMapEffectCache.find(effectName);
	if (itor != mMapEffectCache.end()) {
		effect = (BaseVideoEffect*) itor->second;
	} else {
		effect = createVideoEffect(effectName.c_str());
		if(effect->init()){
			mMapEffectCache[effectName] = effect;
		}
	}
	return effect;
}
