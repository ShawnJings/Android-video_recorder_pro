#include "./video_effect_processor.h"

#define LOG_TAG "VideoEffectProcessor"

VideoEffectProcessor::VideoEffectProcessor() {
	inputVideoFrame = NULL;
	outputVideoFrame = NULL;
	directPassEffect = NULL;
	effectCache = NULL;
	timeLine = new ModelTimeline();
	timeLine->init();
	mNeedChangeTimeline = false;
	mThemeProtocolFile = NULL;
	pthread_rwlock_init(&mRwlock, NULL);
}

VideoEffectProcessor::~VideoEffectProcessor() {
	mNeedChangeTimeline = false;

	if (mThemeProtocolFile) {
		delete mThemeProtocolFile;
		mThemeProtocolFile = NULL;
	}

	pthread_rwlock_destroy(&mRwlock);
}

bool VideoEffectProcessor::init() {
	inputVideoFrame = new OpenglVideoFrame();
	outputVideoFrame = new OpenglVideoFrame();

	directPassEffect = new DirectPassEffect();
	directPassEffect->init();
	effectCache = new VideoEffectCache();

	return true;
}

bool VideoEffectProcessor::readThemeProtocol(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
		bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
		bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		void* context) {
	bool ret = ThemeParser::GetInstance()->readThemeProtocol(protocolFile, timeLine, frameWidth, frameHeight, durationInSec, pfnGetTextPixels, pfnGetOverlayPixels, context);
	if(!ret){
		//如果添加主题失败, 清除掉所有的主题Filter
		timeLine->clearFilters();
	}
	return ret;
}

void VideoEffectProcessor::clearTheme() {
	pthread_rwlock_wrlock(&mRwlock);
	if (mThemeProtocolFile) {
		delete mThemeProtocolFile;
		mThemeProtocolFile = NULL;
	}
	mNeedChangeTimeline = true;
	pthread_rwlock_unlock(&mRwlock);
}

bool VideoEffectProcessor::applyTheme(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
		bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
		bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		void* context) {
	return this->applyTheme(protocolFile, frameWidth, frameHeight, durationInSec,
			pfnGetTextPixels, pfnGetOverlayPixels, NULL, context);
}

bool VideoEffectProcessor::applyTheme(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
		bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
		bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		bool (*pfnDetectGPUSurpportedEffect)(void* context),
		void* context) {
	if (protocolFile) {
		detectGPUSurpportedEffectByPlatforms = pfnDetectGPUSurpportedEffect;
		mContext = context;
		return this->readThemeProtocol(protocolFile, frameWidth, frameHeight, durationInSec, pfnGetTextPixels, pfnGetOverlayPixels, context);
	}
	return false;
}

void VideoEffectProcessor::applyThemeAsync(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
		bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
		bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		void* context) {
	this->applyThemeAsync(protocolFile, frameWidth, frameHeight, durationInSec,
			pfnGetTextPixels, pfnGetOverlayPixels, NULL, context);
}

void VideoEffectProcessor::applyThemeAsync(char* protocolFile, int frameWidth, int frameHeight, float durationInSec,
		bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void* context),
		bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		bool (*pfnDetectGPUSurpportedEffect)(void* context),
		void* context) {
	pthread_rwlock_wrlock(&mRwlock);

	if (mThemeProtocolFile) {
		delete mThemeProtocolFile;
		mThemeProtocolFile = NULL;
	}

	int length = 0;
	if (protocolFile) {
		length = strlen(protocolFile);
	}

	if (length > 0) {
		mThemeProtocolFile = new char[length + 1];
		memcpy(mThemeProtocolFile, protocolFile, sizeof(char) * (length + 1));
	}

	mFrameWidth = frameWidth;
	mFrameHeight = frameHeight;
	mDurationInSec = durationInSec;
	mContext = context;

	getTextPixelsByPlatforms = pfnGetTextPixels;
	getOverlayPixelsByPlatforms = pfnGetOverlayPixels;
	detectGPUSurpportedEffectByPlatforms = pfnDetectGPUSurpportedEffect;

	mNeedChangeTimeline = true;

	pthread_rwlock_unlock(&mRwlock);
}

bool VideoEffectProcessor::addVideoTransition(int64_t splitTimeInMicroSecs, int64_t durationInMicroSecs, int type, char* videoPath){
	int64_t sequenceIn = splitTimeInMicroSecs - durationInMicroSecs / 2;
	int64_t sequenceOut = splitTimeInMicroSecs + durationInMicroSecs / 2;
	int videoTrackIndex = EFFECT_PROCESSOR_VIDEO_TRACK_INDEX;
	int filterId = timeLine->addTransition(videoTrackIndex, sequenceIn, sequenceOut, TRANSITION_SCENE_FILTER_NAME);
	ParamVal pathValue;
	pathValue.type = EffectParamTypeString;
	pathValue.strVal = string(videoPath);
	timeLine->setTransitionParamValue(videoTrackIndex, filterId, TRANSITION_SCENE_PARAM_ID_PATH, pathValue);
	ParamVal durationValue;
	durationValue.type = EffectParamTypeFloat;
	durationValue.u.fltVal = (double) durationInMicroSecs / (double) 1000000;
	timeLine->setTransitionParamValue(videoTrackIndex, filterId, TRANSITION_SCENE_PARAM_ID_DURATION, durationValue);
	ParamVal typeValue;
	typeValue.type = EffectParamTypeInt;
	typeValue.u.intVal = (int) type;
	timeLine->setTransitionParamValue(videoTrackIndex, filterId, TRANSITION_SCENE_PARAM_ID_TYPE, typeValue);
	return timeLine->invokeTransitionOnInit(videoTrackIndex, filterId);
}

