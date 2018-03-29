#include "vignette_effect.h"

#define LOG_TAG "VignetteEffect"

VignetteEffect::VignetteEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = VIGNETTE_EFFECT_FRAGMENT_SHADER;
}

VignetteEffect::~VignetteEffect() {
}

bool VignetteEffect::init(){
	if(BaseVideoEffect::init()){
		GLfloat mVignetteCenter[2] = {0.5f, 0.5f};
		GLfloat mVignetteColor[3] = {0.0f, 0.0f, 0.0f};

        mVignetteCenterLocation = glGetUniformLocation(mGLProgId, "vignetteCenter");
    		checkGlError("glGetUniformLocation vignetteCenter");
        mVignetteColorLocation = glGetUniformLocation(mGLProgId, "vignetteColor");
    		checkGlError("glGetUniformLocation vignetteColor");

    		glUseProgram(mGLProgId);
        glUniform2fv(mVignetteCenterLocation, 1, mVignetteCenter);
    		checkGlError("glUniform2fv mVignetteCenterLocation");
        glUniform3fv(mVignetteColorLocation, 1, mVignetteColor);
    		checkGlError("glUniform3fv mVignetteColorLocation");
        return true;
	}
	return false;
}
