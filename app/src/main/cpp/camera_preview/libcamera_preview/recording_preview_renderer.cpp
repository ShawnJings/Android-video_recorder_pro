#include "recording_preview_renderer.h"

#define LOG_TAG "RecordingPreviewRenderer"

RecordingPreviewRenderer::RecordingPreviewRenderer() {
	degress = 270;
	cameraTexFrame = NULL;
	mCopier = NULL;
	textureCoords = NULL;
	textureCoordsSize = 8;
	mProcessor = NULL;
	mRenderer = NULL;
}

RecordingPreviewRenderer::~RecordingPreviewRenderer() {
}

void RecordingPreviewRenderer::setDegress(int degress, bool isVFlip) {
	this->degress = degress;
	this->isVFlip = isVFlip;
	this->fillTextureCoords();
}

void RecordingPreviewRenderer::init(int degress, bool isVFlip, int textureWidth, int textureHeight, int cameraWidth, int cameraHeight) {
	LOGI("enter RecordingPreviewRenderer::init() textureWidth is %d, textureHeight is %d", textureWidth, textureHeight);
	this->degress = degress;
	this->isVFlip = isVFlip;
	textureCoords = new GLfloat[textureCoordsSize];
	this->fillTextureCoords();
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;
	this->cameraWidth = cameraWidth;
	this->cameraHeight = cameraHeight;

	mCopier = new GPUTextureFrameCopier();
	mCopier->init();
	mProcessor = new VideoEffectProcessor();
	mProcessor->init();
	mRenderer = new VideoGLSurfaceRender();
	mRenderer->init(textureWidth, textureHeight);
	cameraTexFrame = new GPUTextureFrame();
	cameraTexFrame->createTexture();
	glGenTextures(1, &inputTexId);
	checkGlError("glGenTextures inputTexId");
	glBindTexture(GL_TEXTURE_2D, inputTexId);
	checkGlError("glBindTexture inputTexId");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLint internalFormat = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei) textureWidth, (GLsizei) textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &outputTexId);
	checkGlError("glGenTextures outputTexId");
	glBindTexture(GL_TEXTURE_2D, outputTexId);
	checkGlError("glBindTexture outputTexId");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei) textureWidth, (GLsizei) textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &FBO);
	checkGlError("glGenFramebuffers");

	glGenTextures(1, &rotateTexId);
	checkGlError("glGenTextures rotateTexId");
	glBindTexture(GL_TEXTURE_2D, rotateTexId);
	checkGlError("glBindTexture rotateTexId");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (degress == 90 || degress == 270)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cameraHeight, cameraWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cameraWidth, cameraHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//初始化sourceVideoFrame以及targetVideoFrame
	ImagePosition imagePosition(0, 0, GLsizei(textureWidth), GLsizei(textureHeight));
	sourceVideoFrame = new OpenglVideoFrame(inputTexId, imagePosition);
	targetVideoFrame = new OpenglVideoFrame(outputTexId, imagePosition);

	mixFilterId = -1;
	glGenTextures(1, &pausedTexId);
	checkGlError("glGenTextures pausedTexId");
	glBindTexture(GL_TEXTURE_2D, pausedTexId);
	checkGlError("glBindTexture pausedTexId");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei) textureWidth, (GLsizei) textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->setFilter(0, NULL, 0);

	LOGI("leave RecordingPreviewRenderer::init()");
}

bool RecordingPreviewRenderer::setFilter(int filterType, byte* mACVBuffer, int mACVBufferSize) {
	bool ret = true;
	mProcessor->removeAllFilters();
	int filterId = this->addFilter(filterType, mACVBuffer, mACVBufferSize);
	LOGI("add Filter %d", filterId);
	if(filterId >= 0){
		ret = mProcessor->invokeFilterOnReady(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId);
	}
	return ret;
}

void RecordingPreviewRenderer::processFrame(float position) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	checkGlError("glBindFramebuffer FBO");

	if (degress == 90 || degress == 270)
		glViewport(0, 0, cameraHeight, cameraWidth);
	else
		glViewport(0, 0, cameraWidth, cameraHeight);

	GLfloat* vertexCoords = this->getVertexCoords();
	mCopier->renderWithCoords(cameraTexFrame, rotateTexId, vertexCoords, textureCoords);

	int rotateTexWidth = cameraWidth;
	int rotateTexHeight = cameraHeight;
	if (degress == 90 || degress == 270){
		rotateTexWidth = cameraHeight;
		rotateTexHeight = cameraWidth;
	}
	mRenderer->renderToAutoFitTexture(rotateTexId, rotateTexWidth, rotateTexHeight, inputTexId);
	mProcessor->process(sourceVideoFrame, PREVIEW_FILTER_POSITION, targetVideoFrame);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RecordingPreviewRenderer::drawToView(int videoWidth, int videoHeight) {
	mRenderer->renderToView(outputTexId, videoWidth, videoHeight);
}

