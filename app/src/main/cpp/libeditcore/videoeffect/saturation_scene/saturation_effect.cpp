#include "./saturation_effect.h"

#define LOG_TAG "SaturationEffect"

SaturationEffect::SaturationEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = SATURATION_EFFECT_FRAGMENT_SHADER;
}

SaturationEffect::~SaturationEffect() {

}

bool SaturationEffect::init() {
	if (BaseVideoEffect::init()) {
		mSaturationLocation = glGetUniformLocation(mGLProgId, "saturation");
		checkGlError("glGetUniformLocation mSaturationLocation");
		return true;
	}
	return false;
}

void SaturationEffect::onDrawArraysPre(EffectCallback* filterCallback) {
	float mSaturation = 1.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(SATURATION_SCENE_PARAM_ID_SATURATION), val);
		if (suc) {
			mSaturation = val.u.fltVal;
			LOGI("get success, mSaturation:%.4f", mSaturation);
		} else {
			LOGI("get progress failed, use default value");
		}
	}
	glUniform1f(mSaturationLocation, mSaturation);
	checkGlError("glUniform1f mSaturationLocation");
}
