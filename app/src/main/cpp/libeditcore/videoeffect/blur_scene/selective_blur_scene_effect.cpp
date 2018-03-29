#include "./selective_blur_scene_effect.h"

#define LOG_TAG "SelectiveBlurSceneEffect"

SelectiveBlurSceneEffect::SelectiveBlurSceneEffect() {
	blurTexCache = NULL;
	blurVideoFrame = NULL;
}

SelectiveBlurSceneEffect::~SelectiveBlurSceneEffect() {
}

bool SelectiveBlurSceneEffect::init() {
	LOGI("before SelectiveBlurSceneEffect::init");
	if (BlurSceneEffect::init()) {
		blurTexCache = NULL;
		blurVideoFrame = NULL;
		/** build Selective Blur Program **/
		selectiveProgId = loadProgram(SELECTIVE_BLUR_SCENE_VERTEX_SHADER, SELECTIVE_BLUR_SCENE_FRAG_SHADER);
		if (!selectiveProgId) {
			LOGE("Could not create program selectiveBlur Scene selectiveProgId.");
			return false;
		}
		glUseProgram(selectiveProgId);
		selectiveVertexCoords = glGetAttribLocation(selectiveProgId, "position");
		checkGlError("glGetAttribLocation selectiveVertexCoords");
		selectiveSharpTextureCoords = glGetAttribLocation(selectiveProgId, "texcoord");
		checkGlError("glGetAttribLocation selectiveSharpTextureCoords");
		selectiveBlurTextureCoords = glGetAttribLocation(selectiveProgId, "texcoord2");
		checkGlError("glGetAttribLocation selectiveBlurTextureCoords");
		selectiveUniformSharpTexture = glGetUniformLocation(selectiveProgId, "yuvTexSampler");
		checkGlError("glGetUniformLocation selectiveUniformSharpTexture");
		selectiveUniformBlurTexture = glGetUniformLocation(selectiveProgId, "yuvTexSampler2");
		checkGlError("glGetUniformLocation selectiveUniformSharpTexture");
		selectiveUniformExcludeCircleRadius = glGetUniformLocation(selectiveProgId, "excludeCircleRadius");
		checkGlError("glGetUniformLocation selectiveUniformExcludeCircleRadius");
		selectiveUniformExcludeCirclePoint = glGetUniformLocation(selectiveProgId, "excludeCirclePoint");
		checkGlError("glGetUniformLocation selectiveUniformExcludeCirclePoint");
		selectiveUniformExcludeBlurSize = glGetUniformLocation(selectiveProgId, "excludeBlurSize");
		checkGlError("glGetUniformLocation selectiveUniformExcludeBlurSize");
		selectiveUniformAspectRatio = glGetUniformLocation(selectiveProgId, "aspectRatio");
		checkGlError("glGetUniformLocation selectiveUniformAspectRatio");

		LOGI("leave SelectiveBlurSceneEffect::init");
		return true;
	} else {
		return false;
	}
}

void SelectiveBlurSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("SelectiveBlurSceneEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}
	if (!mIsInitialized) {
		LOGE("SelectiveBlurSceneEffect::renderEffect effect not initialized!");
		return;
	}
	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);
	if(NULL == blurTexCache){
		blurTexCache = GPUTextureCache::GetInstance()->fetchTexture(imgPos.width, imgPos.height);
		blurTexCache->lock();
		blurVideoFrame = new OpenglVideoFrame();
		blurVideoFrame->init(blurTexCache->getTexId(), imgPos);
	}
	LOGI("before BlurSceneEffect::renderEffect...");
	BlurSceneEffect::renderEffect(inputFrame, blurVideoFrame, filterCallback);
	LOGI("after BlurSceneEffect::renderEffect...");

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	LOGI("glUseProgram(selectiveProgId)");
	glUseProgram(selectiveProgId);
	LOGI("selectiveVertex...");
	static const GLfloat selectiveVertex[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(selectiveVertexCoords, 2, GL_FLOAT, 0, 0, selectiveVertex);
	glEnableVertexAttribArray(selectiveVertexCoords);
	LOGI("sharpTexCoords...");
	static const GLfloat sharpTexCoords[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(selectiveSharpTextureCoords, 2, GL_FLOAT, 0, 0, sharpTexCoords);
	glEnableVertexAttribArray(selectiveSharpTextureCoords);
	LOGI("blurTexCoords...");
	static const GLfloat blurTexCoords[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(selectiveBlurTextureCoords, 2, GL_FLOAT, 0, 0, blurTexCoords);
	glEnableVertexAttribArray(selectiveBlurTextureCoords);

	LOGI("begin uniform setting...");
	/** 这个是临时过渡的纹理 **/
	glActiveTexture(GL_TEXTURE0);
	this->bindTexture(inputFrame->getTextureId());
	glUniform1i(selectiveUniformSharpTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	this->bindTexture(blurTexCache->getTexId());
	glUniform1i(selectiveUniformBlurTexture, 1);
	/** 设置Uniform的各种变量 **/
	float excludeCircleRadius = 0.5176f;
	float excludeBlurSize = 0.09375f;
	float aspectRatio = 1.0f;
	GLfloat excludeCirclePoint[2] = {0.5f, 0.5f};
	glUniform1f(selectiveUniformExcludeCircleRadius, excludeCircleRadius);
    glUniform2fv(selectiveUniformExcludeCirclePoint, 1, excludeCirclePoint);
	glUniform1f(selectiveUniformExcludeBlurSize, excludeBlurSize);
	glUniform1f(selectiveUniformAspectRatio, aspectRatio);

	this->draw();

	glDisableVertexAttribArray(selectiveVertexCoords);
	glDisableVertexAttribArray(selectiveBlurTextureCoords);
	glDisableVertexAttribArray(selectiveSharpTextureCoords);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("2: SelectiveBlurSceneEffect::renderEffect vertical blur bindframebuffer 0");
}

void SelectiveBlurSceneEffect::destroy() {
	BlurSceneEffect::destroy();
	glDeleteProgram(selectiveProgId);
	if(NULL != blurTexCache){
		blurTexCache->unLock();
	}
	if(blurVideoFrame){
		delete blurVideoFrame;
		blurVideoFrame = NULL;
	}
}