void RecordingPreviewRenderer::drawToViewWithAutofit(int videoWidth, int videoHeight, int texWidth, int texHeight) {
	mRenderer->renderToViewWithAutofit(outputTexId, videoWidth, videoHeight, texWidth, texHeight);
}

void RecordingPreviewRenderer::releasePausedState(){
	if(mixFilterId >= 0){
		mProcessor->removeFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, mixFilterId);
		mixFilterId = -1;
	}
}

int RecordingPreviewRenderer::getCameraTexId() {
	if (cameraTexFrame) {
		return cameraTexFrame->getDecodeTexId();
	}
	return -1;
}

bool RecordingPreviewRenderer::preparePausedState(){
	LOGI("enter RecordingPreviewRenderer::copyPausedTexture...");
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	checkGlError("glBindFramebuffer FBO");
	mRenderer->renderToTexture(outputTexId, pausedTexId);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mixFilterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, MIX_FILTER_NAME);
	if(mixFilterId >= 0){
		ParamVal mixTexIdValue;
		mixTexIdValue.u.intVal = pausedTexId;
		mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, mixFilterId, MIX_FILTER_TEXTURE_ID, mixTexIdValue);
		ParamVal mixProgressValue;
		mixProgressValue.u.fltVal = 0.5f;
		mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, mixFilterId, MIX_FILTER_MIX_PROGRESS, mixProgressValue);
		return mProcessor->invokeFilterOnReady(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, mixFilterId);
	}
	LOGI("leave RecordingPreviewRenderer::copyPausedTexture...");
	return false;
}

void RecordingPreviewRenderer::dealloc(){
	LOGI("enter RecordingPreviewRenderer::dealloc()");
	if(mCopier){
		mCopier->destroy();
		delete mCopier;
		mCopier = NULL;
	}
	LOGI("after delete mCopier");
	if(mProcessor){
		mProcessor->dealloc();
		delete mProcessor;
		mProcessor = NULL;
	}
	LOGI("after delete mProcessor");
	if(mRenderer) {
		LOGI("delete mRenderer..");
		mRenderer->dealloc();
		delete mRenderer;
		mRenderer = NULL;
	}
	LOGI("after delete mRenderer");
	if (inputTexId) {
		LOGI("delete inputTexId ..");
		glDeleteTextures(1, &inputTexId);
	}
	if(outputTexId){
		LOGI("delete outputTexId ..");
		glDeleteTextures(1, &outputTexId);
	}
	if(pausedTexId){
		LOGI("delete pausedTexId..");
		glDeleteTextures(1, &pausedTexId);
	}
	if(FBO){
		LOGI("delete FBO..");
		glDeleteBuffers(1, &FBO);
	}
	if (NULL != targetVideoFrame) {
		delete targetVideoFrame;
		targetVideoFrame = NULL;
	}
	if (NULL != sourceVideoFrame) {
		delete sourceVideoFrame;
		sourceVideoFrame = NULL;
	}

	if (cameraTexFrame != NULL){
		cameraTexFrame->dealloc();
		delete cameraTexFrame;
		cameraTexFrame = NULL;
	}

	if(textureCoords){
		delete[] textureCoords;
		textureCoords = NULL;
	}
	LOGI("leave RecordingPreviewRenderer::dealloc()");
}

