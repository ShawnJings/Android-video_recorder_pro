#include "./trailer_scene_filter.h"

#define LOG_TAG "TrailerSceneFilter"

/* TrailerSceneFilter class */
TrailerSceneFilter::TrailerSceneFilter() {
	textureFromCache = NULL;
	texId = -1;
}

TrailerSceneFilter::TrailerSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	textureFromCache = NULL;
	texId = -1;
}

TrailerSceneFilter::~TrailerSceneFilter() {
	clear();
}

//由于iOS的时间传递过来是由Timer生成的传过来，是不准确的，所以这里只要大于sequenceIn就是要执行的
bool TrailerSceneFilter::isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context){
	int64_t position = pos * 1000000;
	if (position >= sequenceIn) {
		return true;
	} else {
		return false;
	}
}

bool TrailerSceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	texId = -1;
	ParamVal val;
	endTimeInSecs = (double)this->sequenceOut / (double)1000000;
	duration = (float)(sequenceIn - sequenceOut) / (float)1000000;
	bool suc = this->getFilterParamValue(string(TRAILER_SCENE_PARAM_ID_DURATION), val);
	if (suc) {
		fadeInDuration = val.u.fltVal;
		LOGI("fadeInDuration is %f", fadeInDuration);
	}
	return true;
}

void TrailerSceneFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (-1 == texId) {
		int width = 480;
		ParamVal val;
		bool suc = this->getFilterParamValue(string(TRAILER_SCENE_PARAM_ID_WIDTH), val);
		if (suc) {
			width = val.u.intVal;
			LOGI("get success, width:%d", width);
		} else{
			LOGI("get width failed use default value %d", width);
		}
		int height = 480;
		suc = this->getFilterParamValue(string(TRAILER_SCENE_PARAM_ID_HEIGHT), val);
		if (suc) {
			height = val.u.intVal;
			LOGI("get success, height:%d", height);
		} else{
			LOGI("get height failed use default value %d", height);
		}
		byte* pixels;
		suc = this->getFilterParamValue(string(TRAILER_SCENE_PARAM_ID_PIXELS), val);
		if (suc) {
			pixels = (byte*)(val.u.arbData);
			LOGI("get trailer overlay pixels success");
			//Create Texture
			textureFromCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
			if (textureFromCache) {
				textureFromCache->lock();
				texId = textureFromCache->getTexId();
				glBindTexture(GL_TEXTURE_2D, texId);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				delete[] pixels;
				//put texId to
				ParamVal paramValue;
				paramValue.type = EffectParamTypeInt;
				paramValue.u.intVal = texId;
				this->setFilterParamValue(TRAILER_SCENE_PARAM_ID_TEXTURE_ID, paramValue);
			}
		} else{
			LOGI("get trailer overlay pixels failed");
		}
	}
	//calculate TRANSITION_SCENE_PARAM_ID_PROGRESS
	float progress = 0.0;
	float timeDiff = pos - endTimeInSecs;
	if(timeDiff > 0){
		progress = 1.0;
	}else if (timeDiff > duration) {
		progress = (fabs(duration) - fabs(timeDiff)) / fadeInDuration;
		progress = progress > 1.0f ? 1.0f : progress;
	}
	ParamVal paramValue;
	paramValue.type = EffectParamTypeFloat;
	paramValue.u.fltVal = progress;
	this->setFilterParamValue(TRAILER_SCENE_PARAM_ID_PROGRESS, paramValue);
}

void TrailerSceneFilter::clear() {
	//TODO:删除文字的Pixels
	if (textureFromCache)
		textureFromCache->unLock();
}
