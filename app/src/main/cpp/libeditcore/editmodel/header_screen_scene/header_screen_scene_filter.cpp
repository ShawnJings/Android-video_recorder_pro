#include "./header_screen_scene_filter.h"

#define LOG_TAG "HeaderSceneFilter"

/* HeaderSceneFilter class */

HeaderSceneFilter::HeaderSceneFilter() {
	decoder = NULL;
	screenFrame = NULL;
	headerVideoTexCache = NULL;
	screenTexCache = NULL;
}

HeaderSceneFilter::HeaderSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	decoder = NULL;
	screenFrame = NULL;
	headerVideoTexCache = NULL;
	screenTexCache = NULL;
}

HeaderSceneFilter::~HeaderSceneFilter() {
	clear();
}

bool HeaderSceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	screenTexId = -1;
	headerVideoTexId = -1;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(HEADER_VIDEO_PATH), val);
	if (suc) {
		char *headerVideoPath = (char *)val.strVal.c_str();
		LOGI("get success, headerVideoPath:%s", headerVideoPath);
		decoder = new SceneVideoDecoder();
		int initSceneVideoDecoderCode = decoder->openFile(headerVideoPath);
		if (initSceneVideoDecoderCode > 0) {
			LOGI("Open Scene Video Decoder file success...");
		} else {
			LOGI("Open Scene Video Decoder file failed...");
			decoder = NULL;
			return false;
		}
	}
	PngDecoder* decoder = new PngDecoder();
	suc = this->getFilterParamValue(string(HEADER_SCREEN_PIC_PATH), val);
	if (suc) {
		char *screenPicPath = (char *) val.strVal.c_str();
		LOGI("get success, screenPicPath:%s", screenPicPath);
		if (decoder->openFile(screenPicPath) > 0) {
			screenFrame = decoder->getRGBAFrame();
			decoder->closeFile();
		}
	}
	delete decoder;
	return true;
}

GPUTexture* HeaderSceneFilter::buildTexture(RGBAFrame* frame) {
	GPUTexture* texCache = NULL;
	if(frame){
		int width = frame->width;
		int height = frame->height;
		texCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
		if (texCache) {
			texCache->lock();
			GLuint texId = texCache->getTexId();
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					(GLsizei) width,
					(GLsizei) height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, frame->pixels);
		}
	}
	return texCache;
}


void HeaderSceneFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (-1 == headerVideoTexId) {
		int width = decoder->getVideoFrameWidth();
		int height = decoder->getVideoFrameHeight();
		headerVideoTexCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
		if (headerVideoTexCache) {
			headerVideoTexCache->lock();
			headerVideoTexId = headerVideoTexCache->getTexId();
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = headerVideoTexId;
		this->setFilterParamValue(HEADER_VIDEO_TEXTURE_ID, paramValue);
	}
	if (-1 == screenTexId) {
		screenTexCache = buildTexture(screenFrame);
		if (screenTexCache) {
			screenTexId = screenTexCache->getTexId();
		}
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = screenTexId;
		this->setFilterParamValue(HEADER_SCREEN_TEXTURE_ID, paramValue);
		delete screenFrame;
		screenFrame = NULL;
	}

	int decodeVideoErrorState = 0;
	RGBAFrame *frame = NULL;
	frame = decoder->decodeVideoFrame(&decodeVideoErrorState);
//	LOGI("frame->position : %.3f ", frame->position);
	if (NULL != frame) {
		float timeDiff = frame->position - pos;
//		LOGI("frame->position : %.3f pos:%.3f timeDiff : %.3f", frame->position, pos, timeDiff);
		if(fabs(timeDiff) > 0.2){
//			LOGI("we will seek to pos %.3f ", pos);
			delete frame;
			decoder->seek_frame(pos);
			frame = decoder->decodeVideoFrame(&decodeVideoErrorState);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, headerVideoTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->pixels);
		delete frame;
	}
}

void HeaderSceneFilter::clear() {
	//删除纹理以及decoder 释放资源
	if (headerVideoTexCache)
		headerVideoTexCache->unLock();

	if (screenTexCache)
		screenTexCache->unLock();

	if(screenFrame){
		delete screenFrame;
		screenFrame = NULL;
	}
	if (decoder) {
		decoder->closeFile();
		delete decoder;
		decoder = NULL;
	}
}
