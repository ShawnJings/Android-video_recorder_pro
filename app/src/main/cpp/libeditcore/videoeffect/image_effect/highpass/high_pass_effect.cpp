//
//  high_pass_effect.cpp
//  liveDemo
//
//  Created by apple on 16/4/5.
//  Copyright © 2016年 changba. All rights reserved.
//

#include "high_pass_effect.h"

#define LOG_TAG "HighPassEffect"

HighPassEffect::HighPassEffect() {
    mVertexShader = HIGH_PASS_BLUR_SCENE_HORIZONTAL_VERTEX_SHADER;
    mFragmentShader = HIGH_PASS_BLUR_SCENE_HORIZONTAL_FRAG_SHADER;
    blurTempTexCache = NULL;
}

HighPassEffect::~HighPassEffect() {
}

bool HighPassEffect::init(){
    /** default build Horizontal Program **/
    BaseVideoEffect::init();
    glUseProgram(mGLProgId);
    uniformLoc_hor_texelWidthOffset = glGetUniformLocation(mGLProgId, "texelWidthOffset");
    checkGlError("BlurSceneEffect glGetUniformLocation texelWidthOffset");
    uniformLoc_hor_texelHeightOffset = glGetUniformLocation(mGLProgId, "texelHeightOffset");
    checkGlError("BlurSceneEffect glGetUniformLocation texelHeightOffset");
    blurTempTexCache = NULL;
    /** build Vertical Program **/
    verticalProgId = loadProgram(HIGH_PASS_BLUR_SCENE_VERTICAL_VERTEX_SHADER, HIGH_PASS_BLUR_SCENE_VERTICAL_FRAG_SHADER);
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
    checkGlError("glGetAttribLocation uniformLoc_ver_Texture");
    uniformLoc_ver_texelWidthOffset = glGetUniformLocation(verticalProgId, "texelWidthOffset");
    checkGlError("glGetAttribLocation uniformLoc_ver_texelHeightOffset");
    uniformLoc_ver_texelHeightOffset = glGetUniformLocation(verticalProgId, "texelHeightOffset");
    checkGlError("glGetAttribLocation uniformLoc_ver_texelHeightOffset");
    
    
    /** build Blur Horizontal Program **/
    alphaBlurHorProgId = loadProgram(ALPHA_BLUR_HORIZONTAL_VERTEX_SHADER, ALPHA_BLUR_HORIZONTAL_FRAG_SHADER);
    if (!alphaBlurHorProgId) {
        LOGE("Could not create program Alpha Blur horizontalProgId.");
        return false;
    }
    glUseProgram(alphaBlurHorProgId);
    attr_alpha_blur_hor_VertexCoords = glGetAttribLocation(alphaBlurHorProgId, "position");
    checkGlError("glGetAttribLocation alphaBlurHorProgId attr_alpha_blur_hor_VertexCoords");
    attr_alpha_blur_hor_TextureCoords = glGetAttribLocation(alphaBlurHorProgId, "texcoord");
    checkGlError("glGetAttribLocation alphaBlurHorProgId attr_alpha_blur_hor_TextureCoords");
    uniformLoc_alpha_blur_hor_Texture = glGetUniformLocation(alphaBlurHorProgId, "yuvTexSampler");
    checkGlError("glGetAttribLocation alphaBlurHorProgId uniformLoc_alpha_blur_hor_Texture");
    uniformLoc_alpha_blur_hor_texelWidthOffset = glGetUniformLocation(alphaBlurHorProgId, "texelWidthOffset");
    checkGlError("glGetAttribLocation alphaBlurHorProgId uniformLoc_alpha_blur_hor_texelWidthOffset");
    uniformLoc_alpha_blur_hor_texelHeightOffset = glGetUniformLocation(alphaBlurHorProgId, "texelHeightOffset");
    checkGlError("glGetAttribLocation alphaBlurHorProgId uniformLoc_alpha_blur_hor_texelHeightOffset");
    
    /** build Vertical Program **/
    alphaBlurVerProgId = loadProgram(ALPHA_BLUR_VERTICAL_VERTEX_SHADER, ALPHA_BLUR_VERTICAL_FRAG_SHADER);
    if (!alphaBlurVerProgId) {
        LOGE("Could not create program Trailer Scene verticalProgId.");
        return false;
    }
    glUseProgram(alphaBlurVerProgId);
    attr_alpha_blur_ver_VertexCoords = glGetAttribLocation(alphaBlurVerProgId, "position");
    checkGlError("glGetAttribLocation alphaBlurVerProgId attr_alpha_blur_ver_VertexCoords");
    attr_alpha_blur_ver_TextureCoords = glGetAttribLocation(alphaBlurVerProgId, "texcoord");
    checkGlError("glGetAttribLocation alphaBlurVerProgId attr_alpha_blur_ver_TextureCoords");
    uniformLoc_alpha_blur_ver_Texture = glGetUniformLocation(alphaBlurVerProgId, "yuvTexSampler");
    checkGlError("glGetAttribLocation alphaBlurVerProgId uniformLoc_alpha_blur_ver_Texture");
    uniformLoc_alpha_blur_ver_texelWidthOffset = glGetUniformLocation(alphaBlurVerProgId, "texelWidthOffset");
    checkGlError("glGetAttribLocation alphaBlurVerProgId uniformLoc_alpha_blur_ver_texelWidthOffset");
    uniformLoc_alpha_blur_ver_texelHeightOffset = glGetUniformLocation(alphaBlurVerProgId, "texelHeightOffset");
    checkGlError("glGetAttribLocation alphaBlurVerProgId uniformLoc_alpha_blur_ver_texelHeightOffset");
    
    return true;
}

