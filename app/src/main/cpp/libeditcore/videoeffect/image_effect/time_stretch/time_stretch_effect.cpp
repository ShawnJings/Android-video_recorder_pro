//
//  image_4mirror_effect.cpp
//  video_player
//
//  Created by apple on 16/9/29.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "time_stretch_effect.h"

TimeStretchEffect::TimeStretchEffect(){
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = FRAG_SHADER_FILTER;
}
TimeStretchEffect::~TimeStretchEffect(){
}

void TimeStretchEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    
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
    this->drawTextureWithVertices(inputFrame, (GLfloat*)imageVertices,
                                  (GLfloat*)textureCoordinates);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

void TimeStretchEffect::drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords)
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