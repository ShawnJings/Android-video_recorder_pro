#include "./text_scene_filter.h"

#define LOG_TAG "TextSceneFilter"

/* TextSceneFilter class */

TextSceneFilter::TextSceneFilter() {
	textTextureId = -1;
	textTextureCache = NULL;
}

TextSceneFilter::TextSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	textTextureId = -1;
	textTextureCache = NULL;
}

TextSceneFilter::~TextSceneFilter() {
	clear();
}

bool TextSceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	textTextureId = -1;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(TEXT_SCENE_PARAM_ID_FADE_IN_END_TIME), val);
	if (suc) {
		fadeInEndTime = val.u.intVal;
	}
	suc = this->getFilterParamValue(string(TEXT_SCENE_PARAM_ID_FADE_OUT_START_TIME), val);
	if (suc) {
		fadeOutStartTime = val.u.intVal;
	}
	return true;
}

void TextSceneFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (-1 == textTextureId) {
		int width = 480;
		ParamVal val;
		bool suc = this->getFilterParamValue(string(TEXT_SCENE_PARAM_ID_WIDTH), val);
		if (suc) {
			width = val.u.intVal;
			LOGI("get success, width:%d", width);
		} else{
			LOGI("get width failed use default value %d", width);
		}
		int height = 480;
		suc = this->getFilterParamValue(string(TEXT_SCENE_PARAM_ID_HEIGHT), val);
		if (suc) {
			height = val.u.intVal;
			LOGI("get success, height:%d", height);
		} else{
			LOGI("get height failed use default value %d", height);
		}
		byte* pixels;
		suc = this->getFilterParamValue(string(TEXT_SCENE_PARAM_ID_PIXELS), val);
		if (suc) {
			pixels = (byte*)(val.u.arbData);
			LOGI("get pixels success");
			//Create Texture
			textTextureCache = GPUTextureCache::GetInstance()->fetchTexture(
					width, height);

			if (textTextureCache) {
				textTextureCache->lock();
				textTextureId = textTextureCache->getTexId();
			}

			glBindTexture(GL_TEXTURE_2D, textTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			LOGI("text TextureId is %d", textTextureId);
			if(pixels){
				delete[] pixels;
			}
			//put texId to
			ParamVal paramValue;
			paramValue.type = EffectParamTypeInt;
			paramValue.u.intVal = textTextureId;
			this->setFilterParamValue(TEXT_SCENE_PARAM_ID_TEXTURE_ID, paramValue);
		} else{
			LOGI("get pixels failed");
		}
	}
	//calculate TRANSITION_SCENE_PARAM_ID_PROGRESS
	float progress = 1.0;
	int64_t currentTime = pos * 1000000;
	if(currentTime <= fadeInEndTime){
		progress = (float)(currentTime - sequenceIn) / (float)(fadeInEndTime - sequenceIn);
	} else if(currentTime >= fadeOutStartTime){
		progress = 1.0 - ((float)(currentTime - fadeOutStartTime) / (float)(sequenceOut - fadeOutStartTime));
	}
	ParamVal paramValue;
	paramValue.type = EffectParamTypeFloat;
	paramValue.u.fltVal = progress;
	this->setFilterParamValue(TEXT_SCENE_PARAM_ID_ALPHA_PROGRESS, paramValue);
}

void TextSceneFilter::clear() {
	if (textTextureCache)
		textTextureCache->unLock();
}
