//
//  color_invert_effect.cpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_color_invert_effect.h"

#define LOG_TAG "ImageColorInvertEffect"

ImageColorInvertEffect::ImageColorInvertEffect(){
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = COLOR_INVERT_FRAG_SHADER_FILTER;
    colorInvertFrameTexture = NULL;
    colorInvertVideoFrame = NULL;
}
ImageColorInvertEffect::~ImageColorInvertEffect(){
}

bool ImageColorInvertEffect::init() {
    isInitFrameBufferFlag = false;
    if(BaseVideoEffect::init()){
       colorInvertFlagUniform = glGetUniformLocation(mGLProgId, "colorInvertFlag");
        return true;
    }
    return false;
}

void ImageColorInvertEffect::destroy(){
    BaseVideoEffect::destroy();
    if(NULL != colorInvertFrameTexture){
        colorInvertFrameTexture->unLock();
    }
    if(colorInvertVideoFrame){
        delete colorInvertVideoFrame;
        colorInvertVideoFrame = NULL;
    }
}

GPUTexture* ImageColorInvertEffect::initFramebuffers(EffectCallback * filterCallback) {
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

void ImageColorInvertEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback)
{
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
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    float position = inputFrame->getStreamPosition();
    if(!isInitFrameBufferFlag){
        colorInvertFrameTexture = this->initFramebuffers(filterCallback);
        colorInvertVideoFrame = new OpenglVideoFrame();
        colorInvertVideoFrame->init(colorInvertFrameTexture->getTexId(), imgPos, position);
        lastColorInvertFrameRenderPos = position - (float)COLOR_INVERT_FRAME_RENDER_DURATION_IN_TIMEMILLS / 1000.0f;
        isInitFrameBufferFlag = true;
    }
    if(this->detectCpyColorInvertFrame(position)) {
        this->cpyColorInvertTexture(inputFrame, (GLfloat*)imageVertices, (GLfloat*)textureCoordinates);
    }
    this->switchFBOAttachmentTexture(outputFrame->getTextureId());
    OpenglVideoFrame* renderFrame = this->getRenderFrame(inputFrame);
    this->drawTextureWithVertices(renderFrame, (GLfloat*)imageVertices, (GLfloat*)textureCoordinates);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

OpenglVideoFrame* ImageColorInvertEffect::getRenderFrame(OpenglVideoFrame* inputFrame) {
    float position = inputFrame->getStreamPosition();
    if(1000 * (position - lastColorInvertFrameRenderPos) >= COLOR_INVERT_FRAME_RENDER_DURATION_IN_TIMEMILLS) {
        return inputFrame;
    }
    return colorInvertVideoFrame;
}

bool ImageColorInvertEffect::detectCpyColorInvertFrame(float position) {
    if(1000 * (position - lastColorInvertFrameRenderPos) >= (COMMON_FRAME_RENDER_DURATION_IN_TIMEMILLS + COLOR_INVERT_FRAME_RENDER_DURATION_IN_TIMEMILLS)){
        return true;
    }
    return false;
}

void ImageColorInvertEffect::cpyColorInvertTexture(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords) {
    this->switchFBOAttachmentTexture(colorInvertVideoFrame->getTextureId());
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glUniform1i(colorInvertFlagUniform, 1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    lastColorInvertFrameRenderPos = inputFrame->getStreamPosition();
}

void ImageColorInvertEffect::drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords)
{
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glUniform1i(colorInvertFlagUniform, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}