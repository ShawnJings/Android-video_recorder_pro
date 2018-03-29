//
//  image_4mirror_effect.cpp
//  video_player
//
//  Created by apple on 16/9/29.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_2mirror_effect.h"

Image2MirrorEffect::Image2MirrorEffect(){
    mVertexShader = VERTEX_SHADER_FILTER;
    mFragmentShader = FRAG_SHADER_FILTER;
}
Image2MirrorEffect::~Image2MirrorEffect(){
}

void Image2MirrorEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    
    this->switchFBOAttachmentTexture(outputFrame->getTextureId());

    this->drawTextureWithVertices(inputFrame, (GLfloat*)leftImageVertices,
                                  (GLfloat*)leftTextureCoordinates);
    this->drawTextureWithVertices(inputFrame, (GLfloat*)rightImageVertices,
                                  (GLfloat*)rightTextureCoordinates);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

void Image2MirrorEffect::drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords)
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