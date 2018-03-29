#include "whitening_hor_effect.h"

#define LOG_TAG "WhiteningHorizontalEffect"

WhiteningHorizontalEffect::WhiteningHorizontalEffect() {
	mVertexShader = (char*) WHITENING_EFFECT_HOR_VERTEX_SHADER;
	mFragmentShader = (char*) WHITENING_EFFECT_HORIZONTAL_FRAG_SHADER;
}

WhiteningHorizontalEffect::~WhiteningHorizontalEffect() {
}

bool WhiteningHorizontalEffect::init() {
	if (BaseVideoEffect::init()) {
		uniformLoc_amplitude = glGetUniformLocation(mGLProgId, "amplitude");
		checkGlError("glGetAttribLocation HorizontalProgram amplitude");
		uniformLoc_edger = glGetUniformLocation(mGLProgId, "edger");
		checkGlError("glGetAttribLocation HorizontalProgram edger");
		uniformLoc_offset = glGetUniformLocation(mGLProgId, "texCoordOffset");
		checkGlError("glGetAttribLocation HorizontalProgram texCoordOffset");
		return true;
	}
	return false;
}

void WhiteningHorizontalEffect::onDrawArraysPre(EffectCallback * filterCallback) {
	int width = 480;
	float amplitude = 0.04f;
    float edger = 20.0f;
    float radiusFactor = 1.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(WHITENING_FILTER_TEXTURE_WIDTH), val);
		if (suc) {
			width = val.u.intVal;
//			LOGI("get success, width:%d", width);
		} else {
			LOGI("get width failed, use default value");
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
	}
	glUniform1f(uniformLoc_amplitude, amplitude);
	checkGlError("glUniform1f Vertical program amplitude...");
	glUniform1f(uniformLoc_edger, edger);
	checkGlError("glUniform1f Vertical program edger...");
	glUniform1f(uniformLoc_offset, (GLfloat) (radiusFactor * 1.0 / width));
	checkGlError("glUniform1f Vertical program offset...");
}
