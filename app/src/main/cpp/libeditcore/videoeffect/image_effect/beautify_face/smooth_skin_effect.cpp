//
//  high_pass_effect.cpp
//  liveDemo
//
//  Created by apple on 16/4/5.
//  Copyright © 2016年 changba. All rights reserved.
//
#include "smooth_skin_effect.h"
#include <iostream>

#define LOG_TAG "SmoothSkinEffect"

SmoothSkinEffect::SmoothSkinEffect() {
    mVertexShader = getHorizontalSkinVertexShader();;
    mFragmentShader = getHorizontalSkinFragmentShader();
    blurTempTexCache = NULL;
}

SmoothSkinEffect::~SmoothSkinEffect() {
}

bool SmoothSkinEffect::init(){
    /** default build Horizontal Program **/
    BaseVideoEffect::init();
    glUseProgram(mGLProgId);
    uniformLoc_hor_texelWidthOffset = glGetUniformLocation(mGLProgId, "texelWidthOffset");
    checkGlError("SmoothSkinEffect glGetUniformLocation texelWidthOffset");
    uniformLoc_hor_texelHeightOffset = glGetUniformLocation(mGLProgId, "texelHeightOffset");
    checkGlError("SmoothSkinEffect glGetUniformLocation texelHeightOffset");
    blurTempTexCache = NULL;
    /** build Vertical Program **/
    verticalProgId = loadProgram(getVerticalSkinVertexShader(), getVerticalSkinFragmentShader());
    if (!verticalProgId) {
        LOGE("Could not create program High Pass Effect verticalProgId.");
        return false;
    }
    glUseProgram(verticalProgId);
    attr_ver_VertexCoords = glGetAttribLocation(verticalProgId, "position");
    checkGlError("glGetAttribLocation attr_ver_VertexCoords");
    attr_ver_TextureCoords = glGetAttribLocation(verticalProgId, "texcoord");
    checkGlError("glGetAttribLocation attr_ver_TextureCoords");
    uniformLoc_ver_Texture = glGetUniformLocation(verticalProgId, "yuvTexSampler");
    checkGlError("glGetUniform uniformLoc_ver_Texture");
    uniformLoc_ver_texelWidthOffset = glGetUniformLocation(verticalProgId, "texelWidthOffset");
    checkGlError("glGetUniform uniformLoc_ver_texelWidthOffset");
    uniformLoc_ver_texelHeightOffset = glGetUniformLocation(verticalProgId, "texelHeightOffset");
    checkGlError("glGetUniform uniformLoc_ver_texelHeightOffset");
    maskCurveProgressUniform = glGetUniformLocation(verticalProgId, "mask_curve_progress");
    checkGlError("glGetUniform maskCurveProgressUniform");
    softLightProgressUniform = glGetUniformLocation(verticalProgId, "soft_light_progress");
    checkGlError("glGetUniform softLightProgressUniform");
    sCurveProgressUniform = glGetUniformLocation(verticalProgId, "s_curve_progress");
    checkGlError("glGetUniform sCurveProgressUniform");
    satuRatioUniform = glGetUniformLocation(verticalProgId, "satuRatio");
    checkGlError("glGetUniform satuRatioUniform");
    
    return true;
}

