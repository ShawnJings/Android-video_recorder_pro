#include "./model_decoder_filter.h"

#define LOG_TAG "MODEL_DECODER_FILTER"

/* ModelDecoderFilter class */

ModelDecoderFilter::ModelDecoderFilter() {
	textureFromCache = NULL;
	texId = -1;
	bNeedSync = false;
	destFPS = 30.0f;
	curPosition = 0.0f;
}

ModelDecoderFilter::ModelDecoderFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	textureFromCache = NULL;
	texId = -1;
	bNeedSync = false;
	destFPS = 30.0f;
	curPosition = 0.0f;
}

ModelDecoderFilter::~ModelDecoderFilter() {
	clear();
}

bool ModelDecoderFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	texId = -1;
	decoder = NULL;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(GIF_SCENE_PARAM_ID_PATH), val);
	if (suc) {
		char *sceneVideoPath = (char *)val.strVal.c_str();
		LOGI("get sceneVideoPath success, sceneVideoPath:%s", sceneVideoPath);
		decoder = new SceneVideoDecoder();
		int initSceneVideoDecoderCode = decoder->openFile(sceneVideoPath);
		if (initSceneVideoDecoderCode > 0) {
			LOGI("Open Scene Video Decoder file success...");
			fps = decoder->getVideoFrameRate();

			LOGI("fps is %f, dest is %f", fps, destFPS);

			if (fps < destFPS) {
				LOGI("ssdsd");
				bNeedSync = true;
				curPosition = sequenceIn/1000000.0f;
			}
			else
				bNeedSync = false;

		} else {
			LOGI("Open Scene Video Decoder file failed...");
			decoder = NULL;
			return false;
		}
	} else {
		LOGI("get param value failed, use default value");
	}
	return true;
}

void ModelDecoderFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (-1 == texId) {
		int width = decoder->getVideoFrameWidth();
		int height = decoder->getVideoFrameHeight();

		ParamVal paramValGifWidth;
		paramValGifWidth.type = EffectParamTypeInt;
		paramValGifWidth.u.intVal = width;
		this->setFilterParamValue(GIF_SCENE_PARAM_ID_GIF_WIDTH, paramValGifWidth);

		ParamVal paramValGifHeight;
		paramValGifHeight.type = EffectParamTypeInt;
		paramValGifHeight.u.intVal = height;
		this->setFilterParamValue(GIF_SCENE_PARAM_ID_GIF_HEIGHT, paramValGifHeight);

		//Create Texture
		textureFromCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);

		if (textureFromCache) {
			textureFromCache->lock();
			texId = textureFromCache->getTexId();
		}

		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = texId;
		this->setFilterParamValue(GIF_SCENE_PARAM_ID_TEXTURE_ID, paramValue);
	}

	// 这里做一个假设，gif的帧率，永远比我们的帧率低。
//	LOGI("bNeedSync is %d", (int)bNeedSync);

	if (bNeedSync) {
		if (pos >= curPosition) {
			int decodeVideoErrorState = 0;
			RGBAFrame *frame = NULL;
			frame = decoder->decodeVideoFrame(&decodeVideoErrorState);
			if (NULL != frame) {
				glActiveTexture (GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texId);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width,
						frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
						frame->pixels);
				delete frame;
			}
			curPosition += 1/fps;
		} else if(curPosition - pos >= (2 * 1/fps)){
			curPosition = sequenceIn/1000000.0f;
			this->onRenderPre(pos);
		}
	}else {
		int decodeVideoErrorState = 0;
		RGBAFrame *frame = NULL;
		frame = decoder->decodeVideoFrame(&decodeVideoErrorState);
		if (NULL != frame) {
			glActiveTexture (GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->pixels);
			delete frame;
		}
	}
}

void ModelDecoderFilter::clear() {
	if (textureFromCache)
		textureFromCache->unLock();

	if (decoder) {
		decoder->closeFile();
		delete decoder;
		decoder = NULL;
	}
}
