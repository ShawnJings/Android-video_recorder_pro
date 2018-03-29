#include "./header_fade_scene_filter.h"

#define LOG_TAG "HeaderFadeSceneFilter"

/* HeaderFadeSceneFilter class */

HeaderFadeSceneFilter::HeaderFadeSceneFilter() {
	decoder = NULL;
	headerVideoTexCache = NULL;
}

HeaderFadeSceneFilter::HeaderFadeSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	decoder = NULL;
	headerVideoTexCache = NULL;
}

HeaderFadeSceneFilter::~HeaderFadeSceneFilter() {
	clear();
}

bool HeaderFadeSceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	headerVideoTexId = -1;
	beginFadeOutTime = 3000000;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(HEADER_FADE_VIDEO_PATH), val);
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
	suc = this->getFilterParamValue(string(HEADER_FADE_VIDEO_FADE_OUT_TIME_IN_SECS), val);
	if (suc) {
		beginFadeOutTime = val.u.intVal;
		LOGI("get success, beginFadeOutTime:%d", beginFadeOutTime);
	}
	return true;
}

GPUTexture* HeaderFadeSceneFilter::buildTexture(RGBAFrame* frame) {
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


void HeaderFadeSceneFilter::onRenderPre(float pos) {
	if(NULL == decoder){
		return;
	}
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
		this->setFilterParamValue(HEADER_FADE_VIDEO_TEXTURE_ID, paramValue);
	}
	int decodeVideoErrorState = 0;
	RGBAFrame *frame = NULL;
	frame = decoder->decodeVideoFrame(&decodeVideoErrorState);
//	LOGI("frame->position : %.3f ", frame->position);
	if (NULL != frame) {
		float timeDiff = frame->position - pos;
//		LOGI("frame->position : %.3f pos:%.3f timeDiff : %.3f", frame->position, pos, timeDiff);
		if(fabs(timeDiff) > 1.0){
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

	//calculate TRANSITION_SCENE_PARAM_ID_PROGRESS
	float progress = 0.0;
	float fadeOutDurationInSec = (float)(sequenceOut - beginFadeOutTime) / (float)1000000;
	float timeDiff = pos - (float)(beginFadeOutTime / (float)1000000);
	if (timeDiff >= 0.0f) {
		progress = fabs(timeDiff) / fadeOutDurationInSec;
	}
	ParamVal paramValue;
	paramValue.type = EffectParamTypeFloat;
	paramValue.u.fltVal = progress;
	this->setFilterParamValue(HEADER_FADE_VIDEO_FADE_OUT_PROGRESS, paramValue);
}

void HeaderFadeSceneFilter::clear() {
	//删除纹理以及decoder 释放资源
	if (headerVideoTexCache)
		headerVideoTexCache->unLock();

	if (decoder) {
		decoder->closeFile();
		delete decoder;
		decoder = NULL;
	}
}
