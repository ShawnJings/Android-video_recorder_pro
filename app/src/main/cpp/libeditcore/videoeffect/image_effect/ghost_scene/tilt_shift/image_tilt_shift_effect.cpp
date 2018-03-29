//
//  tilt_shift_effect.cpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_tilt_shift_effect.h"

#define LOG_TAG "ImageTiltShiftEffect"
ImageTiltShiftEffect::ImageTiltShiftEffect(){
    mVertexShader = TILT_SHIFT_VERTEX_SHADER_FILTER;
    mFragmentShader = TILT_SHIFT_FRAG_SHADER_FILTER;
    blurFrameTexture = NULL;
    blurVideoFrame = NULL;
    blurEffect = NULL;
}
ImageTiltShiftEffect::~ImageTiltShiftEffect(){
}

bool ImageTiltShiftEffect::init() {
    isInitFrameBufferFlag = false;
    if (BaseVideoEffect::init()) {
        texture2VertexCoords = glGetAttribLocation(mGLProgId, "inputTextureCoordinate2");
        checkGlError("glGetAttribLocation texture2VertexCoords");
        
        mGLUniformTexture2 = glGetUniformLocation(mGLProgId, "inputImageTexture2");
        checkGlError("glGetUniformLocation texture2Coords Uniform");
        
        topFocusLevelUniform = glGetUniformLocation(mGLProgId, "topFocusLevel");
        checkGlError("glGetUniformLocation topFocusLevel Uniform");
        bottomFocusLevelUniform = glGetUniformLocation(mGLProgId, "bottomFocusLevel");
        checkGlError("glGetUniformLocation bottomFocusLevel Uniform");
        focusFallOffRateUniform = glGetUniformLocation(mGLProgId, "focusFallOffRate");
        checkGlError("glGetUniformLocation focusFallOffRate Uniform");
        
        glUseProgram(mGLProgId);
        leftFocusLevel = 0.0f;
        rightFocusLevel = 0.0f;
        glUniform1f(topFocusLevelUniform, leftFocusLevel);
        checkGlError("glUniform1f topFocusLevelUniform");
        glUniform1f(bottomFocusLevelUniform, rightFocusLevel);
        checkGlError("glUniform1f bottomFocusLevelUniform");
        focusFallOffRate = FOCUS_FALL_OFF_RATE;
        glUniform1f(focusFallOffRateUniform, focusFallOffRate);
        checkGlError("glUniform1f focusFallOffRateUniform");
        
        blurEffect = new BlurSceneEffect();
        return blurEffect->init();
    }
    return false;
}

GPUTexture* ImageTiltShiftEffect::initFramebuffers(EffectCallback * filterCallback) {
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

void ImageTiltShiftEffect::calSliderPosition(float position)
{
    float moveDurationInTimeMills = (position - lastSolidFramePosition) * 1000;
    leftFocusLevel = (1.0 + MOVE_SLIDER_WIDTH_RATIO) * moveDurationInTimeMills / MOVE_SCREEN_WIDTH_DURATION_IN_TIMEMILLS - MOVE_SLIDER_WIDTH_RATIO;
    rightFocusLevel = leftFocusLevel + MOVE_SLIDER_WIDTH_RATIO;
    if(leftFocusLevel <= 0.0) {
        leftFocusLevel = 0.0;
    }
    if(leftFocusLevel >= 1.0){
        leftFocusLevel = 1.0f;
    }
    if(rightFocusLevel >= 1.0){
        rightFocusLevel = 1.0;
    }
}

bool ImageTiltShiftEffect::detectCpyBlurFrame()
{
    if(leftFocusLevel == 1.0) {
        return true;
    }
    return false;
}

void ImageTiltShiftEffect::solidBlurVideoFrame(OpenglVideoFrame* inputFrame, EffectCallback* filterCallback) {
    float position = inputFrame->getStreamPosition();
    ParamVal val;
    val.type = EffectParamTypeFloat;
    val.u.fltVal = 1.0;
    filterCallback->setParamValue(string(BLUR_SCENE_PARAM_ID_PROGRESS), val);
    blurEffect->renderEffect(inputFrame, blurVideoFrame, filterCallback);
    lastSolidFramePosition = position;
}

void ImageTiltShiftEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    float position = inputFrame->getStreamPosition();
    if(!isInitFrameBufferFlag){
        blurFrameTexture = this->initFramebuffers(filterCallback);
        blurVideoFrame = new OpenglVideoFrame();
        blurVideoFrame->init(blurFrameTexture->getTexId(), imgPos);
        this->solidBlurVideoFrame(inputFrame, filterCallback);
        isInitFrameBufferFlag = true;
    }
    //将输入纹理作为输入，输出临时的的blurVideoFrame
    this->calSliderPosition(position);
    if(this->detectCpyBlurFrame()){
        this->solidBlurVideoFrame(inputFrame, filterCallback);
    }
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
    this->drawTextureWithVertices(inputFrame, blurVideoFrame, (GLfloat*)imageVertices,
                                  (GLfloat*)textureCoordinates);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
    if(leftFocusLevel == 1.0f && rightFocusLevel == 1.0f) {
        leftFocusLevel = 0.0f;
        rightFocusLevel = 0.0f;
    }
}

void ImageTiltShiftEffect::destroy(){
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

void ImageTiltShiftEffect::drawTextureWithVertices(OpenglVideoFrame * inputFrame, OpenglVideoFrame * blurFrame, GLfloat* vertices, GLfloat* textureCoords)
{
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blurVideoFrame->getTextureId());
    glUniform1i(mGLUniformTexture2, 1);
    
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    glVertexAttribPointer(texture2VertexCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(texture2VertexCoords);
    
    glUniform1f(topFocusLevelUniform, leftFocusLevel);
    checkGlError("glUniform1f topFocusLevelUniform");
    glUniform1f(bottomFocusLevelUniform, rightFocusLevel);
    checkGlError("glUniform1f bottomFocusLevelUniform");
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