int RecordingPreviewRenderer::addFilter(int filterType, byte* mACVBuffer, int mACVBufferSize) {
	LOGI("MVRecordingPreviewController::getFilter type is %d", filterType);
	int filterId = -1;
	switch (filterType) {
	case 10000:
		//清凉
        filterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, BEAUTIFY_FACE_COOL_FILTER_NAME);
		this->setBeautifyFaceFilterValue(filterId, 0.7, 1.0, 0.25, 359.0, 0.5, 0.9f);
        this->setToneCurveFilterValue(filterId, mACVBuffer, mACVBufferSize);
		this->setFilterZoomRatioValue(filterId);
		break;
	case 10001:
		//瘦脸
        filterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, THIN_BEAUTIFY_FACE_FILTER_NAME);
        this->setBeautifyFaceFilterValue(filterId, 0.7, 1.0, 0.25, 359.0, 0.5, 0.9f);
        	this->setFilterZoomRatioValue(filterId);
		break;
	case 10003:
		//自然
        filterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, BEAUTIFY_FACE_FILTER_NAME);
        this->setBeautifyFaceFilterValue(filterId, 0.7, 1.0, 0.25, 359.0, 0.5, 0.9f);
		this->setFilterZoomRatioValue(filterId);
		break;
	case 10004:
		//美肤
        filterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, BEAUTIFY_FACE_FILTER_NAME);
        this->setBeautifyFaceFilterValue(filterId, 0.65, 0.9, 0.2, 359.0, 0.5, 0.875f);
		this->setFilterZoomRatioValue(filterId);
		break;
	case 10002:
	default:
		//无
//        filterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, BEAUTIFY_FACE_FILTER_NAME);
//        this->setBeautifyFaceFilterValue(filterId, 0.85, 1.1, 0.2, 0.0, 0.5, 0.9f);
//        	this->setFilterZoomRatioValue(filterId);
        	filterId = mProcessor->addFilter(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, PREVIEW_FILTER_SEQUENCE_IN, PREVIEW_FILTER_SEQUENCE_OUT, IMAGE_BASE_EFFECT_NAME);
    		this->setFilterZoomRatioValue(filterId);
		break;
	}
	return filterId;
}

void RecordingPreviewRenderer::setToneCurveFilterValue(int filterId, byte* mACVBuffer, int mACVBufferSize) {
	ParamVal changeFlagValue;
	changeFlagValue.u.boolVal = true;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, TONE_CURVE_FILTER_ACV_BUFFER_CHANGED, changeFlagValue);
	ParamVal acvBufferValue;
	acvBufferValue.u.arbData = mACVBuffer;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, TONE_CURVE_FILTER_ACV_BUFFER, acvBufferValue);
	ParamVal acvBufferSizeValue;
	acvBufferSizeValue.u.intVal = mACVBufferSize;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, TONE_CURVE_FILTER_ACV_BUFFER_SIZE, acvBufferSizeValue);
}

void RecordingPreviewRenderer::setBeautifyFaceFilterValue(int filterId, float maskCurveProgress, float softLightProgress,
		float sCurveProgress, float hueAngle, float sharpness, float satuRatio) {
    ParamVal textureWidthValue;
    textureWidthValue.u.intVal = textureWidth;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_FILTER_TEXTURE_WIDTH, textureWidthValue);
    ParamVal textureHeightValue;
    textureHeightValue.u.intVal = textureHeight;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_FILTER_TEXTURE_HEIGHT, textureHeightValue);
    ParamVal smoothSkinEffectParamChangedValue;
    smoothSkinEffectParamChangedValue.u.boolVal = true;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, SMOOTH_SKIN_EFFECT_PARAM_CHANGED, smoothSkinEffectParamChangedValue);
    ParamVal maskCurveProgressValue;
    maskCurveProgressValue.u.fltVal = maskCurveProgress;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, MASK_CURVE_PROGRESS, maskCurveProgressValue);
    ParamVal softLightProgressValue;
    softLightProgressValue.u.fltVal = softLightProgress;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, SOFT_LIGHT_PROGRESS, softLightProgressValue);
    ParamVal sCurveProgressValue;
    sCurveProgressValue.u.fltVal = sCurveProgress;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, S_CURVE_PROGRESS, sCurveProgressValue);
    ParamVal satuRatioValue;
    satuRatioValue.u.fltVal = satuRatio;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, SATU_RATIO, satuRatioValue);
    ParamVal hueAngleChangedValue;
    hueAngleChangedValue.u.boolVal = true;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, HUE_EFFECT_HUE_ANGLE_CHANGED, hueAngleChangedValue);
    ParamVal hueAngleValue;
    hueAngleValue.u.fltVal = hueAngle;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, HUE_EFFECT_HUE_ANGLE, hueAngleValue);
    ParamVal sharpnessValue;
    sharpnessValue.u.fltVal = sharpness;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, SHARPEN_EFFECT_SHARPNESS, sharpnessValue);
    ParamVal groupEffectWidthValue;
    groupEffectWidthValue.u.intVal = textureWidth;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, IMAGE_EFFECT_GROUP_TEXTURE_WIDTH, groupEffectWidthValue);
    ParamVal groupEffectHeightValue;
    groupEffectHeightValue.u.intVal = textureHeight;
    mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, IMAGE_EFFECT_GROUP_TEXTURE_HEIGHT, groupEffectHeightValue);
}

