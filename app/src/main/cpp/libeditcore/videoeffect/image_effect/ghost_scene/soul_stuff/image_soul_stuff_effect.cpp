//
//  soul_stuff_effect.cpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_soul_stuff_effect.h"

#define LOG_TAG "ImageSoulStuffEffect"

ImageSoulStuffEffect::ImageSoulStuffEffect(){
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = FRAG_SHADER_FILTER;
    soulFrameTexture = NULL;
    soulVideoFrame = NULL;
    mixedFrameTexture = NULL;
    mixedVideoFrame = NULL;
    transformEffect = NULL;
    mixEffect = NULL;
}

ImageSoulStuffEffect::~ImageSoulStuffEffect(){
}

bool ImageSoulStuffEffect::init() {
    isInitFrameBufferFlag = false;
    soulFrameCounter = 0;
    if (BaseVideoEffect::init()) {
        transformEffect = new ImageTransformEffect();
        if(transformEffect->init()){
            mixEffect = new MixEffect();
            return mixEffect->init();
        }
    }
    return false;
}

GPUTexture* ImageSoulStuffEffect::initFramebuffers(EffectCallback * filterCallback) {
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


void ImageSoulStuffEffect::destroy() {
    BaseVideoEffect::destroy();
    if(NULL != soulFrameTexture){
        soulFrameTexture->unLock();
    }
    if(NULL != mixedFrameTexture){
        mixedFrameTexture->unLock();
    }
    if(soulVideoFrame){
        delete soulVideoFrame;
        soulVideoFrame = NULL;
    }
    if(mixedVideoFrame) {
        delete mixedVideoFrame;
        mixedVideoFrame = NULL;
    }
    if(transformEffect) {
        transformEffect->destroy();
        delete transformEffect;
        transformEffect = NULL;
    }
    if(mixEffect) {
        mixEffect->destroy();
        delete mixEffect;
        mixEffect = NULL;
    }
}

void ImageSoulStuffEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback)
{
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    //1:如果需要初始化
    if(!isInitFrameBufferFlag){
        soulFrameTexture = this->initFramebuffers(filterCallback);
        soulVideoFrame = new OpenglVideoFrame();
        soulVideoFrame->init(soulFrameTexture->getTexId(), imgPos);
        mixedFrameTexture = this->initFramebuffers(filterCallback);
        mixedVideoFrame = new OpenglVideoFrame();
        mixedVideoFrame->init(mixedFrameTexture->getTexId(), imgPos);
        isInitFrameBufferFlag = true;
    }
    
    //2:判断是否需要拷贝, 如果需要拷贝，那么就拷贝到soulFrame;
    if(this->detectCpySoulTexture()){
        this->cpySoulTextureWithVertices(inputFrame);
    }
    
    float progress = (float)(soulFrameCounter % SOUL_STUFF_FREQUENCY) / SOUL_STUFF_FREQUENCY;
    //3:将soulTexture根据我们的fps进行放大
    this->transformMixVideoFrame(filterCallback, progress);
    
    //4:把mixedFrame和inputFrame进行Mix
    this->outputFrame(inputFrame, outputFrame, filterCallback, progress);
}

bool ImageSoulStuffEffect::detectCpySoulTexture(){
    if(soulFrameCounter % SOUL_STUFF_FREQUENCY == 0){
        return true;
    }
    return false;
}
    
void ImageSoulStuffEffect::transformMixVideoFrame(EffectCallback * filterCallback, float progress)
{
    ParamVal paramValTextureWidth = getParamValHelper(string(IMAGE_TEXTURE_WIDTH), filterCallback);
    int width = paramValTextureWidth.u.intVal;
    ParamVal paramValX;
    paramValX.type = EffectParamTypeFloat;
    paramValX.u.fltVal = 0;
    filterCallback->setParamValue(string(IMAGE_TRANSFORM_TRANSLATE_XOFFSET), paramValX);
    ParamVal paramValY;
    paramValY.type = EffectParamTypeFloat;
    paramValY.u.fltVal = 0;
    filterCallback->setParamValue(string(IMAGE_TRANSFORM_TRANSLATE_YOFFSET), paramValY);
    ParamVal paramValWidth;
    paramValWidth.type = EffectParamTypeInt;
    paramValWidth.u.intVal = width + progress * width * 3;
    filterCallback->setParamValue(string(IMAGE_TRANSFORM_SCALE_WIDTH), paramValWidth);
    ParamVal paramValAngle;
    paramValAngle.type = EffectParamTypeFloat;
    paramValAngle.u.fltVal = 0;
    filterCallback->setParamValue(string(IMAGE_TRANSFORM_ROTATE_ANGLE), paramValAngle);
    transformEffect->renderEffect(soulVideoFrame, mixedVideoFrame, filterCallback);
    soulFrameCounter++;
}
    
void ImageSoulStuffEffect::cpySoulTextureWithVertices(OpenglVideoFrame * inputFrame)
{
    this->switchFBOAttachmentTexture(soulVideoFrame->getTextureId());
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
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, imageVertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoordinates);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

void ImageSoulStuffEffect::outputFrame(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, float progress)
{
    ParamVal mixTextureIdVal;
    mixTextureIdVal.type = EffectParamTypeInt;
    mixTextureIdVal.u.intVal = mixedVideoFrame->getTextureId();
    filterCallback->setParamValue(string(MIX_FILTER_TEXTURE_ID), mixTextureIdVal);
    ParamVal mixTextureAlphaVal;
    float alpha = 0.3 - progress * 0.5;
    mixTextureAlphaVal.u.fltVal = alpha <= 0 ? 0 : alpha;
    filterCallback->setParamValue(string(MIX_FILTER_MIX_PROGRESS), mixTextureAlphaVal);
    mixEffect->renderEffect(inputFrame, outputFrame, filterCallback);
}
