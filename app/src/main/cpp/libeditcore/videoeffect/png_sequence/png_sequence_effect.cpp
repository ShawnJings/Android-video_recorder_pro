#include "./png_sequence_effect.h"

#define LOG_TAG "PngSequenceEffect"

PngSequenceEffect::PngSequenceEffect() {
	mVertexShader = PNG_SEQUENCE_VERTEX_SHADER;
	mFragmentShader = PNG_SEQUENCE_FRAG_SHADER;
}

PngSequenceEffect::~PngSequenceEffect() {
}

bool PngSequenceEffect::init(){
	BaseVideoEffect::init();
	cpyTextureProgId = loadProgram(VERTEX_SHADER_FILTER, FRAG_SHADER_FILTER);
	if (!cpyTextureProgId) {
		LOGE("Could not create program cpyTextureProgId.");
		return false;
	}
	cpyTexturePosition = glGetAttribLocation(cpyTextureProgId, "position");
	checkGlError("glGetAttribLocation cpyTexturePosition");
	cpyTextureCoords = glGetAttribLocation(cpyTextureProgId, "texcoord");
	checkGlError("glGetAttribLocation cpyTextureCoords");
	cpyTextureUniformSampler = glGetUniformLocation(cpyTextureProgId, "yuvTexSampler");
	checkGlError("glGetUniformLocation cpyTextureUniformSampler");

	mUniformTransforms = glGetUniformLocation(mGLProgId, "trans");
	checkGlError("glGetUniformLocation mUniformTransforms");

	return true;
}

void PngSequenceEffect::buildParamDefaultValue() {
	mMapParamDef.clear();

	ParamVal paramValHFlip;
	paramValHFlip.type = EffectParamTypeBoolean;
	paramValHFlip.u.boolVal = false;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_HFLIP)] = paramValHFlip;

	ParamVal paramValX;
	paramValX.type = EffectParamTypeFloat;
	paramValX.u.fltVal = 0;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_X)] = paramValX;

	ParamVal paramValY;
	paramValY.type = EffectParamTypeFloat;
	paramValY.u.fltVal = 0;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_Y)] = paramValY;

	ParamVal paramValWidth;
	paramValWidth.type = EffectParamTypeFloat;
	paramValWidth.u.fltVal = 0.5;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_WIDTH)] = paramValWidth;

	ParamVal paramValAngle;
	paramValAngle.type = EffectParamTypeFloat;
	paramValAngle.u.fltVal = 45;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_ANGLE)] = paramValAngle;

	ParamVal paramValTexId;
	paramValTexId.type = EffectParamTypeInt;
	paramValTexId.u.intVal = 0;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_TEXTURE_ID)] = paramValTexId;

	ParamVal paramValGifWidth;
	paramValGifWidth.type = EffectParamTypeInt;
	paramValGifWidth.u.intVal = 0;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_PNG_WIDTH)] = paramValGifWidth;

	ParamVal paramValGifHeight;
	paramValGifHeight.type = EffectParamTypeInt;
	paramValGifHeight.u.intVal = 0;
	mMapParamDef[string(PNG_SEQUENCE_PARAM_ID_PNG_HEIGHT)] = paramValGifHeight;
}

//void PngSequenceEffect::enableVertexCoord(float x, float y, float width, int gifWidth, int gifHeight) {
//	float height = width*gifHeight/gifWidth;
//	y = -y;	// 图像本来是倒着的
//	static const GLfloat _vertices[] = { x-width, y-height, x+width, y-height, x-width, y+height,  x+width, y+height };
////	static const GLfloat _vertices[] = { x-1, y-1, x+1, y-1, x-1, y+1,  x+1, y+1 };
//	glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
//	glEnableVertexAttribArray(mGLVertexCoords);
//}

void PngSequenceEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	// get the parameter value needed
	ParamVal paramValHFlip = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_HFLIP), filterCallback);
	ParamVal paramValX = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_X), filterCallback);
	ParamVal paramValY = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_Y), filterCallback);
	ParamVal paramValWidth = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_WIDTH), filterCallback);
	ParamVal paramValAngle = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_ANGLE), filterCallback);
	ParamVal paramValTexId = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_TEXTURE_ID), filterCallback);
	ParamVal paramValGifWidth = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_PNG_WIDTH), filterCallback);
	ParamVal paramValGifHeight = getParamValHelper(string(PNG_SEQUENCE_PARAM_ID_PNG_HEIGHT), filterCallback);

	// texture id bind with FBO
	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	GL_CHECK_ERROR("PngSequenceEffect::renderEffect glFramebufferTexture2D");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//拷贝原始纹理
	glUseProgram(cpyTextureProgId);
	static const GLfloat cpyTexVertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(cpyTexturePosition, 2, GL_FLOAT, 0, 0, cpyTexVertices);
	glEnableVertexAttribArray(cpyTexturePosition);
	static const GLfloat cpyTexCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(cpyTextureCoords, 2, GL_FLOAT, 0, 0, cpyTexCoords);
	glEnableVertexAttribArray(cpyTextureCoords);

	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	glActiveTexture(GL_TEXTURE0);
	this->bindTexture(inputFrame->getTextureId());
	glUniform1i(cpyTextureUniformSampler, 0);
	this->draw();
	glDisableVertexAttribArray(cpyTexturePosition);
	glDisableVertexAttribArray(cpyTextureCoords);
	GL_CHECK_ERROR("PngSequenceEffect::renderEffect glDisableVertexAttribArray");

	int gifTexId = paramValTexId.u.intVal;

	if(0 != gifTexId){
		LOGI("gifTexId is %d", gifTexId);
		glUseProgram(mGLProgId);

		static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
				1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
		glEnableVertexAttribArray(mGLVertexCoords);

		// hflip
		if (paramValHFlip.u.boolVal) {
			static const GLfloat texCoords[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
			glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
			glEnableVertexAttribArray(mGLTextureCoords);
		} else {
			static const GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
			glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
			glEnableVertexAttribArray(mGLTextureCoords);
		}

		float scaleMatrix[4 * 4];
		matrixSetIdentityM(scaleMatrix);
		float height = paramValWidth.u.fltVal * paramValGifHeight.u.intVal
				/ paramValGifWidth.u.intVal;
		matrixScaleM(scaleMatrix, paramValWidth.u.fltVal, height, 1.0f);

		float rotateMatrix[4 * 4];
		matrixSetIdentityM(rotateMatrix);
		matrixRotateM(rotateMatrix, paramValAngle.u.fltVal, 0, 0, 1);

		float translateMatrix[4 * 4];
		matrixSetIdentityM(translateMatrix);
		matrixTranslateM(translateMatrix, paramValX.u.fltVal,
				-1 * paramValY.u.fltVal, 0.0f);

		float transformMatrix[4 * 4];
		matrixMultiplyMM(transformMatrix, translateMatrix, rotateMatrix);

		float transformMatrix1[4 * 4];
		matrixMultiplyMM(transformMatrix1, transformMatrix, scaleMatrix);

		glUniformMatrix4fv(mUniformTransforms, 1, GL_FALSE,
				(GLfloat *) transformMatrix1);

		glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
		/** 开启混合 **/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		this->bindTexture(gifTexId);
		glUniform1i(mGLUniformTexture, 0);
		this->draw();
		glDisableVertexAttribArray(mGLVertexCoords);
		glDisableVertexAttribArray(mGLTextureCoords);

		GL_CHECK_ERROR("PngSequenceEffect::renderEffect glDisableVertexAttribArray mGLTextureCoords");
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("PngSequenceEffect::renderEffect glBindFramebuffer 0 ");
}

void PngSequenceEffect::destroy() {
	BaseVideoEffect::destroy();

	glDeleteProgram(cpyTextureProgId);

	mMapParamDef.clear();
}
