//
//  hue_effect.cpp
//  liveDemo
//
//  Created by apple on 16/4/21.
//  Copyright © 2016年 changba. All rights reserved.
//

#include "hue_effect.h"

#define LOG_TAG "SepiaEffect"

HueEffect::HueEffect() {
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = HUE_EFFECT_FRAGMENT_SHADER;
}

HueEffect::~HueEffect() {
}

bool HueEffect::init() {
    if (BaseVideoEffect::init()) {
        hueAdjustUniform = glGetUniformLocation(mGLProgId, "hueAdjust");
        checkGlError("glGetUniformLocation hueAdjustUniform");
        return true;
    }
    return false;
}

void HueEffect::onDrawArraysPre(EffectCallback * filterCallback) {
    if (filterCallback) {
        ParamVal val;
        bool suc = filterCallback->getParamValue(string(HUE_EFFECT_HUE_ANGLE_CHANGED), val);
        if (suc) {
            bool isChanged = val.u.boolVal;
            if (isChanged) {
                float hueAngle = 360;
                val.u.boolVal = false;
                filterCallback->setParamValue(string(HUE_EFFECT_HUE_ANGLE_CHANGED), val);
                suc = filterCallback->getParamValue(string(HUE_EFFECT_HUE_ANGLE), val);
                if (suc) {
                    hueAngle = val.u.fltVal;
                    LOGI("get success, hueAngle:%.3f", hueAngle);
                } else {
                    LOGI("get hueAngle failed, use default value hueAngle");
                }
                float hue = 2 * 3.1415926 * hueAngle / 360.0f;
                glUniform1f(hueAdjustUniform, hue);
                checkGlError("glUniform1f hueAdjustUniform");
            }
        }
    }
}