void RecordingPreviewRenderer::setWhiteningFilterValue(int filterId,
		float amplitude, float edger, float radiusFactor,
		float softLightBlendR, float softLightBlendG, float softLightBlendB, float softLightBlendA,
		float hueAngle) {
	ParamVal textureWidthValue;
	textureWidthValue.u.intVal = textureWidth;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_FILTER_TEXTURE_WIDTH, textureWidthValue);
	ParamVal textureHeightValue;
	textureHeightValue.u.intVal = textureHeight;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_FILTER_TEXTURE_HEIGHT, textureHeightValue);
	ParamVal amplitudeValue;
	amplitudeValue.u.fltVal = amplitude;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_FILTER_AMPLITUDE, amplitudeValue);
	ParamVal edgerValue;
	edgerValue.u.fltVal = edger;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_FILTER_EDGER, edgerValue);
	ParamVal radiusFactorValue;
	radiusFactorValue.u.fltVal = radiusFactor;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_BILATERAL_RADIUS_FACTOR, radiusFactorValue);
	ParamVal softLightBlendRValue;
	softLightBlendRValue.u.fltVal = softLightBlendR;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_SOFT_LIGHT_BLEND_R, softLightBlendRValue);
	ParamVal softLightBlendGValue;
	softLightBlendGValue.u.fltVal = softLightBlendG;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_SOFT_LIGHT_BLEND_G, softLightBlendGValue);
	ParamVal softLightBlendBValue;
	softLightBlendBValue.u.fltVal = softLightBlendB;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_SOFT_LIGHT_BLEND_B, softLightBlendBValue);
	ParamVal softLightBlendAValue;
	softLightBlendAValue.u.fltVal = softLightBlendA;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, WHITENING_SOFT_LIGHT_BLEND_A, softLightBlendAValue);
	ParamVal hueAngleChangedValue;
	hueAngleChangedValue.u.boolVal = true;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, HUE_EFFECT_HUE_ANGLE_CHANGED, hueAngleChangedValue);
	ParamVal hueAngleValue;
	hueAngleValue.u.fltVal = hueAngle;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, HUE_EFFECT_HUE_ANGLE, hueAngleValue);
	ParamVal groupEffectWidthValue;
	groupEffectWidthValue.u.intVal = textureWidth;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, IMAGE_EFFECT_GROUP_TEXTURE_WIDTH, groupEffectWidthValue);
	ParamVal groupEffectHeightValue;
	groupEffectHeightValue.u.intVal = textureHeight;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, IMAGE_EFFECT_GROUP_TEXTURE_HEIGHT, groupEffectHeightValue);
}


void RecordingPreviewRenderer::setFilterZoomRatioValue(int filterId) {
	ParamVal zoomRatioValue;
	zoomRatioValue.u.fltVal = 0.95;
	mProcessor->setFilterParamValue(EFFECT_PROCESSOR_VIDEO_TRACK_INDEX, filterId, IMAGE_EFFECT_ZOOM_VIEW_RATIO, zoomRatioValue);
}

void RecordingPreviewRenderer::fillTextureCoords() {
	GLfloat* tempTextureCoords;
	switch (degress) {
	case 90:
		tempTextureCoords = CAMERA_TEXTURE_ROTATED_90;
		break;
	case 180:
		tempTextureCoords = CAMERA_TEXTURE_ROTATED_180;
		break;
	case 270:
		tempTextureCoords = CAMERA_TEXTURE_ROTATED_270;
		break;
	case 0:
	default:
		tempTextureCoords = CAMERA_TEXTURE_NO_ROTATION;
		break;
	}
	memcpy(textureCoords, tempTextureCoords, textureCoordsSize * sizeof(GLfloat));
	if(isVFlip){
		textureCoords[1] = flip(textureCoords[1]);
		textureCoords[3] = flip(textureCoords[3]);
		textureCoords[5] = flip(textureCoords[5]);
		textureCoords[7] = flip(textureCoords[7]);
	}
}

float RecordingPreviewRenderer::flip(float i) {
	if (i == 0.0f) {
		return 1.0f;
	}
	return 0.0f;
}

GLfloat* RecordingPreviewRenderer::getVertexCoords() {
	return CAMERA_TRIANGLE_VERTICES;
}
