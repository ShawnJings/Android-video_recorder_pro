#include "./overlay_scene_filter.h"

#define LOG_TAG "OverlaySceneFilter"

/* OverlaySceneFilter class */

OverlaySceneFilter::OverlaySceneFilter() {
	blackBoardFrame = NULL;
	overlayMapFrame = NULL;
	amaroMapFrame = NULL;
	blackBoardTexCache = NULL;
	overlayMapTexCache = NULL;
	amaroTexCache = NULL;
}

OverlaySceneFilter::OverlaySceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	blackBoardFrame = NULL;
	overlayMapFrame = NULL;
	amaroMapFrame = NULL;
	blackBoardTexCache = NULL;
	overlayMapTexCache = NULL;
	amaroTexCache = NULL;
}

OverlaySceneFilter::~OverlaySceneFilter() {
	clear();
}

bool OverlaySceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	blackBoardTexTd = -1;
	overlayMapTexId = -1;
	amaroMapTexId = -1;
	PngDecoder* decoder = new PngDecoder();
	ParamVal val;
	bool suc = this->getFilterParamValue(string(BLACK_BOARD_PIC_PATH), val);
	if (suc) {
		char *blackBoardPicPath = (char *)val.strVal.c_str();
		LOGI("get success, blackBoardPicPath:%s", blackBoardPicPath);
		if(decoder->openFile(blackBoardPicPath) > 0){
			blackBoardFrame = decoder->getRGBAFrame();
			decoder->closeFile();
		}
	}
	suc = this->getFilterParamValue(string(OVERLAY_MAP_PIC_PATH), val);
	if (suc) {
		char *overlayMapPicPath = (char *) val.strVal.c_str();
		LOGI("get success, overlayMapPicPath:%s", overlayMapPicPath);
		if (decoder->openFile(overlayMapPicPath) > 0) {
			overlayMapFrame = decoder->getRGBAFrame();
			decoder->closeFile();
		}
	}
	suc = this->getFilterParamValue(string(AMARO_MAP_PIC_PATH), val);
	if (suc) {
		char *amaroMapPicPath = (char *) val.strVal.c_str();
		LOGI("get success, amaroMapPicPath:%s", amaroMapPicPath);
		if (decoder->openFile(amaroMapPicPath) > 0) {
			amaroMapFrame = decoder->getRGBAFrame();
			decoder->closeFile();
		}
	}
	delete decoder;
	return true;
}

GPUTexture* OverlaySceneFilter::buildTexture(RGBAFrame* frame) {
	int width = frame->width;
	int height = frame->height;
	GPUTexture* texCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
	if (texCache) {
		texCache->lock();
		GLuint texId = texCache->getTexId();
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				(GLsizei) width,
				(GLsizei) height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, frame->pixels);
	}
	return texCache;
}

void OverlaySceneFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (-1 == blackBoardTexTd) {
		blackBoardTexCache = buildTexture(blackBoardFrame);
		if (blackBoardTexCache) {
			blackBoardTexTd = blackBoardTexCache->getTexId();
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = blackBoardTexTd;
		this->setFilterParamValue(BLACK_BOARD_TEXTURE_ID, paramValue);
		delete blackBoardFrame;
		blackBoardFrame = NULL;
	}
	if (-1 == overlayMapTexId) {
		overlayMapTexCache = buildTexture(overlayMapFrame);
		if (overlayMapTexCache) {
			overlayMapTexId = overlayMapTexCache->getTexId();
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = overlayMapTexId;
		this->setFilterParamValue(OVERLAY_MAP_TEXTURE_ID, paramValue);
		delete overlayMapFrame;
		overlayMapFrame = NULL;
	}
	if (-1 == amaroMapTexId) {
		amaroTexCache = buildTexture(amaroMapFrame);
		if (amaroTexCache) {
			amaroMapTexId = amaroTexCache->getTexId();
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = amaroMapTexId;
		this->setFilterParamValue(AMARO_MAP_TEXTURE_ID, paramValue);
		delete amaroMapFrame;
		amaroMapFrame = NULL;
	}
}

void OverlaySceneFilter::clear() {
	if(blackBoardFrame){
		delete blackBoardFrame;
		blackBoardFrame = NULL;
	}
	if(overlayMapFrame){
		delete overlayMapFrame;
		overlayMapFrame = NULL;
	}
	if(amaroMapFrame){
		delete amaroMapFrame;
		amaroMapFrame = NULL;
	}
	//删除纹理以及decoder 释放资源
	if (blackBoardTexCache)
		blackBoardTexCache->unLock();

	if (overlayMapTexCache)
		overlayMapTexCache->unLock();

	if (amaroTexCache)
		amaroTexCache->unLock();
}
