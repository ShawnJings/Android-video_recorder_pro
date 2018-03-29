//
//  carousel_effect.cpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_carousel_effect.h"

#define LOG_TAG "ImageCarouselEffect"

ImageCarouselEffect::ImageCarouselEffect(){
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = FRAG_SHADER_FILTER;
}
ImageCarouselEffect::~ImageCarouselEffect(){
}

void ImageCarouselEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    currentFramePos = inputFrame->getStreamPosition();
    //1:拷贝当前纹理
    this->cpyTextureFrame(inputFrame);
    //2:将合适的纹理绘制到屏幕上
    this->switchFBOAttachmentTexture(outputFrame->getTextureId());
    static const GLfloat textureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    this->drawLeftTopTextureWithCoords((GLfloat*)textureCoordinates);
    this->drawRightTopTextureWithCoords((GLfloat*)textureCoordinates);
    this->drawLeftBottomTextureWithCoords((GLfloat*)textureCoordinates);
    this->drawRightBottomTextureWithCoords((GLfloat*)textureCoordinates);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

void ImageCarouselEffect::destroy() {
    BaseVideoEffect::destroy();
    while (!rightTopFrameBucket.empty()) {
        DelayTextureFrame* frame = rightTopFrameBucket.front();
        delete frame;
        rightTopFrameBucket.pop();
    }
    while (!leftBottomFrameBucket.empty()) {
        DelayTextureFrame* frame = leftBottomFrameBucket.front();
        delete frame;
        leftBottomFrameBucket.pop();
    }
    while (!rightBottomFrameBucket.empty()) {
        DelayTextureFrame* frame = rightBottomFrameBucket.front();
        delete frame;
        rightBottomFrameBucket.pop();
    }
}

void ImageCarouselEffect::cpyTextureFrame(OpenglVideoFrame * inputFrame)
{
    ImagePosition imgPos;
    inputFrame->GetImagePosition(&imgPos);
    DelayTextureFrame* delayFrame = new DelayTextureFrame(imgPos, inputFrame->getStreamPosition());
    this->switchFBOAttachmentTexture(delayFrame->getTextureId());
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f,  1.0f,
    };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, imageVertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    static const GLfloat textureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoordinates);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    this->switchFBODefaultAttachment();
    rightTopFrameBucket.push(delayFrame);
}

void ImageCarouselEffect::drawLeftTopTextureWithCoords(GLfloat* textureCoords)
{
    DelayTextureFrame* frame = rightTopFrameBucket.back();
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    static const GLfloat vertices[] = {
        -1.0f, 0.0f,
        0.0f, 0.0f,
        -1.0f,  1.0f,
        0.0f,  1.0f,
    };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ImageCarouselEffect::drawRightTopTextureWithCoords(GLfloat* textureCoords)
{
    DelayTextureFrame* firstFrame = rightTopFrameBucket.front();
    if(1000 * (currentFramePos - firstFrame->getPosition()) >= DELAY_TIMEMILLS){
        rightTopFrameBucket.pop();
        leftBottomFrameBucket.push(firstFrame);
    }
    rightTopFramePos = firstFrame->getPosition();
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, firstFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    static const GLfloat vertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f,  1.0f,
        1.0f,  1.0f,
    };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ImageCarouselEffect::drawLeftBottomTextureWithCoords(GLfloat* textureCoords)
{
    DelayTextureFrame* frame;
    if(leftBottomFrameBucket.empty()){
        frame = rightTopFrameBucket.front();
    } else {
        frame = leftBottomFrameBucket.front();
        if(1000 * (rightTopFramePos - frame->getPosition()) >= DELAY_TIMEMILLS){
            leftBottomFrameBucket.pop();
            rightBottomFrameBucket.push(frame);
        }
    }
    leftBottomFramePos = frame->getPosition();
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    static const GLfloat vertices[] = {
        -1.0f, -1.0f,
        0.0f, -1.0f,
        -1.0f,  0.0f,
        0.0f,  0.0f,
    };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ImageCarouselEffect::drawRightBottomTextureWithCoords(GLfloat* textureCoords)
{
    bool isReleaseDelayTextureFrame = false;
    DelayTextureFrame* frame;
    if(rightBottomFrameBucket.empty()){
        frame = rightTopFrameBucket.front();
    } else {
        frame = rightBottomFrameBucket.front();
        if(1000 * (leftBottomFramePos - frame->getPosition()) >= DELAY_TIMEMILLS){
            rightBottomFrameBucket.pop();
            isReleaseDelayTextureFrame = true;
        }
    }
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    static const GLfloat vertices[] = {
        0.0f, -1.0f,
        1.0f, -1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
    };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    if(isReleaseDelayTextureFrame) {
        delete frame;
    }
}
