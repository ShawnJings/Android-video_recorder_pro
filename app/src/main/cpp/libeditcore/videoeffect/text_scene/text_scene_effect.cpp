#include "./text_scene_effect.h"

#define LOG_TAG "TextSceneEffect"

TextSceneEffect::TextSceneEffect() {
	mVertexShader = TEXT_SCENE_VERTEX_SHADER;
	mFragmentShader = TEXT_SCENE_FRAG_SHADER;
}

TextSceneEffect::~TextSceneEffect() {
}

bool TextSceneEffect::init(){
	BaseVideoEffect::init();
	attr_ver_Progress = glGetAttribLocation(mGLProgId, "progress");
	checkGlError("glGetAttribLocation attr_ver_Progress");
	attr_ver_OverlayTextureCoords = glGetAttribLocation(mGLProgId, "overlay_texcoord");
	checkGlError("glGetAttribLocation attr_ver_OverlayTextureCoords");
	uniformLoc_ver_text_Sampler = glGetUniformLocation(mGLProgId, "overlayTexSampler");
	checkGlError("glGetUniformLocation uniformLoc_ver_text_Sampler");
	return true;
}

/**  Template Method  **/
void TextSceneEffect::buildParamDefaultValue() {
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

void TextSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	int textTexId = -1;
	float progress = 1.0;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(TEXT_SCENE_PARAM_ID_TEXTURE_ID), val);
		if (suc) {
			textTexId = val.u.intVal;
//			LOGI("get success, textTexId:%d", textTexId);
		} else {
			LOGI("get textTexId failed, use default value");
		}
		suc = filterCallback->getParamValue(string(TEXT_SCENE_PARAM_ID_ALPHA_PROGRESS), val);
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

	if(-1 != textTexId){
		//将Gif画到屏幕的指定位置
		glUseProgram(mGLProgId);

		static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
		glEnableVertexAttribArray(mGLVertexCoords);

		static const GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
		glEnableVertexAttribArray(mGLTextureCoords);

		static const GLfloat texCoords1[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(attr_ver_OverlayTextureCoords, 2, GL_FLOAT, 0, 0, texCoords1);
		glEnableVertexAttribArray(attr_ver_OverlayTextureCoords);

		GLfloat overlay_progress[4];
		for (int i = 0; i < 4; i++)
			overlay_progress[i] = progress;

		glVertexAttribPointer(attr_ver_Progress, 1, GL_FLOAT, GL_FALSE, 0, overlay_progress);
		glEnableVertexAttribArray(attr_ver_Progress);
		GL_CHECK_ERROR("TextSceneEffect::renderEffect glEnableVertexAttribArray attr_ver_Progress");

		glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);

		glActiveTexture(GL_TEXTURE1);
		this->bindTexture(inputFrame->getTextureId());
		glUniform1i(mGLUniformTexture, 1);

		glActiveTexture(GL_TEXTURE0);
		this->bindTexture(textTexId);
		glUniform1i(uniformLoc_ver_text_Sampler, 0);

		this->draw();
		glDisableVertexAttribArray(mGLVertexCoords);
		glDisableVertexAttribArray(mGLTextureCoords);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("TextSceneEffect::renderEffect glBindFramebuffer 0 ");
}

void TextSceneEffect::destroy() {
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
