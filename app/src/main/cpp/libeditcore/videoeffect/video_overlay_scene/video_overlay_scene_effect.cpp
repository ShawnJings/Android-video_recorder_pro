#include "./video_overlay_scene_effect.h"

#define LOG_TAG "VideoOverlaySceneEffect"

VideoOverlaySceneEffect::VideoOverlaySceneEffect() {
	mVertexShader = VIDEO_OVERLAY_SCENE_VERTEX_SHADER;
	mFragmentShader = VIDEO_OVERLAY_SCENE_FRAG_SHADER;
}

VideoOverlaySceneEffect::~VideoOverlaySceneEffect() {
}

bool VideoOverlaySceneEffect::init(){
	BaseVideoEffect::init();
	blackBoardSampler = glGetUniformLocation(mGLProgId, "inputImageTexture2");
	checkGlError("glGetUniformLocation blackboardSampler");
	overlayMapSampler = glGetUniformLocation(mGLProgId, "inputImageTexture3");
	checkGlError("glGetUniformLocation overlayMapSampler");
	amaroMapSampler = glGetUniformLocation(mGLProgId, "inputImageTexture4");
	checkGlError("glGetUniformLocation amaroMapSampler");
	return true;
}

/**  Template Method  **/
void VideoOverlaySceneEffect::buildParamDefaultValue() {
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

void VideoOverlaySceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	int blackBoardTexTd = -1;
	int overlayMapTexId = -1;
	int amaroMapTexId = -1;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(BLACK_BOARD_TEXTURE_ID), val);
		if (suc) {
			blackBoardTexTd = val.u.intVal;
//			LOGI("get success, blackBoardTexTd:%d", blackBoardTexTd);
		} else {
			LOGI("get param value failed, use default value");
		}
		suc = filterCallback->getParamValue(string(OVERLAY_MAP_TEXTURE_ID), val);
		if (suc) {
			overlayMapTexId = val.u.intVal;
//			LOGI("get success, overlayMapTexId:%d", overlayMapTexId);
		} else {
			LOGI("get param value failed, use default value");
		}
		suc = filterCallback->getParamValue(string(AMARO_MAP_TEXTURE_ID), val);
		if (suc) {
			amaroMapTexId = val.u.intVal;
//			LOGI("get success, amaroMapTexId:%d", amaroMapTexId);
		} else {
			LOGI("get param value failed, use default value");
		}
	}

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());

	if(-1 != blackBoardTexTd && -1 != overlayMapTexId && -1 != amaroMapTexId){
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
		this->bindTexture(blackBoardTexTd);
		glUniform1i(blackBoardSampler, 1);
		glActiveTexture(GL_TEXTURE2);
		this->bindTexture(overlayMapTexId);
		glUniform1i(overlayMapSampler, 2);
		glActiveTexture(GL_TEXTURE3);
		this->bindTexture(amaroMapTexId);
		glUniform1i(amaroMapSampler, 3);
		this->draw();
		glDisableVertexAttribArray(mGLVertexCoords);
		glDisableVertexAttribArray(mGLTextureCoords);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("VideoOverlaySceneEffect::renderEffect glBindFramebuffer 0 ");
}

void VideoOverlaySceneEffect::destroy() {
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
