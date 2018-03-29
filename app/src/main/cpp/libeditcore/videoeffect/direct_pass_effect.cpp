#include "./direct_pass_effect.h"

#define LOG_TAG "DirectPassEffect"

DirectPassEffect::DirectPassEffect() {
	mVertexShader = VERTEX_SHADER_FILTER1;
	mFragmentShader = FRAG_SHADER_FILTER1;

	static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
	mGLVertexBuffer = new GLfloat[8];
	for(int i = 0; i < 8; i++){
		mGLVertexBuffer[i] = _vertices[i];
	}
	static const GLfloat _texCoords[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	mGLTextureBuffer = new GLfloat[8];
	for(int i = 0; i < 8; i++){
		mGLTextureBuffer[i] = _texCoords[i];
	}
}

DirectPassEffect::~DirectPassEffect() {
	if (NULL != mGLVertexBuffer) {
		delete[] mGLVertexBuffer;
		mGLVertexBuffer = NULL;
	}
	if (NULL != mGLTextureBuffer) {
		delete[] mGLTextureBuffer;
		mGLTextureBuffer = NULL;
	}
}

void DirectPassEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL) {
		LOGE("DirectPassEffect::renderEffect input frame pointer is NULL!");
		return;
	}

	if (!mIsInitialized) {
		LOGE("DirectPassEffect::renderEffect effect not initialized!");
		return;
	}

	glUseProgram(mGLProgId);

	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	GL_CHECK_ERROR("DirectPassEffect::renderEffect glFramebufferTexture2D");
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		LOGI("failed to make complete framebuffer object %x", status);
	}
	glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, mGLVertexBuffer);
	glEnableVertexAttribArray(mGLVertexCoords);
	glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, mGLTextureBuffer);
	glEnableVertexAttribArray(mGLTextureCoords);
	GL_CHECK_ERROR("DirectPassEffect::renderEffect upload and Enable vertex coordinates");

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);
	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	GL_CHECK_ERROR("BaseVideoEffect::renderEffect glViewport");

	/* Binding the input texture */
	glActiveTexture(GL_TEXTURE0);
	this->bindTexture(inputFrame->getTextureId());
	glUniform1i(mGLUniformTexture, 0);
	GL_CHECK_ERROR("DirectPassEffect::renderEffect bind input texture");

	this->draw();
	GL_CHECK_ERROR("DirectPassEffect::renderEffect send draw signal to GPU");
	glDisableVertexAttribArray(mGLVertexCoords);
	glDisableVertexAttribArray(mGLTextureCoords);
	GL_CHECK_ERROR("DirectPassEffect::renderEffect Disable vertex coordinates");
	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK_ERROR("DirectPassEffect::renderEffect bind texture 0");
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("DirectPassEffect::renderEffect glFramebufferTexture2D switch to 0 ");
}