int VideoEffectProcessor::addFilter(int trackIndex, int64_t sequenceIn, int64_t sequenceOut, string filterName) {
	int filterId = -1;
	if (timeLine) {
		char *data;
		int len = filterName.length();
		data = new char[len+1];
		filterName.copy(data, len, 0);
		data[len] = '\0';

		filterId = timeLine->addFilter(trackIndex, sequenceIn, sequenceOut, data);

		delete [] data;
	}
    else
    {
        LOGE("error : timeLine is not init ! ! !");
    }
	return filterId;
}

void VideoEffectProcessor::removeFilter(int trackIndex, int filterId){
	timeLine->removeFilter(trackIndex, filterId);
}

void VideoEffectProcessor::removeAllFilters(){
	timeLine->init();
}

bool VideoEffectProcessor::invokeFilterOnReady(int trackIndex, int filterId){
	return timeLine->invokeFilterOnInit(trackIndex, filterId);
}

void VideoEffectProcessor::setFilterParamValue(int trackIndex, int filterId, string paramName, ParamVal value) {
	if (timeLine) {
		char *data;
		int len = paramName.length();
		data = new char[len + 1];
		paramName.copy(data, len, 0);
		data[len] = '\0';

		timeLine->setFilterParamValue(trackIndex, filterId, data, value);

		delete []data;
	}
}

void VideoEffectProcessor::process(OpenglVideoFrame* sourceVideoFrame, float position, OpenglVideoFrame* targetVideoFrame) {
	// traverse the filter list to render the filters need rendered
	ImagePosition imagePosition;
	targetVideoFrame->GetImagePosition(&imagePosition);
	GLuint sourceTexId = sourceVideoFrame->getTextureId();
	GLuint outputTexId = targetVideoFrame->getTextureId();

	pthread_rwlock_wrlock(&mRwlock);	// lock here, maybe block UI, but safe is more important
	if (mNeedChangeTimeline) {
		mNeedChangeTimeline = false;
		timeLine->clearFilters();
		if (mThemeProtocolFile) {
			readThemeProtocol(mThemeProtocolFile, mFrameWidth, mFrameHeight, mDurationInSec,
					getTextPixelsByPlatforms, getOverlayPixelsByPlatforms, mContext);	// change timeline
		}
	}
	pthread_rwlock_unlock(&mRwlock);

	if (timeLine) {
		list<ModelFilter *> filters = timeLine->getAllTranAndFilters(0);
		list<ModelFilter *>::iterator itor = filters.begin();
		int filterCount = 0;
		for (; itor != filters.end(); itor++) {
			ModelFilter* filter = *itor;
			if (filter->isAvailable(position, detectGPUSurpportedEffectByPlatforms, mContext))
				filterCount++;
		}
		if(filterCount == 0){
			inputVideoFrame->init(sourceTexId, imagePosition);
			outputVideoFrame->init(outputTexId, imagePosition);
			directPassEffect->renderEffect(inputVideoFrame, outputVideoFrame, NULL);
		} else{
			itor = filters.begin();
			int sub = 0;
			GLuint previousTexId = sourceTexId;
			GPUTexture* previousTextureFromCache = NULL;
			for (; itor != filters.end(); itor++) {
				ModelFilter* filter = *itor;
				if (filter->isAvailable(position, detectGPUSurpportedEffectByPlatforms, mContext)) {
					BaseVideoEffect* effect = effectCache->getVideoEffectFromCache(string((*itor)->name));
					if (effect) {
						filter->onRenderPre(position);

						inputVideoFrame->init(previousTexId, imagePosition, position);

						GLuint currentTexId = outputTexId;
						GPUTexture* textureFromCache = NULL;
						if(sub < (filterCount - 1)){
							textureFromCache = GPUTextureCache::GetInstance()->fetchTexture(imagePosition.width, imagePosition.height);
							textureFromCache->lock();
							currentTexId = textureFromCache->getTexId();
						}
						outputVideoFrame->init(currentTexId, imagePosition);

						effect->renderEffect(inputVideoFrame, outputVideoFrame, filter->getFilterCallback());

						previousTexId = currentTexId;
						if(NULL != previousTextureFromCache){
							previousTextureFromCache->unLock();
						}
						previousTextureFromCache = textureFromCache;
					} else {
						LOGE("getVideoEffectFromCache failed");
					}
					sub++;
				}
			}
		}
	}
}

void VideoEffectProcessor::dealloc() {

	// release videoFrame
	if(inputVideoFrame){
		delete inputVideoFrame;
		inputVideoFrame = NULL;
	}
	if(outputVideoFrame){
		delete outputVideoFrame;
		outputVideoFrame = NULL;
	}
	if (timeLine) {
		delete timeLine;
		timeLine = NULL;
	}
	if(NULL != directPassEffect){
		delete directPassEffect;
		directPassEffect = NULL;
	}
	if(NULL != effectCache){
		delete effectCache;
		effectCache = NULL;
	}
	GPUTextureCache::GetInstance()->destroy();
}

