#include "whitening_ver_effect.h"

#define LOG_TAG "WhiteningVerticalEffect"

WhiteningVerticalEffect::WhiteningVerticalEffect() {
	mVertexShader = (char*) WHITENING_EFFECT_VERTEX_SHADER;
	mFragmentShader = (char*) WHITENING_EFFECT_VERTICAL_FRAG_SHADER;
}

WhiteningVerticalEffect::~WhiteningVerticalEffect() {
}

bool WhiteningVerticalEffect::init() {
	if (BaseVideoEffect::init()) {
		uniformLoc_amplitude = glGetUniformLocation(mGLProgId, "amplitude");
		checkGlError("glGetUniformLocation VerticalProgram amplitude");
		uniformLoc_edger = glGetUniformLocation(mGLProgId, "edger");
		checkGlError("glGetUniformLocation VerticalProgram edger");
		uniformLoc_offset = glGetUniformLocation(mGLProgId, "texCoordOffset");
        checkGlError("glGetUniformLocation VerticalProgram texCoordOffset");
        uniformLoc_softLightBlendR = glGetUniformLocation(mGLProgId, "softLightBlendR");
        checkGlError("glGetUniformLocation VerticalProgram softLightBlendR");
        uniformLoc_softLightBlendG = glGetUniformLocation(mGLProgId, "softLightBlendG");
        checkGlError("glGetUniformLocation VerticalProgram softLightBlendR");
        uniformLoc_softLightBlendB = glGetUniformLocation(mGLProgId, "softLightBlendB");
        checkGlError("glGetUniformLocation VerticalProgram softLightBlendR");
        uniformLoc_softLightBlendA = glGetUniformLocation(mGLProgId, "softLightBlendA");
        checkGlError("glGetUniformLocation VerticalProgram softLightBlendR");
		return true;
	}
	return false;
}

void WhiteningVerticalEffect::onDrawArraysPre(EffectCallback * filterCallback) {
	int height = 480;
	float amplitude = 0.04f;
    float edger = 20.0f;
    float radiusFactor = 1.0f;
    float softLightBlendR = 0.6f;
    float softLightBlendG = 0.6f;
    float softLightBlendB = 0.6f;
    float softLightBlendA = 0.5f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(WHITENING_FILTER_TEXTURE_HEIGHT), val);
		if (suc) {
			height = val.u.intVal;
//			LOGI("get success, height:%d", height);
		} else {
			LOGI("get height failed, use default value");
		}
		suc = filterCallback->getParamValue(string(WHITENING_FILTER_AMPLITUDE), val);
		if (suc) {
			amplitude = val.u.fltVal;
//			LOGI("get success, amplitude:%.4f", amplitude);
		} else {
			LOGI("get amplitude failed, use default value amplitude is %.2f", amplitude);
		}
		suc = filterCallback->getParamValue(string(WHITENING_FILTER_EDGER), val);
		if (suc) {
			edger = val.u.fltVal;
//			LOGI("get success, edger:%.4f", edger);
		} else {
			LOGI("get edger failed, use default value edger is %.2f", edger);
        }
        suc = filterCallback->getParamValue(string(WHITENING_BILATERAL_RADIUS_FACTOR), val);
        if (suc) {
            radiusFactor = val.u.fltVal;
//            LOGI("get success, radiusFactor:%.4f", radiusFactor);
        } else {
            LOGI("get radiusFactor failed, use default value radiusFactor is %.2f", radiusFactor);
        }
        suc = filterCallback->getParamValue(string(WHITENING_SOFT_LIGHT_BLEND_R), val);
        if (suc) {
            softLightBlendR = val.u.fltVal;
//            LOGI("get success, softLightBlendR:%.4f", softLightBlendR);
        } else {
            LOGI("get softLightBlendR failed, use default value softLightBlendR is %.2f", softLightBlendR);
        }
        suc = filterCallback->getParamValue(string(WHITENING_SOFT_LIGHT_BLEND_G), val);
        if (suc) {
            softLightBlendG = val.u.fltVal;
//            LOGI("get success, softLightBlendG:%.4f", softLightBlendG);
        } else {
            LOGI("get softLightBlendG failed, use default value softLightBlendG is %.2f", softLightBlendG);
        }
        suc = filterCallback->getParamValue(string(WHITENING_SOFT_LIGHT_BLEND_B), val);
        if (suc) {
            softLightBlendB = val.u.fltVal;
//            LOGI("get success, softLightBlendB:%.4f", softLightBlendB);
        } else {
            LOGI("get softLightBlendB failed, use default value softLightBlendB is %.2f", softLightBlendB);
        }
        suc = filterCallback->getParamValue(string(WHITENING_SOFT_LIGHT_BLEND_A), val);
        if (suc) {
            softLightBlendA = val.u.fltVal;
//            LOGI("get success, softLightBlendA:%.4f", softLightBlendA);
        } else {
            LOGI("get softLightBlendA failed, use default value softLightBlendA is %.2f", softLightBlendA);
        }
        
	}
	glUniform1f(uniformLoc_amplitude, amplitude);
	checkGlError("glUniform1f Vertical program amplitude...");
	glUniform1f(uniformLoc_edger, edger);
	checkGlError("glUniform1f Vertical program edger...");
	glUniform1f(uniformLoc_offset, (GLfloat) (radiusFactor * 1.0 / height));
    checkGlError("glUniform1f Vertical program offset...");
    glUniform1f(uniformLoc_softLightBlendR, softLightBlendR);
    checkGlError("glUniform1f Vertical program softLightBlendR...");
    glUniform1f(uniformLoc_softLightBlendG, softLightBlendG);
    checkGlError("glUniform1f Vertical program softLightBlendG...");
    glUniform1f(uniformLoc_softLightBlendB, softLightBlendB);
    checkGlError("glUniform1f Vertical program softLightBlendB...");
    glUniform1f(uniformLoc_softLightBlendA, softLightBlendA);
    checkGlError("glUniform1f Vertical program softLightBlendA...");
}

