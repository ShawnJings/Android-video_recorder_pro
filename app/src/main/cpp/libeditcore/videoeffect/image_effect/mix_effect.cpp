#include "./mix_effect.h"

#define LOG_TAG "MixEffect"

MixEffect::MixEffect() {
	mVertexShader = MIX_EFFECT_VERTEX_SHADER;
	mFragmentShader = MIX_EFFECT_FRAG_SHADER;
}

MixEffect::~MixEffect() {
}

bool MixEffect::init() {
	BaseVideoEffect::init();
	attrLoc_dstTexCoord = glGetAttribLocation(mGLProgId, "dstTexCoordAttr");
	checkGlError("MixEffect glGetAttribLocation dstTexCoordAttr");
	uniformLoc_dstSampler = glGetUniformLocation(mGLProgId, "dstSampler");
	checkGlError("MixEffect glGetUniformLocation dstSampler");
	uniformLoc_progress = glGetUniformLocation(mGLProgId, "progress");
	checkGlError("MixEffect glGetUniformLocation progress");

	return true;
}

void MixEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}


	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	int mixTextureId = -1;
	float progress = PROCESSOR_MIX_EFFECT_MIX_PROGRESS;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(MIX_FILTER_TEXTURE_ID), val);
		if (suc) {
			mixTextureId = val.u.intVal;
			LOGI("get success, mixTextureId:%d", mixTextureId);
		} else {
			LOGI("get textTexId failed, use default value");
		}
		suc = filterCallback->getParamValue(string(MIX_FILTER_MIX_PROGRESS), val);
		if (suc) {
			progress = val.u.fltVal;
			LOGI("get success, progress:%.4f", progress);
		} else {
			LOGI("get progress failed, use default value progress is %.2f", progress);
		}
	}

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if (-1 != mixTextureId) {
		//将Gif画到屏幕的指定位置
		glUseProgram(mGLProgId);

		static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
		glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
		glEnableVertexAttribArray(mGLVertexCoords);

		static const GLfloat texCoords[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
		glEnableVertexAttribArray(mGLTextureCoords);

		static const GLfloat dstTexCoords[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		glVertexAttribPointer(attrLoc_dstTexCoord, 2, GL_FLOAT, 0, 0, dstTexCoords);
		glEnableVertexAttribArray(attrLoc_dstTexCoord);

		/** 这个是做淡入的纹理 **/
		glActiveTexture (GL_TEXTURE1);
		this->bindTexture(mixTextureId);
		glUniform1i(uniformLoc_dstSampler, 1);

		/** 这个是原始的纹理 **/
		glActiveTexture (GL_TEXTURE0);
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
	GL_CHECK_ERROR("ImageMixEffect::renderEffect glBindFramebuffer 0 ");
}
