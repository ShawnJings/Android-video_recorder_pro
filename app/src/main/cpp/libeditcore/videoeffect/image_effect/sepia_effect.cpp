#include "sepia_effect.h"

#define LOG_TAG "SepiaEffect"

SepiaEffect::SepiaEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = SEPIA_EFFECT_FRAGMENT_SHADER;
}

SepiaEffect::~SepiaEffect() {
}

bool SepiaEffect::init() {
	if (BaseVideoEffect::init()) {
		mIntensity = 1.0f;
		GLfloat mColorMatrix[16] = {
								0.3588f, 0.7044f, 0.1368f, 0.0f,
								0.2990f, 0.5870f, 0.1140f, 0.0f,
								0.2392f, 0.4696f, 0.0912f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f
							};

		mColorMatrixLocation = glGetUniformLocation(mGLProgId, "colorMatrix");
		checkGlError("glGetUniformLocation mColorMatrixLocation");
		mIntensityLocation = glGetUniformLocation(mGLProgId, "intensity");
		checkGlError("glGetUniformLocation mIntensityLocation");

		glUseProgram(mGLProgId);
		glUniform1f(mIntensityLocation, mIntensity);
		checkGlError("glUniform1f mIntensityLocation");
		glUniformMatrix4fv(mColorMatrixLocation, 1, false, mColorMatrix);
		checkGlError("glUniformMatrix4fv mColorMatrixLocation");
		return true;
	}
	return false;
}
