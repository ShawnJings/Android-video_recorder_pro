//
//  blur_mirror_effect.cpp
//  video_player
//
//  Created by apple on 16/10/1.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_blur_mirror_effect.h"

#define LOG_TAG "ImageBlurMirrorEffect"

ImageBlurMirrorEffect::ImageBlurMirrorEffect(){
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = FRAG_SHADER_FILTER;
    blurFrameTexture = NULL;
    blurVideoFrame = NULL;
    blurEffect = NULL;
}
ImageBlurMirrorEffect::~ImageBlurMirrorEffect(){
}

bool ImageBlurMirrorEffect::init() {
    isInitFrameBufferFlag = false;
    if (BaseVideoEffect::init()) {
        blurEffect = new BlurSceneEffect();
        return blurEffect->init();
    }
    return false;
}

GPUTexture* ImageBlurMirrorEffect::initFramebuffers(EffectCallback * filterCallback) {
    int width = 480;
    int height = 480;
    if (filterCallback) {
        ParamVal val;
        bool suc = filterCallback->getParamValue(string(IMAGE_TEXTURE_WIDTH), val);
        if (suc) {
            width = val.u.intVal;
            LOGI("get success, width:%d", width);
        } else {
            LOGI("get width failed, use default value");
        }
        suc = filterCallback->getParamValue(string(IMAGE_TEXTURE_HEIGHT), val);
        if (suc) {
            height = val.u.intVal;
            LOGI("get height success, progress:%d", height);
        } else {
            LOGI("get height failed, use default value progress is %d", height);
        }
    }
    GPUTexture* frameTexture = GPUTextureCache::GetInstance()->fetchTexture(width, height);
    frameTexture->lock();
    return frameTexture;
}

void ImageBlurMirrorEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    if(!isInitFrameBufferFlag){
        blurFrameTexture = this->initFramebuffers(filterCallback);
        blurVideoFrame = new OpenglVideoFrame();
        blurVideoFrame->init(blurFrameTexture->getTexId(), imgPos);
        isInitFrameBufferFlag = true;
    }
    //将输入纹理作为输入，输出临时的的blurVideoFrame
    ParamVal val;
    val.type = EffectParamTypeFloat;
    val.u.fltVal = 1.0;
    filterCallback->setParamValue(string(BLUR_SCENE_PARAM_ID_PROGRESS), val);
    blurEffect->renderEffect(inputFrame, blurVideoFrame, filterCallback);
    //将输入纹理与上一步得到的纹理作为输入，输出最终的outputFrame
    this->switchFBOAttachmentTexture(outputFrame->getTextureId());
    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f,  1.0f,
    };
    static const GLfloat textureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    this->drawTextureWithVertices(blurVideoFrame, (GLfloat*)imageVertices,
                                  (GLfloat*)textureCoordinates);
    
    static const GLfloat leftCenterImageVertices[] = {
        -1.0f, -0.5f,
        0.0f, -0.5f,
        -1.0f,  0.5f,
        0.0f,  0.5f,
    };
    
    static const GLfloat leftCenterTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    this->drawTextureWithVertices(inputFrame, (GLfloat*)leftCenterImageVertices,
                                  (GLfloat*)leftCenterTextureCoordinates);
    
    static const GLfloat rightCenterImageVertices[] = {
        0.0f, -0.5f,
        1.0f, -0.5f,
        0.0f,  0.5f,
        1.0f,  0.5f,
    };
    static const GLfloat rightCenterTextureCoordinates[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };
    this->drawTextureWithVertices(inputFrame, (GLfloat*)rightCenterImageVertices,
                                  (GLfloat*)rightCenterTextureCoordinates);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

void ImageBlurMirrorEffect::destroy(){
    BaseVideoEffect::destroy();
    if(NULL != blurFrameTexture){
        blurFrameTexture->unLock();
    }
    if(blurVideoFrame){
        delete blurVideoFrame;
        blurVideoFrame = NULL;
    }
    if(blurEffect){
        blurEffect->destroy();
        delete blurEffect;
        blurEffect = NULL;
    }
}

void ImageBlurMirrorEffect::drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords)
{
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
