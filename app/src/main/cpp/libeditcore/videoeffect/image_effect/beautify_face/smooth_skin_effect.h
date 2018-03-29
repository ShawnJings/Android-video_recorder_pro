//
//  high_pass_effect.hpp
//  liveDemo
//
//  Created by apple on 16/4/5.
//  Copyright © 2016年 changba. All rights reserved.
//

#ifndef IMAGE_EFFECT_SMOOTH_SKIN_EFFECT_H
#define IMAGE_EFFECT_SMOOTH_SKIN_EFFECT_H

#include "../../../gpu_texture_cache.h"
#include "../../base_video_effect.h"
#include "./smooth_skin_shader.glsl"
#include "./optimized_skin_shader.glsl"

class SmoothSkinEffect: public BaseVideoEffect {
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
    GLint maskCurveProgressUniform;
    GLint softLightProgressUniform;
    GLint sCurveProgressUniform;
    GLint satuRatioUniform;
    
    float getHighpassRadius(){
        #ifdef __ANDROID__
                return 2.0f;
        #elif defined(__APPLE__)	// IOS or OSX
                return 2.5f;
        #endif
    };
    
    char* getHorizontalSkinVertexShader(){
        #ifdef __ANDROID__
                return OPTIMIZED_SKIN_HORIZONTAL_VERTEX_SHADER;
        #elif defined(__APPLE__)	// IOS or OSX
                return SMOOTH_SKIN_HORIZONTAL_VERTEX_SHADER;
        #endif
    };
    char* getHorizontalSkinFragmentShader(){
        #ifdef __ANDROID__
                return OPTIMIZED_SKIN_HORIZONTAL_FRAG_SHADER;
        #elif defined(__APPLE__)	// IOS or OSX
                return SMOOTH_SKIN_HORIZONTAL_FRAG_SHADER;
        #endif
    };
    char* getVerticalSkinVertexShader(){
        #ifdef __ANDROID__
                return OPTIMIZED_SKIN_VERTICAL_VERTEX_SHADER;
        #elif defined(__APPLE__)	// IOS or OSX
                return SMOOTH_SKIN_VERTICAL_VERTEX_SHADER;
        #endif
    };
    char* getVerticalSkinFragmentShader(){
        #ifdef __ANDROID__
                return OPTIMIZED_SKIN_VERTICAL_FRAG_SHADER;
        #elif defined(__APPLE__)	// IOS or OSX
                return SMOOTH_SKIN_VERTICAL_FRAG_SHADER;
        #endif
    };
    
public:
    SmoothSkinEffect();
    virtual ~SmoothSkinEffect();
    
    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords);
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
    
};

#endif /* IMAGE_EFFECT_SMOOTH_SKIN_EFFECT_H */
