#include "contrast_effect.h"

#define LOG_TAG "ContrastEffect"

ContrastEffect::ContrastEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = CONTRAST_EFFECT_FRAGMENT_SHADER;
}

ContrastEffect::~ContrastEffect() {
}

bool ContrastEffect::init() {
    if (BaseVideoEffect::init()) {
    		contrastSlopeUniform = glGetUniformLocation(mGLProgId, "contrast");
        checkGlError("glGetUniformLocation contrastSlopeUniform");
        return true;
    }
    return false;
}

void ContrastEffect::onDrawArraysPre(EffectCallback * filterCallback) {
    if (filterCallback) {
        ParamVal val;
        bool suc = filterCallback->getParamValue(string(CONTRAST_EFFECT_SLOPE_CHANGED), val);
        if (suc) {
            bool isChanged = val.u.boolVal;
            if (isChanged) {
                float slope = 1.0;
                val.u.boolVal = false;
                filterCallback->setParamValue(string(CONTRAST_EFFECT_SLOPE_CHANGED), val);
                suc = filterCallback->getParamValue(string(CONTRAST_EFFECT_SLOPE), val);
                if (suc) {
                		slope = val.u.fltVal;
                    LOGI("get success, slope:%.3f", slope);
                } else {
                    LOGI("get slope failed, use default value slope");
                }
                glUniform1f(contrastSlopeUniform, slope);
                checkGlError("glUniform1f contrastSlopeUniform");
            }
        }
    }
}
