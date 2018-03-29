#include "image_effect_group.h"

#define LOG_TAG "ImageEffectGroup"

ImageEffectGroup::ImageEffectGroup() {
	mFrameBufferTextures = NULL;
	mGLVertexBuffer = NULL;
	mGLTextureBuffer = NULL;
	mGLTextureFlipBuffer = NULL;
	isInitFrameBufferFlag = false;
	mFilters = new std::list<BaseVideoEffect*>();
}

ImageEffectGroup::~ImageEffectGroup() {
	if(NULL != mFilters){
		mFilters->clear();
		delete mFilters;
		mFilters = NULL;
	}
}

void ImageEffectGroup::addFilter(BaseVideoEffect* effect) {
	if(NULL != mFilters){
		mFilters->push_back(effect);
	}
}

bool ImageEffectGroup::init() {
	BaseVideoEffect::init();
	std::list<BaseVideoEffect*>::iterator ite = mFilters->begin();
	for (; ite != mFilters->end(); ++ite) {
		BaseVideoEffect* filter = *ite;
		filter->init();
	}
	static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
	mGLVertexBuffer = new GLfloat[8];
	for(int i = 0; i < 8; i++){
		mGLVertexBuffer[i] = _vertices[i];
	}
	static const GLfloat _texCoords[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	mGLTextureBuffer = new GLfloat[8];
	for(int i = 0; i < 8; i++){
		mGLTextureBuffer[i] = _texCoords[i];
	}
	static const GLfloat _texFlipCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	mGLTextureFlipBuffer = new GLfloat[8];
	for(int i = 0; i < 8; i++){
		mGLTextureFlipBuffer[i] = _texFlipCoords[i];
	}
	inputVideoFrame = new OpenglVideoFrame();
	outputVideoFrame = new OpenglVideoFrame();
 

	return true;
}

void ImageEffectGroup::initFramebuffers(EffectCallback * filterCallback) {
	int width = 480;
	int height = 480;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(IMAGE_EFFECT_GROUP_TEXTURE_WIDTH), val);
		if (suc) {
			width = val.u.intVal;
			LOGI("get success, width:%d", width);
		} else {
			LOGI("get width failed, use default value");
		}
		suc = filterCallback->getParamValue(string(IMAGE_EFFECT_GROUP_TEXTURE_HEIGHT), val);
		if (suc) {
			height = val.u.intVal;
			LOGI("get height success, progress:%d", height);
		} else {
			LOGI("get height failed, use default value progress is %d", height);
		}
	}

	size_t size = mFilters->size();
    mFrameBufferTextures = new GLuint[size - 1];
	for (int i = 0; i < (size - 1); i++) {
		GLuint mFrameBufferTexture;
		glGenTextures(1, &mFrameBufferTexture);
		checkGlError("glGenTextures mFrameBufferTexture");
		glBindTexture(GL_TEXTURE_2D, mFrameBufferTexture);
		checkGlError("glBindTexture mFrameBufferTexture");
		// Specify texture
//		LOGI("gen mFrameBufferTexture %d width is %d; height is %d", mFrameBufferTexture, width, height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
		mFrameBufferTextures[i] = mFrameBufferTexture;
	}
}

void ImageEffectGroup::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords){
	if(!isInitFrameBufferFlag){
		this->initFramebuffers(filterCallback);
		isInitFrameBufferFlag = true;
	}

	ImagePosition imagePosition;
	outputFrame->GetImagePosition(&imagePosition);
    float position = inputFrame->getStreamPosition();
    
    size_t size = mFilters->size();
    int previousTexture = inputFrame->getTextureId();
	std::list<BaseVideoEffect*>::iterator ite = mFilters->begin();
	int i = 0;
	for (; ite != mFilters->end(); ++ite) {
		BaseVideoEffect* filter = *ite;
		bool isNotLast = i < size - 1;
		if (isNotLast) {
			outputVideoFrame->init(mFrameBufferTextures[i], imagePosition);
		} else{
			outputVideoFrame->init(outputFrame->getTextureId(), imagePosition);
		}
		inputVideoFrame->init(previousTexture, imagePosition, position);
		if (i == 0) {
			filter->renderEffect(inputVideoFrame, outputVideoFrame, filterCallback, vertexCoords, textureCoords);
		} else if (i == size - 1) {
			filter->renderEffect(inputVideoFrame, outputVideoFrame, filterCallback, mGLVertexBuffer, (size % 2 == 0) ? mGLTextureFlipBuffer : mGLTextureBuffer);
		} else {
			filter->renderEffect(inputVideoFrame, outputVideoFrame, filterCallback, mGLVertexBuffer, mGLTextureBuffer);
		}
		if (isNotLast) {
			previousTexture = mFrameBufferTextures[i];
		}
		i++;
	}

}

void ImageEffectGroup::destroy() {
	if(isInitFrameBufferFlag){
		this->destroyFramebuffers();
	}
	if(NULL != mGLVertexBuffer){
		delete[] mGLVertexBuffer;
		mGLVertexBuffer = NULL;
	}
	if(NULL != mGLTextureBuffer){
		delete[] mGLTextureBuffer;
		mGLTextureBuffer = NULL;
	}
	if(NULL != mGLTextureFlipBuffer){
		delete[] mGLTextureFlipBuffer;
		mGLTextureFlipBuffer = NULL;
	}
	if(NULL != mFilters){
		std::list<BaseVideoEffect*>::iterator ite = mFilters->begin();
		for (; ite != mFilters->end(); ++ite) {
			BaseVideoEffect* filter = *ite;
			filter->destroy();
            delete filter;
		}
	}
	if(inputVideoFrame){
		delete inputVideoFrame;
		inputVideoFrame = NULL;
	}
	if(outputVideoFrame){
		delete outputVideoFrame;
		outputVideoFrame = NULL;
	}
	BaseVideoEffect::destroy();
}

void ImageEffectGroup::destroyFramebuffers() {
	if (NULL != mFrameBufferTextures) {
		glDeleteTextures(1, mFrameBufferTextures);
		delete[] mFrameBufferTextures;
		mFrameBufferTextures = NULL;
	}
}
