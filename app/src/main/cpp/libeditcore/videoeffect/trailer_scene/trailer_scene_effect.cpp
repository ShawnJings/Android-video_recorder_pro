#include "./trailer_scene_effect.h"

#define LOG_TAG "TrailerSceneEffect"

TrailerSceneEffect::TrailerSceneEffect() {
	mVertexShader = TRAILER_SCENE_OVERLAY_VERTEX_SHADER;
	mFragmentShader = TRAILER_SCENE_OVERLAY_FRAG_SHADER;
}

TrailerSceneEffect::~TrailerSceneEffect() {
}

bool TrailerSceneEffect::init(){
	/** default build Horizontal Program **/
	BaseVideoEffect::init();
	glUseProgram(mGLProgId);
	attr_overlay_OverlayTextureCoords = glGetAttribLocation(mGLProgId, "overlay_texcoord");
	checkGlError("glGetAttribLocation attr_overlay_OverlayTextureCoords");
	uniformLoc_overlay_Overlay_Texture = glGetUniformLocation(mGLProgId, "overlayTexSampler");
	checkGlError("glGetAttribLocation uniformLoc_overlay_Overlay_Texture");
	uniformLoc_overlay_Progress = glGetUniformLocation(mGLProgId, "progress");
	checkGlError("glGetAttribLocation uniformLoc_overlay_Progress");

	return true;
}

/**  Template Method  **/
void TrailerSceneEffect::buildParamDefaultValue() {
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

void TrailerSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("TrailerSceneEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}
	if (!mIsInitialized) {
		LOGE("TrailerSceneEffect::renderEffect effect not initialized!");
		return;
	}

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

//	LOGI("TrailerSceneEffect getParamValue ");

	int fadeInTexId = -1;
	float progress = 0.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(TRAILER_SCENE_PARAM_ID_TEXTURE_ID), val);
		if (suc) {
			fadeInTexId = val.u.intVal;
//			LOGI("get success, fadeInTexId:%d", fadeInTexId);
		} else {
			LOGI("get fadeInTexId failed, use default value");
		}
		suc = filterCallback->getParamValue(string(TRAILER_SCENE_PARAM_ID_PROGRESS), val);
		if (suc) {
			progress = val.u.fltVal;
//			LOGI("get success, progress:%.4f", progress);
		} else {
			LOGI("get progress failed, use default value");
		}
	}

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());

	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	glUseProgram(mGLProgId);
	static const GLfloat overlayVertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, overlayVertices);
	glEnableVertexAttribArray(mGLVertexCoords);
	static const GLfloat overlayTexCoords[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, overlayTexCoords);
	glEnableVertexAttribArray(mGLTextureCoords);
	static const GLfloat overlayTexCoords2[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(attr_overlay_OverlayTextureCoords, 2, GL_FLOAT, 0, 0, overlayTexCoords);
	glEnableVertexAttribArray(attr_overlay_OverlayTextureCoords);

	glActiveTexture(GL_TEXTURE1);
	this->bindTexture(fadeInTexId);
	glUniform1i(uniformLoc_overlay_Overlay_Texture, 1);
	/** 这个是临时过渡的纹理 **/
	glActiveTexture(GL_TEXTURE0);
	this->bindTexture(inputFrame->getTextureId());
	glUniform1i(mGLUniformTexture, 0);
	/** 设置FadeIn的Progress **/
	glUniform1f(uniformLoc_overlay_Progress, progress);
//	LOGI("set uniformLoc_ver_Progress , progress:%.4f", progress);

	this->draw();
	glDisableVertexAttribArray(mGLVertexCoords);
	glDisableVertexAttribArray(mGLTextureCoords);
	glDisableVertexAttribArray(attr_overlay_OverlayTextureCoords);

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("333 TrailerSceneEffect::renderEffect overlay bindframebuffer 0 ");
}

void TrailerSceneEffect::destroy() {
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
