#include "base_video_effect.h"

#define LOG_TAG "BaseVideoEffect"

BaseVideoEffect::BaseVideoEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = FRAG_SHADER_FILTER;
}

BaseVideoEffect::~BaseVideoEffect() {
}

void BaseVideoEffect::checkGlError(const char* op){
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGE("after %s() glError (0x%x)\n", op, error);
	}
}

bool BaseVideoEffect::init() {
	mGLProgId = loadProgram(mVertexShader, mFragmentShader);
	if (!mGLProgId) {
		LOGE("Could not create program.");
		return false;
	}
	mGLVertexCoords = glGetAttribLocation(mGLProgId, "position");
	checkGlError("glGetAttribLocation vPosition");
	mGLTextureCoords = glGetAttribLocation(mGLProgId, "texcoord");
	checkGlError("glGetAttribLocation vTexCords");
	mGLUniformTexture = glGetUniformLocation(mGLProgId, "yuvTexSampler");
	checkGlError("glGetAttribLocation yuvTexSampler");

	buildParamDefaultValue();

	mIsInitialized = true;
	return true;
}

ParamVal BaseVideoEffect::getParamValHelper(string paramName, EffectCallback * filterCallback) {
	ParamVal value;

	if (filterCallback) {
		bool suc = filterCallback->getParamValue(paramName, value);
		if (suc) {
			return value;
		}

		else {
			map<string, ParamVal>::iterator itor;
			itor = mMapParamDef.find(paramName);

			if (itor != mMapParamDef.end()) {
				value = itor->second;
			}

			else {
				LOGE("can't find parameter value %s", paramName.c_str());
			}
		}
	}

	else {
		LOGE("filterCallback is NULL");

		map<string, ParamVal>::iterator itor;
		itor = mMapParamDef.find(paramName);
		if (itor != mMapParamDef.end()) {
			value = itor->second;
		}

		else {
			LOGE("can't find parameter value %s", paramName.c_str());
		}
	}

	return value;
}

void BaseVideoEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BaseVideoEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("BaseVideoEffect::renderEffect effect not initialized!");
		return;
	}

	//
	glUseProgram(mGLProgId);

	glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertexCoords);
	glEnableVertexAttribArray(mGLVertexCoords);
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glEnableVertexAttribArray");

	glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
	glEnableVertexAttribArray(mGLTextureCoords);
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glEnableVertexAttribArray");

	//
	glBindTexture(GL_TEXTURE_2D, outputFrame->getTextureId());
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glBindTexture");

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glFramebufferTexture2D");

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);
	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glViewport");

	this->onDrawArraysPre(filterCallback);

	glActiveTexture (GL_TEXTURE0);
	this->bindTexture(inputFrame->getTextureId());
	glUniform1i(mGLUniformTexture, 0);
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glUniform1i");

	this->draw();
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect draw");
	//
	glDisableVertexAttribArray(mGLVertexCoords);
	glDisableVertexAttribArray(mGLTextureCoords);

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glFramebufferTexture2D");
}

void BaseVideoEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	this->renderEffect(inputFrame, outputFrame, filterCallback, _vertices, texCoords);
}

void BaseVideoEffect::bindTexture(int texId){
	glBindTexture(GL_TEXTURE_2D, texId);

}
void BaseVideoEffect::draw(){
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void BaseVideoEffect::switchFBOAttachmentTexture(GLuint textureId){
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
}

void BaseVideoEffect::switchFBODefaultAttachment(){
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void BaseVideoEffect::destroy() {
	mIsInitialized = false;
	glDeleteProgram(mGLProgId);
}

GLuint BaseVideoEffect::loadProgram(char* pVertexSource, char* pFragmentSource) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}
	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}
	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGI("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

GLuint BaseVideoEffect::loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGI("Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}
			} else {
				LOGI( "Guessing at GL_INFO_LOG_LENGTH size\n");
				char* buf = (char*) malloc(0x1000);
				if (buf) {
					glGetShaderInfoLog(shader, 0x1000, NULL, buf);
					LOGI("Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

void BaseVideoEffect::buildParamDefaultValue() {

}
