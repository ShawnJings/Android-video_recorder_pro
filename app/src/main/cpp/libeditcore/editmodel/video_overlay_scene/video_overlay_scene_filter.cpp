#include "./video_overlay_scene_filter.h"

#define LOG_TAG "VideoOverlaySceneFilter"

/* VideoOverlaySceneFilter class */

VideoOverlaySceneFilter::VideoOverlaySceneFilter() {
	decoder = NULL;
	overlayMapFrame = NULL;
	amaroMapFrame = NULL;
	blackBoardTexCache = NULL;
	overlayMapTexCache = NULL;
	amaroTexCache = NULL;
}

VideoOverlaySceneFilter::VideoOverlaySceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	decoder = NULL;
	overlayMapFrame = NULL;
	amaroMapFrame = NULL;
	blackBoardTexCache = NULL;
	overlayMapTexCache = NULL;
	amaroTexCache = NULL;
}

VideoOverlaySceneFilter::~VideoOverlaySceneFilter() {
	clear();
}

bool VideoOverlaySceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	blackBoardTexTd = -1;
	overlayMapTexId = -1;
	amaroMapTexId = -1;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(VIDEO_BLACK_BOARD_PATH), val);
	if (suc) {
		char *videoBlackBoardPath = (char *)val.strVal.c_str();
		LOGI("get success, videoBlackBoardPath:%s", videoBlackBoardPath);
		decoder = new SceneVideoDecoder();
		int initSceneVideoDecoderCode = decoder->openFile(videoBlackBoardPath);
		if (initSceneVideoDecoderCode > 0) {
			LOGI("Open Scene Video Decoder file success...");
		} else {
			LOGI("Open Scene Video Decoder file failed...");
			decoder = NULL;
			return false;
		}
	}
	PngDecoder* decoder = new PngDecoder();
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

GPUTexture* VideoOverlaySceneFilter::buildTexture(RGBAFrame* frame) {
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


void VideoOverlaySceneFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (-1 == blackBoardTexTd && NULL != decoder) {
		int width = decoder->getVideoFrameWidth();
		int height = decoder->getVideoFrameHeight();
		blackBoardTexCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
		if (blackBoardTexCache) {
			blackBoardTexCache->lock();
			blackBoardTexTd = blackBoardTexCache->getTexId();
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = blackBoardTexTd;
		this->setFilterParamValue(BLACK_BOARD_TEXTURE_ID, paramValue);
	}
	if (-1 == overlayMapTexId) {
		if(NULL != overlayMapFrame){
			overlayMapTexCache = buildTexture(overlayMapFrame);
			if (overlayMapTexCache) {
				overlayMapTexId = overlayMapTexCache->getTexId();
			}
			delete overlayMapFrame;
			overlayMapFrame = NULL;
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = overlayMapTexId;
		this->setFilterParamValue(OVERLAY_MAP_TEXTURE_ID, paramValue);
	}
	if (-1 == amaroMapTexId) {
		if(NULL != amaroMapFrame){
			amaroTexCache = buildTexture(amaroMapFrame);
			if (amaroTexCache) {
				amaroMapTexId = amaroTexCache->getTexId();
			}
			delete amaroMapFrame;
			amaroMapFrame = NULL;
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = amaroMapTexId;
		this->setFilterParamValue(AMARO_MAP_TEXTURE_ID, paramValue);
	}
	if(NULL != decoder){
		int decodeVideoErrorState = 0;
		RGBAFrame *frame = NULL;
		frame = decoder->decodeVideoFrame(&decodeVideoErrorState);
		if (NULL != frame) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, blackBoardTexTd);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->pixels);
			delete frame;
		}
	}
}

void VideoOverlaySceneFilter::clear() {
	//删除纹理以及decoder 释放资源
	if (blackBoardTexCache)
		blackBoardTexCache->unLock();

	if (overlayMapTexCache)
		overlayMapTexCache->unLock();

	if (amaroTexCache)
		amaroTexCache->unLock();

	if(overlayMapFrame){
		delete overlayMapFrame;
		overlayMapFrame = NULL;
	}
	if(amaroMapFrame){
		delete amaroMapFrame;
		amaroMapFrame = NULL;
	}
	if (decoder) {
		decoder->closeFile();
		delete decoder;
		decoder = NULL;
	}
}