/**  Template Method  **/
void HighPassEffect::buildParamDefaultValue() {
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


void HighPassEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords){

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
    
    float progress = 0.5f;
    if (filterCallback) {
        ParamVal val;
        bool suc = filterCallback->getParamValue(string(BLUR_SCENE_PARAM_ID_PROGRESS), val);
        if (suc) {
            progress = val.u.fltVal;
//            LOGI("get success, progress:%.4f", progress);
        } else {
//            LOGI("get progress failed, use default value");
        }
    }
//    LOGI("after BlurSceneEffect getParamValue ");
    
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
    glUniform1f(uniformLoc_hor_texelHeightOffset, (GLfloat)(1.0 * progress / imgPos.width));
    
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
    glUniform1f(uniformLoc_ver_texelWidthOffset, (GLfloat)(1.0 * progress / imgPos.height));
    glUniform1f(uniformLoc_ver_texelHeightOffset, 0.0);
    
    this->draw();
    glDisableVertexAttribArray(attr_ver_VertexCoords);
    glDisableVertexAttribArray(attr_ver_TextureCoords);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    
    /** 做完HighPass放到Alpha里面的图像，我们再做一次Blur **/
    this->switchFBOAttachmentTexture(blurTempTexCache->getTexId());
    
    glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
    glUseProgram(alphaBlurHorProgId);
    glVertexAttribPointer(attr_alpha_blur_hor_VertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(attr_alpha_blur_hor_VertexCoords);
    glVertexAttribPointer(attr_alpha_blur_hor_TextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(attr_alpha_blur_hor_TextureCoords);
    
    /** 这个是原始的纹理 **/
    glActiveTexture(GL_TEXTURE0);
    this->bindTexture(outputFrame->getTextureId());
    glUniform1i(uniformLoc_alpha_blur_hor_Texture, 0);
    /** 设置FadeIn的Progress **/
    glUniform1f(uniformLoc_alpha_blur_hor_texelWidthOffset, 0.0);
    glUniform1f(uniformLoc_alpha_blur_hor_texelHeightOffset, (GLfloat)(1.0 * progress / imgPos.width));
    
    this->draw();
    glDisableVertexAttribArray(attr_alpha_blur_hor_VertexCoords);
    glDisableVertexAttribArray(attr_alpha_blur_hor_TextureCoords);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    GL_CHECK_ERROR("11111TrailerSceneEffect::renderEffect glBindFramebuffer 0 ");
    
    /** 然后以blurTempTexCache这个纹理作为输入纹理，再加上FadeInTexId作为输入纹理，以及Progress输出到我们的目标FBO上面 **/
    this->switchFBOAttachmentTexture(outputFrame->getTextureId());
    glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
    glUseProgram(alphaBlurVerProgId);
    glVertexAttribPointer(attr_alpha_blur_ver_VertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(attr_alpha_blur_ver_VertexCoords);
    glVertexAttribPointer(attr_alpha_blur_ver_TextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(attr_alpha_blur_ver_TextureCoords);
    
    /** 这个是临时过渡的纹理 **/
    glActiveTexture(GL_TEXTURE0);
    this->bindTexture(blurTempTexCache->getTexId());
    glUniform1i(uniformLoc_alpha_blur_ver_Texture, 0);
    /** 设置FadeIn的Progress **/
    glUniform1f(uniformLoc_alpha_blur_ver_texelWidthOffset, (GLfloat)(1.0 * progress / imgPos.height));
    glUniform1f(uniformLoc_alpha_blur_ver_texelHeightOffset, 0.0);
    
    this->draw();
    glDisableVertexAttribArray(attr_alpha_blur_ver_VertexCoords);
    glDisableVertexAttribArray(attr_alpha_blur_ver_TextureCoords);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    
    GL_CHECK_ERROR("2: BlurSceneEffect::renderEffect vertical blur bindframebuffer 0");
}

void HighPassEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat texCoords[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    renderEffect(inputFrame, outputFrame, filterCallback, _vertices, texCoords);
}

void HighPassEffect::destroy() {
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
