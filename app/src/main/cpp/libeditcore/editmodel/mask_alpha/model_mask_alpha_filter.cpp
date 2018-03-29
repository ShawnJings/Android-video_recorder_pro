#include "./model_mask_alpha_filter.h"

#define LOG_TAG "ModelMaskAlphaFilter"

/* ModelMaskAlphaFilter class */
ModelMaskAlphaFilter::ModelMaskAlphaFilter() {
	maskAlphaTexId = -1;
	textureFromCache = NULL;
}

ModelMaskAlphaFilter::ModelMaskAlphaFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelDecoderFilter(index, sequenceIn, sequenceOut, filterName) {
	maskAlphaTexId = -1;
	textureFromCache = NULL;
}

ModelMaskAlphaFilter::~ModelMaskAlphaFilter() {
	clear();
}

bool ModelMaskAlphaFilter::onInit() {
	ModelDecoderFilter::onInit();
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	maskAlphaTexId = -1;
	maskAlphaDecoder = NULL;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(MASK_ALPHA_PARAM_ID_PATH), val);
	if (suc) {
		char *maskAlphaVideoPath = (char *)val.strVal.c_str();
		LOGI("get maskAlphaVideoPath success, maskAlphaVideoPath:%s", maskAlphaVideoPath);
		maskAlphaDecoder = new SceneVideoDecoder();
		int initDecoderCode = maskAlphaDecoder->openFile(maskAlphaVideoPath);
		if (initDecoderCode > 0) {
			LOGI("Open maskAlpha Video Decoder file success...");
		} else {
			LOGI("Open maskAlpha Video Decoder file failed...");
			maskAlphaDecoder = NULL;
			return false;
		}
	} else {
		LOGI("get param value failed, use default value");
	}
	return true;
}

void ModelMaskAlphaFilter::onRenderPre(float pos) {
	ModelDecoderFilter::onRenderPre(pos);
	//第一次进来的时候 进行创建纹理操作
	if (-1 == maskAlphaTexId) {
		int width = maskAlphaDecoder->getVideoFrameWidth();
		int height = maskAlphaDecoder->getVideoFrameHeight();
		//Create Texture
		textureFromCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);

		if (textureFromCache) {
			textureFromCache->lock();
			maskAlphaTexId = textureFromCache->getTexId();
		}

		glBindTexture(GL_TEXTURE_2D, maskAlphaTexId);
		LOGI("maskAlphaTexId is %d", maskAlphaTexId);
		//put texId to
		ParamVal paramValue;
		paramValue.type = EffectParamTypeInt;
		paramValue.u.intVal = maskAlphaTexId;
		this->setFilterParamValue(MASK_ALPHA_PARAM_ID_TEXTURE_ID, paramValue);
	}
	//根据position和sequenceIn 以及当前纹理的position 判断是否需要解码下一帧操作
	//解码完毕 上传纹理
	int decodeVideoErrorState = 0;
	RGBAFrame *frame = NULL;
	frame = maskAlphaDecoder->decodeVideoFrame(&decodeVideoErrorState);
	if (NULL != frame) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, maskAlphaTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->pixels);
		delete frame;
	}
}

void ModelMaskAlphaFilter::clear() {
	//删除纹理以及decoder 释放资源
	if (textureFromCache)
			textureFromCache->unLock();

	if (maskAlphaDecoder) {
		maskAlphaDecoder->closeFile();
		delete maskAlphaDecoder;
		maskAlphaDecoder = NULL;
	}
}
