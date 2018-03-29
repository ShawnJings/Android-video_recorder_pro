#include "./blur_scene_filter.h"

#define LOG_TAG "BlurSceneFilter"

/* BlurSceneFilter class */
BlurSceneFilter::BlurSceneFilter() {
}

BlurSceneFilter::BlurSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
}

BlurSceneFilter::~BlurSceneFilter() {
	clear();
}

bool BlurSceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	ParamVal val;
	endTimeInSecs = this->sequenceOut / 1000000;
	duration = (float)(sequenceIn - sequenceOut) / (float)1000000;
	return true;
}

bool BlurSceneFilter::isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context){
	if(ModelFilter::isAvailable(pos, pfnDetectGPUSurpportedEffect, context)){
		if(NULL != pfnDetectGPUSurpportedEffect){
			return pfnDetectGPUSurpportedEffect(context);
		}
		return true;
	} else{
		return false;
	}
}

void BlurSceneFilter::onRenderPre(float pos) {
	//calculate TRANSITION_SCENE_PARAM_ID_PROGRESS
	float progress = 1.0;
	float timeDiff = pos - endTimeInSecs;
	if(timeDiff > 0){
		progress = 0.0;
	}else if (timeDiff > duration) {
		progress = fabs(timeDiff) / (0 - duration);
	}
	//如果是越来与模糊的话就是true 默认是越来越清楚
	bool isAsc = false;
	ParamVal isAscValue;
	bool success = getFilterParamValue(BLUR_SCENE_PARAM_ID_BLUR_ASCEND_FLAG, isAscValue);
	if(success){
		LOGI("find isAsc");
		isAsc = isAscValue.u.boolVal;
		LOGI("isAsc is %d", isAsc);
	} else{
		LOGI("not find isAsc is");
	}
	if(isAsc){
		progress = 1.0 - progress;
	}
	ParamVal paramValue;
	paramValue.type = EffectParamTypeFloat;
	paramValue.u.fltVal = progress;
	this->setFilterParamValue(BLUR_SCENE_PARAM_ID_PROGRESS, paramValue);
}

void BlurSceneFilter::clear() {
}
