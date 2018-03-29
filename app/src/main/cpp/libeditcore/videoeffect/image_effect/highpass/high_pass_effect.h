//
//  high_pass_effect.hpp
//  liveDemo
//
//  Created by apple on 16/4/5.
//  Copyright © 2016年 changba. All rights reserved.
//

#ifndef IMAGE_EFFECT_HIGH_PASS_EFFECT_H
#define IMAGE_EFFECT_HIGH_PASS_EFFECT_H

#include "../../../gpu_texture_cache.h"
#include "../../base_video_effect.h"
#include "./high_pass_blur_shader.glsl"

class HighPassEffect: public BaseVideoEffect {
private:
    /** Horizontal Program extra uniform **/
    GLint uniformLoc_hor_texelWidthOffset;
    GLint uniformLoc_hor_texelHeightOffset;
    
    /** 中间的临时纹理ID **/
    GPUTexture* blurTempTexCache;
    
    /** Vertical Program attr&uniform **/
    GLuint verticalProgId;
    GLuint attr_ver_VertexCoords;
    GLuint attr_ver_TextureCoords;
    GLint uniformLoc_ver_Texture;
    GLint uniformLoc_ver_texelWidthOffset;
    GLint uniformLoc_ver_texelHeightOffset;
    
    /** Alpha Blur Horizontal Program attr&uniform **/
    GLuint alphaBlurHorProgId;
    GLuint attr_alpha_blur_hor_VertexCoords;
    GLuint attr_alpha_blur_hor_TextureCoords;
    GLint uniformLoc_alpha_blur_hor_Texture;
    GLint uniformLoc_alpha_blur_hor_texelWidthOffset;
    GLint uniformLoc_alpha_blur_hor_texelHeightOffset;
    
    /** Alpha Blur Vertical Program attr&uniform **/
    GLuint alphaBlurVerProgId;
    GLuint attr_alpha_blur_ver_VertexCoords;
    GLuint attr_alpha_blur_ver_TextureCoords;
    GLint uniformLoc_alpha_blur_ver_Texture;
    GLint uniformLoc_alpha_blur_ver_texelWidthOffset;
    GLint uniformLoc_alpha_blur_ver_texelHeightOffset;
    
    
public:
    HighPassEffect();
    virtual ~HighPassEffect();
    
    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords);
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();

};

#endif /* IMAGE_EFFECT_HIGH_PASS_EFFECT_H */