/**  Template Method  **/
void SmoothSkinEffect::buildParamDefaultValue() {
    mMapParamDef.clear();
    
    ParamVal paramValVertexCoords;
    paramValVertexCoords.type = EffectParamTypeArbitrary;
    float _vertexs[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
    float * vertexCoords = new float[8];
    memcpy(vertexCoords, _vertexs, sizeof(float) * 8);
    paramValVertexCoords.u.arbData = (void *) vertexCoords;
    mMapParamDef[string(CONTRAST_PARAM_ID_VERTEX_COORDS)] = paramValVertexCoords;
    
    ParamVal paramValTextureCoords;
    paramValTextureCoords.type = EffectParamTypeArbitrary;
    float _textures[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    float * texCoords = new float[8];
    memcpy(texCoords, _textures, sizeof(float) * 8);
    paramValTextureCoords.u.arbData = (void *) texCoords;
    mMapParamDef[string(CONTRAST_PARAM_ID_TEXTURE_COORDS)] = paramValTextureCoords;
}


void SmoothSkinEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords){
    
    if (inputFrame == NULL || outputFrame == NULL) {
        LOGE("BlurSceneEffect::renderEffect input or output frame pointer is NULL!");
        return;
    }
    if (!mIsInitialized) {
        LOGE("BlurSceneEffect::renderEffect effect not initialized!");
        return;
    }
    
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    
    //LOGI("BlurSceneEffect getParamValue ");
    if(NULL == blurTempTexCache){
        blurTempTexCache = GPUTextureCache::GetInstance()->fetchTexture(imgPos.width, imgPos.height);
        blurTempTexCache->lock();
    }
    /** 首先以原始纹理作为输入, 做一遍横向的blur, 输出到blurTempTexCache中 **/
    this->switchFBOAttachmentTexture(blurTempTexCache->getTexId());
    
    glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
    glUseProgram(mGLProgId);
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertexCoords);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    /** 这个是原始的纹理 **/
    glActiveTexture(GL_TEXTURE0);
    this->bindTexture(inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    /** 设置FadeIn的Progress **/
    glUniform1f(uniformLoc_hor_texelWidthOffset, 0.0);
    glUniform1f(uniformLoc_hor_texelHeightOffset, (GLfloat)(getHighpassRadius() / imgPos.height));
    
    this->draw();
    glDisableVertexAttribArray(mGLVertexCoords);
    glDisableVertexAttribArray(mGLTextureCoords);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    GL_CHECK_ERROR("11111TrailerSceneEffect::renderEffect glBindFramebuffer 0 ");
    
    /** 然后以blurTempTexCache这个纹理作为输入纹理，再加上FadeInTexId作为输入纹理，以及Progress输出到我们的目标FBO上面 **/
    this->switchFBOAttachmentTexture(outputFrame->getTextureId());
    glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
    glUseProgram(verticalProgId);
    GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
    glVertexAttribPointer(attr_ver_VertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(attr_ver_VertexCoords);
    glVertexAttribPointer(attr_ver_TextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(attr_ver_TextureCoords);
    
    /** 这个是临时过渡的纹理 **/
    glActiveTexture(GL_TEXTURE0);
    this->bindTexture(blurTempTexCache->getTexId());
    glUniform1i(uniformLoc_ver_Texture, 0);
    /** 设置FadeIn的Progress **/
    //    std::cout<< "progress in height: " << progress << std::endl;
    glUniform1f(uniformLoc_ver_texelWidthOffset, (GLfloat)(getHighpassRadius() / imgPos.width));
    glUniform1f(uniformLoc_ver_texelHeightOffset, 0.0);
    
    ParamVal val;
    bool suc = filterCallback->getParamValue(string(SMOOTH_SKIN_EFFECT_PARAM_CHANGED), val);
    if (suc) {
        bool isChanged = val.u.boolVal;
        if (isChanged) {
            val.u.boolVal = false;
            filterCallback->setParamValue(string(SMOOTH_SKIN_EFFECT_PARAM_CHANGED), val);
            float maskCurveProgress = 0.3;
            suc = filterCallback->getParamValue(string(MASK_CURVE_PROGRESS), val);
            if (suc) {
                maskCurveProgress = val.u.fltVal;
                LOGI("get success, maskCurveProgress:%.3f", maskCurveProgress);
            } else {
                LOGI("get maskCurveProgress failed, use default value maskCurveProgress");
            }
            glUniform1f(maskCurveProgressUniform, maskCurveProgress);
            checkGlError("glUniform1f maskCurveProgressUniform");
            
            float softLightProgress = 1.0;
            suc = filterCallback->getParamValue(string(SOFT_LIGHT_PROGRESS), val);
            if (suc) {
                softLightProgress = val.u.fltVal;
                LOGI("get success, softLightProgress:%.3f", softLightProgress);
            } else {
                LOGI("get softLightProgress failed, use default value softLightProgress");
            }
            glUniform1f(softLightProgressUniform, softLightProgress);
            checkGlError("glUniform1f maskCurveProgressUniform");
            
            
            float sCurveProgress = 48.0;
            suc = filterCallback->getParamValue(string(S_CURVE_PROGRESS), val);
            if (suc) {
                sCurveProgress = val.u.fltVal;
                LOGI("get success, sCurveProgress:%.3f", sCurveProgress);
            } else {
                LOGI("get sCurveProgress failed, use default value sCurveProgress");
            }
            glUniform1f(sCurveProgressUniform, sCurveProgress);
            checkGlError("glUniform1f sCurveProgressUniform");
            
            
            float satuRatio = 0.8f;
            suc = filterCallback->getParamValue(string(SATU_RATIO), val);
            if (suc) {
                satuRatio = val.u.fltVal;
                LOGI("get success, satuRatio:%.3f", satuRatio);
            } else {
                LOGI("get satuRatio failed, use default value satuRatio");
            }
            glUniform1f(satuRatioUniform, satuRatio);
            checkGlError("glUniform1f sCurveProgressUniform");
        }
    }
    
    this->draw();
    glDisableVertexAttribArray(attr_ver_VertexCoords);
    glDisableVertexAttribArray(attr_ver_TextureCoords);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    GL_CHECK_ERROR("2: BlurSceneEffect::renderEffect vertical blur bindframebuffer 0");
}

void SmoothSkinEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat texCoords[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    renderEffect(inputFrame, outputFrame, filterCallback, _vertices, texCoords);
}

void SmoothSkinEffect::destroy() {
    BaseVideoEffect::destroy();
    
    glDeleteProgram(verticalProgId);
    
    if(NULL != blurTempTexCache){
        blurTempTexCache->unLock();
    }
    // must delete arbitrary data
    map<string, ParamVal>::iterator itor;
    itor = mMapParamDef.find(string(CONTRAST_PARAM_ID_VERTEX_COORDS));
    if (itor != mMapParamDef.end()) {
        float * pointer = (float *) ((itor->second).u.arbData);
        delete[] pointer;
    }
    
    itor = mMapParamDef.find(string(CONTRAST_PARAM_ID_TEXTURE_COORDS));
    if (itor != mMapParamDef.end()) {
        float * pointer = (float *) ((itor->second).u.arbData);
        delete[] pointer;
    }
    
    mMapParamDef.clear();
}
