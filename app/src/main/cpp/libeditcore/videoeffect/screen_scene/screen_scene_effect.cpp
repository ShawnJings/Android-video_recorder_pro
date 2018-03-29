#include "./screen_scene_effect.h"

#define LOG_TAG "ScreenSceneEffect"

ScreenSceneEffect::ScreenSceneEffect() {
	mVertexShader = SCREEN_SCENE_VERTEX_SHADER;
	mFragmentShader = SCREEN_SCENE_FRAG_SHADER;
}

ScreenSceneEffect::~ScreenSceneEffect() {
}

bool ScreenSceneEffect::init(){
	BaseVideoEffect::init();
	uniformLoc_screen_sampler = glGetUniformLocation(mGLProgId, "inputImageTexture2");
	return true;
}

/**  Template Method  **/
void ScreenSceneEffect::buildParamDefaultValue() {
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
	float _textures[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	float * texCoords = new float[8];
	memcpy(texCoords, _textures, sizeof(float) * 8);
	paramValTextureCoords.u.arbData = (void *) texCoords;
	mMapParamDef[string(CONTRAST_PARAM_ID_TEXTURE_COORDS)] = paramValTextureCoords;
}

void ScreenSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	int screenTexId = -1;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(GIF_SCENE_PARAM_ID_TEXTURE_ID), val);
		if (suc) {
			screenTexId = val.u.intVal;
			LOGI("get success, screenTexId:%d", screenTexId);
		} else {
			LOGI("get maskTexId value failed, use default value");
		}
	}

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	if (-1 != screenTexId) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
		glUseProgram(mGLProgId);
		static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
		glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
		glEnableVertexAttribArray(mGLVertexCoords);
		static const GLfloat texCoords[] =  { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
		glEnableVertexAttribArray(mGLTextureCoords);

		glActiveTexture(GL_TEXTURE1);
		this->bindTexture(screenTexId);
		glUniform1i(uniformLoc_screen_sampler, 1);

		glActiveTexture(GL_TEXTURE0);
		this->bindTexture(inputFrame->getTextureId());
		glUniform1i(mGLUniformTexture, 0);

		this->draw();
		glDisableVertexAttribArray(mGLVertexCoords);
		glDisableVertexAttribArray(mGLTextureCoords);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("ScreenSceneEffect::renderEffect glBindFramebuffer 0 ");
}

void ScreenSceneEffect::destroy() {
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
