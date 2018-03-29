//
//  image_transform_effect.cpp
//  video_player
//
//  Created by apple on 16/10/17.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "image_transform_effect.h"

ImageTransformEffect::ImageTransformEffect(){
    mVertexShader = IMAGE_TRANSFORM_VERTEX_SHADER;
    mFragmentShader = IMAGE_TRANSFORM_FRAG_SHADER;
}
ImageTransformEffect::~ImageTransformEffect(){
}


bool ImageTransformEffect::init()
{
    if (BaseVideoEffect::init()) {
        mUniformTransforms = glGetUniformLocation(mGLProgId, "trans");
        checkGlError("glGetUniformLocation mUniformTransforms");
        return true;
    }
    return false;
}

void ImageTransformEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback)
{
    
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
    this->drawTextureWithVertices(inputFrame, outputFrame, (GLfloat*)imageVertices,
                                  (GLfloat*)textureCoordinates, filterCallback);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->switchFBODefaultAttachment();
}

void ImageTransformEffect::drawTextureWithVertices(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, GLfloat* vertices, GLfloat* textureCoords, EffectCallback * filterCallback)
{
    glUseProgram(mGLProgId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputFrame->getTextureId());
    glUniform1i(mGLUniformTexture, 0);
    
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, textureCoords);
    glEnableVertexAttribArray(mGLTextureCoords);
    
    ImagePosition imgPos;
    outputFrame->GetImagePosition(&imgPos);
    ParamVal paramValTextureWidth = getParamValHelper(string(IMAGE_TEXTURE_WIDTH), filterCallback);
    ParamVal paramValTextureHeight = getParamValHelper(string(IMAGE_TEXTURE_HEIGHT), filterCallback);
    ParamVal paramValX = getParamValHelper(string(IMAGE_TRANSFORM_TRANSLATE_XOFFSET), filterCallback);
    ParamVal paramValY = getParamValHelper(string(IMAGE_TRANSFORM_TRANSLATE_YOFFSET), filterCallback);
    ParamVal paramValWidth = getParamValHelper(string(IMAGE_TRANSFORM_SCALE_WIDTH), filterCallback);
    ParamVal paramValAngle = getParamValHelper(string(IMAGE_TRANSFORM_ROTATE_ANGLE), filterCallback);
    //构造缩放矩阵
    float scaleMatrix[4 * 4];
    matrixSetIdentityM(scaleMatrix);
    float widthRatio = (float)paramValWidth.u.intVal/(float)paramValTextureWidth.u.intVal;
    float widthScale = (float)paramValTextureWidth.u.intVal/(float)imgPos.width*widthRatio;
    float heightScale = (float)paramValTextureHeight.u.intVal/(float)imgPos.height*widthRatio;
    matrixScaleM(scaleMatrix, widthScale, heightScale, 1.0f);
    //构造旋转矩阵
    float rotateMatrix[4 * 4];
    matrixSetIdentityM(rotateMatrix);
    matrixRotateM(rotateMatrix, paramValAngle.u.fltVal, 0, 0, 1);
    //构造位移矩阵
    float translateMatrix[4 * 4];
    matrixSetIdentityM(translateMatrix);
    matrixTranslateM(translateMatrix, paramValX.u.fltVal, -1 * paramValY.u.fltVal, 0.0f);
    //组合位移与旋转矩阵
    float transformMatrix[4 * 4];
    matrixMultiplyMM(transformMatrix, translateMatrix, rotateMatrix);
    //将上一步的结果组合缩放矩阵, 注意这里的顺序其实S->R->T 由于矩阵相乘 所以倒着写
    float transformMatrix1[4 * 4];
    matrixMultiplyMM(transformMatrix1, transformMatrix, scaleMatrix);
    //最后将结果矩阵传递给OpenGL
    glUniformMatrix4fv(mUniformTransforms, 1, GL_FALSE, (GLfloat *) transformMatrix1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
