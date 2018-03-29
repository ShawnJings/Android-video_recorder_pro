//
//  sharpen_effect.cpp
//  liveDemo
//
//  Created by apple on 16/5/12.
//  Copyright © 2016年 changba. All rights reserved.
//

#include "sharpen_effect.h"

#define LOG_TAG "SharpenEffect"

SharpenEffect::SharpenEffect() {
    mVertexShader = SHARPEN_EFFECT_VERTEX_SHADER;
    mFragmentShader = SHARPEN_EFFECT_FRAGMENT_SHADER;
}

SharpenEffect::~SharpenEffect() {
}

bool SharpenEffect::init() {
    if (BaseVideoEffect::init()) {
        sharpnessUniform = glGetUniformLocation(mGLProgId, "sharpness");
        checkGlError("glGetUniformLocation sharpnessUniform");
        imageWidthFactorUniform = glGetUniformLocation(mGLProgId, "imageWidthFactor");
        checkGlError("glGetUniformLocation imageWidthFactorUniform");
        imageHeightFactorUniform = glGetUniformLocation(mGLProgId, "imageHeightFactor");
        checkGlError("glGetUniformLocation imageHeightFactorUniform");
        return true;
    }
    return false;
}

void SharpenEffect::onDrawArraysPre(EffectCallback * filterCallback) {
    if (filterCallback) {
        ParamVal val;
        bool suc = filterCallback->getParamValue(string(SHARPEN_EFFECT_SHARPNESS), val);
        if (suc) {
            float sharpness = 0.0f;
            if (suc) {
                sharpness = val.u.fltVal;
//                LOGI("get success, sharpness:%.3f", sharpness);
            } else {
                LOGI("get sharpness failed, use default value sharpness");
            }
            glUniform1f(sharpnessUniform, sharpness);
            checkGlError("glUniform1f sharpnessUniform");
        }
        suc = filterCallback->getParamValue(string(IMAGE_EFFECT_GROUP_TEXTURE_WIDTH), val);
        if (suc) {
            float inputWidth = 720.f;
            if (suc) {
                inputWidth = val.u.intVal;
//                LOGI("get success, inputWidth:%.3f", inputWidth);
            } else {
                LOGI("get inputWidth failed, use default value inputWidth");
            }
            glUniform1f(imageWidthFactorUniform, 1.0 / inputWidth);
            checkGlError("glUniform1f imageWidthFactorUniform");
        }
        suc = filterCallback->getParamValue(string(IMAGE_EFFECT_GROUP_TEXTURE_HEIGHT), val);
        if (suc) {
            float inputHeight = 1280.f;
            if (suc) {
                inputHeight = val.u.intVal;
//                LOGI("get success, inputHeight:%.3f", inputHeight);
            } else {
                LOGI("get inputHeight failed, use default value inputHeight");
            }
            glUniform1f(imageHeightFactorUniform, 1.0 / inputHeight);
            checkGlError("glUniform1f imageHeightFactorUniform");
        }
    }
}