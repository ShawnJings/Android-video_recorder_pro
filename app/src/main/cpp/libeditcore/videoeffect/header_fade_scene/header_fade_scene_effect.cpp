#include "./header_fade_scene_effect.h"

#define LOG_TAG "HeaderFadeSceneEffect"

HeaderFadeSceneEffect::HeaderFadeSceneEffect() {
	mVertexShader = HEADER_FADE_SCENE_VERTEX_SHADER;
	mFragmentShader = HEADER_FADE_SCENE_FRAG_SHADER;
}

HeaderFadeSceneEffect::~HeaderFadeSceneEffect() {
}

bool HeaderFadeSceneEffect::init(){
	BaseVideoEffect::init();
	headerTexSampler = glGetUniformLocation(mGLProgId, "headerTexSampler");
	checkGlError("glGetUniformLocation headerTexSampler");
	uniformLoc_progress = glGetUniformLocation(mGLProgId, "progress");
	checkGlError("glGetUniformLocation uniformLoc_progress");
	return true;
}

/**  Template Method  **/
void HeaderFadeSceneEffect::buildParamDefaultValue() {
	mMapParamDef.clear();

	ParamVal paramValVertexCoords;
	paramValVertexCoords.type = EffectParamTypeArbitrary;
	float _vertexs[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	float * vertexCoords = new float[8];
	memcpy(vertexCoords, _vertexs, sizeof(float) * 8);
	paramValVertexCoords.u.arbData = (void *) vertexCoords;
	mMapParamDef[string(CONTRAST_PARAM_ID_VERTEX_COORDS)] = paramValVertexCoords;

	ParamVal paramValTextureCoords;
	paramValTextureCoords.type = EffectParamTypeArbitrary;
	float _textures[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	float * texCoords = new float[8];
	memcpy(texCoords, _textures, sizeof(float) * 8);
	paramValTextureCoords.u.arbData = (void *) texCoords;
	mMapParamDef[string(CONTRAST_PARAM_ID_TEXTURE_COORDS)] = paramValTextureCoords;
}

void HeaderFadeSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	int headerVideoTexId = -1;
	float progress = 0.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(HEADER_FADE_VIDEO_TEXTURE_ID), val);
		if (suc) {
			headerVideoTexId = val.u.intVal;
//			LOGI("get success, headerVideoTexId:%d", headerVideoTexId);
		} else {
			LOGI("get param value failed, use default value");
		}

		suc = filterCallback->getParamValue(string(HEADER_FADE_VIDEO_FADE_OUT_PROGRESS), val);
		if (suc) {
			progress = val.u.fltVal;
//			LOGI("get success, progress:%.4f", progress);
		} else {
			LOGI("get progress failed, use default value");
		}
	}

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	if(-1 != headerVideoTexId){
		//将Gif画到屏幕的指定位置
		glUseProgram(mGLProgId);

		static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
		glEnableVertexAttribArray(mGLVertexCoords);

		static const GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
		glEnableVertexAttribArray(mGLTextureCoords);

		glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
		/** 开启混合 **/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		this->bindTexture(inputFrame->getTextureId());
		glUniform1i(mGLUniformTexture, 0);
		glActiveTexture(GL_TEXTURE1);
		this->bindTexture(headerVideoTexId);
		glUniform1i(headerTexSampler, 1);
		/** 设置FadeIn的Progress **/
		glUniform1f(uniformLoc_progress, progress);

		this->draw();
		glDisableVertexAttribArray(mGLVertexCoords);
		glDisableVertexAttribArray(mGLTextureCoords);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("HeaderFadeSceneEffect::renderEffect glBindFramebuffer 0 ");
}

void HeaderFadeSceneEffect::destroy() {
	BaseVideoEffect::destroy();

	// must delete arbitrary data
	map<string, ParamVal>::iterator itor;
	itor = mMapParamDef.find(string(CONTRAST_PARAM_ID_VERTEX_COORDS));
	if (itor != mMapParamDef.end()) {
		float * pointer = (float *) ((itor->second).u.arbData);
		delete[] pointer;
	}

	itor = mMapParamDef.find(string(CONTRAST_PARAM_ID_TEXTURE_COORDS));
	if (itor != mMapParamDef.end()) {
		float * pointer = (float *) ((itor->second).u.arbData);
		delete[] pointer;
	}

	mMapParamDef.clear();
}
