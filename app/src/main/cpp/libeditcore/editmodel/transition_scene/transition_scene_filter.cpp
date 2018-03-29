#include "./transition_scene_filter.h"

#define LOG_TAG "TransitionSceneFilter"

/* TransitionSceneFilter class */
TransitionSceneFilter::TransitionSceneFilter() {
	fadeInTextureFromCache = NULL;
	fadeOutTextureFromCache = NULL;
	fadeInFrame = NULL;
	fadeOutFrame = NULL;
}

TransitionSceneFilter::TransitionSceneFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) :
		ModelFilter(index, sequenceIn, sequenceOut, filterName) {
	fadeInTextureFromCache = NULL;
	fadeOutTextureFromCache = NULL;
	fadeInFrame = NULL;
	fadeOutFrame = NULL;
}

TransitionSceneFilter::~TransitionSceneFilter() {
	clear();
}

bool TransitionSceneFilter::onInit() {
	//根据放入mMapParamValue 中key为sceneVideoPath的ParamVal 构造解码器
	splitSeconds = (float)(this->sequenceIn + (this->sequenceOut - this->sequenceIn) / 2) / (float)1000000;
	ParamVal val;
	bool suc = this->getFilterParamValue(string(TRANSITION_SCENE_PARAM_ID_PATH), val);
	if (suc) {
		char *videoPath = (char *)val.strVal.c_str();
		LOGI("get videoPath success, videoPath:%s", videoPath);
		SceneVideoDecoder* decoder = new SceneVideoDecoder();
		int initSceneVideoDecoderCode = decoder->openFile(videoPath);
		if (initSceneVideoDecoderCode > 0) {
			int decodeVideoErrorState = 0;
			LOGI("splitSeconds is %f", splitSeconds);
			std::vector<RGBAFrame*>* frames = decoder->getCorrectFrame(splitSeconds, &decodeVideoErrorState);
			fadeOutFrame = frames->at(0);
			fadeInFrame = frames->at(1);
			delete frames;
			decoder->closeFile();
			delete decoder;
			decoder = NULL;
		} else {
			LOGI("Open Scene Video Decoder file failed...");
			decoder = NULL;
			return false;
		}
	} else {
		LOGI("get param value failed, use default value");
	}

	suc = this->getFilterParamValue(string(TRANSITION_SCENE_PARAM_ID_DURATION), val);
	if (suc) {
		duration = val.u.fltVal;
		LOGI("duration is %f", duration);
	}

	int type = TRANSITION_TYPE_FADE_IN;
	suc = this->getFilterParamValue(string(TRANSITION_SCENE_PARAM_ID_TYPE), val);
	if (suc) {
		type = val.u.intVal;
	}
	LOGI("transition type is %d", type);

	return true;
}

void TransitionSceneFilter::onRenderPre(float pos) {
	//第一次进来的时候 进行创建纹理操作
	if (NULL == fadeInTextureFromCache && NULL != fadeInFrame) {
		int width = fadeInFrame->width;
		int height = fadeInFrame->height;
		//Create Texture
		fadeInTextureFromCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
		if (fadeInTextureFromCache) {
			fadeInTextureFromCache->lock();
			GLuint texId = fadeInTextureFromCache->getTexId();
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, fadeInFrame->pixels);
		}
		delete fadeInFrame;
		fadeInFrame = NULL;
	}
	if (NULL == fadeOutTextureFromCache && NULL != fadeOutFrame) {
		int width = fadeOutFrame->width;
		int height = fadeOutFrame->height;
		//Create Texture
		fadeOutTextureFromCache = GPUTextureCache::GetInstance()->fetchTexture(width, height);
		if (fadeOutTextureFromCache) {
			fadeOutTextureFromCache->lock();
			GLuint texId = fadeOutTextureFromCache->getTexId();
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, fadeOutFrame->pixels);
		}
		delete fadeOutFrame;
		fadeOutFrame = NULL;
	}
	GLuint texId = -1;
	float timeDiff = pos - splitSeconds;
	if (timeDiff < 0.0f) {
		texId = fadeInTextureFromCache->getTexId();
	} else{
		texId = fadeOutTextureFromCache->getTexId();
	}
	//put texId to
	ParamVal texIdValue;
	texIdValue.type = EffectParamTypeInt;
	texIdValue.u.intVal = texId;
	this->setFilterParamValue(TRANSITION_SCENE_PARAM_ID_TEXTURE_ID, texIdValue);
	//calculate TRANSITION_SCENE_PARAM_ID_PROGRESS
	float progress = duration / 2 - fabs(timeDiff);
	ParamVal progressValue;
	progressValue.type = EffectParamTypeFloat;
	progressValue.u.fltVal = progress;
	this->setFilterParamValue(TRANSITION_SCENE_PARAM_ID_PROGRESS, progressValue);
}

void TransitionSceneFilter::clear() {
	if (fadeInTextureFromCache){
		fadeInTextureFromCache->unLock();
	}
	if (fadeOutTextureFromCache){
		fadeOutTextureFromCache->unLock();
	}
	if(fadeInFrame){
		delete fadeInFrame;
		fadeInFrame = NULL;
	}
	if(fadeOutFrame){
		delete fadeOutFrame;
		fadeOutFrame = NULL;
	}
}
