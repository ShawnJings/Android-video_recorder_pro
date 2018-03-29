#include "./transition_scene_effect.h"

#define LOG_TAG "TransitionSceneEffect"

TransitionSceneEffect::TransitionSceneEffect() {
	mVertexShader = TRANSITION_SCENE_VERTEX_SHADER;
	mFragmentShader = TRANSITION_SCENE_FRAG_SHADER;
}

TransitionSceneEffect::~TransitionSceneEffect() {
}

bool TransitionSceneEffect::init(){
	BaseVideoEffect::init();
	attrLoc_dstTexCoord = glGetAttribLocation(mGLProgId, "dstTexCoordAttr");
	checkGlError("TransitionSceneEffect glGetAttribLocation dstTexCoordAttr");
	uniformLoc_dstSampler = glGetUniformLocation(mGLProgId, "dstSampler");
	checkGlError("TransitionSceneEffect glGetUniformLocation dstSampler");
	uniformLoc_progress = glGetUniformLocation(mGLProgId, "progress");
	checkGlError("TransitionSceneEffect glGetUniformLocation progress");
	return true;
}

/**  Template Method  **/
void TransitionSceneEffect::buildParamDefaultValue() {
	mMapParamDef.clear();

	ParamVal paramValVertexCoords;
	paramValVertexCoords.type = EffectParamTypeArbitrary;
	float _vertexs[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
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

void TransitionSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	LOGI("TransitionSceneEffect getParamValue ");
	int fadeInTexId = -1;
	float progress = 0.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(TRANSITION_SCENE_PARAM_ID_TEXTURE_ID), val);
		if (suc) {
			fadeInTexId = val.u.intVal;
			LOGI("get success, fadeInTexId:%d", fadeInTexId);
		} else {
			LOGI("get fadeInTexId failed, use default value");
		}
		suc = filterCallback->getParamValue(string(TRANSITION_SCENE_PARAM_ID_PROGRESS), val);
		if (suc) {
			progress = val.u.fltVal;
			LOGI("get success, progress:%.4f", progress);
		} else {
			LOGI("get progress failed, use default value");
		}
	}
	LOGI("after TransitionSceneEffect getParamValue ");

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);
	this->switchFBOAttachmentTexture(outputFrame->getTextureId());

	if (-1 != fadeInTexId) {
	    glDisable(GL_BLEND);
	    glDisable(GL_DEPTH_TEST);
	    glDisable(GL_CULL_FACE);

		glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
		glUseProgram(mGLProgId);
		static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
		glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
		glEnableVertexAttribArray(mGLVertexCoords);
		static const GLfloat texCoords[] =  { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
		glEnableVertexAttribArray(mGLTextureCoords);
		static const GLfloat dstTexCoords[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		glVertexAttribPointer(attrLoc_dstTexCoord, 2, GL_FLOAT, 0, 0, dstTexCoords);
		glEnableVertexAttribArray(attrLoc_dstTexCoord);

		/** 这个是做淡入的纹理 **/
		glActiveTexture(GL_TEXTURE1);
		this->bindTexture(fadeInTexId);
		glUniform1i(uniformLoc_dstSampler, 1);
		/** 这个是原始的纹理 **/
		glActiveTexture(GL_TEXTURE0);
		this->bindTexture(inputFrame->getTextureId());
		glUniform1i(mGLUniformTexture, 0);
		/** 设置FadeIn的Progress **/
		glUniform1f(uniformLoc_progress, progress);

		this->draw();
		glDisableVertexAttribArray(mGLVertexCoords);
		glDisableVertexAttribArray(mGLTextureCoords);
	    glDisableVertexAttribArray(attrLoc_dstTexCoord);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("TransitionSceneEffect::renderEffect glBindFramebuffer 0 ");
}

void TransitionSceneEffect::destroy() {
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
