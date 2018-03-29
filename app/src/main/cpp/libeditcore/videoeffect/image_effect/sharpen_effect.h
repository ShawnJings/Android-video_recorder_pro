//
//  sharpen_effect.hpp
//  liveDemo
//
//  Created by apple on 16/5/12.
//  Copyright © 2016年 changba. All rights reserved.
//

#ifndef PROCESSOR_SHARPEN_EFFECT_H
#define PROCESSOR_SHARPEN_EFFECT_H

#include "./image_base_effect.h"

static char* SHARPEN_EFFECT_VERTEX_SHADER =
"                                                                                           \n"
"attribute vec4 position;                                                                   \n"
"attribute vec4 texcoord;                                                                   \n"
"                                                                                           \n"
"uniform float imageWidthFactor;                                                            \n"
"uniform float imageHeightFactor;                                                           \n"
"uniform float sharpness;                                                                   \n"
"                                                                                           \n"
"varying vec2 textureCoordinate;                                                            \n"
"varying vec2 leftTextureCoordinate;                                                        \n"
"varying vec2 rightTextureCoordinate;                                                       \n"
"varying vec2 topTextureCoordinate;                                                         \n"
"varying vec2 bottomTextureCoordinate;                                                      \n"
"                                                                                           \n"
"varying float centerMultiplier;                                                            \n"
"varying float edgeMultiplier;                                                              \n"
"                                                                                           \n"
"void main()                                                                                \n"
"{                                                                                          \n"
"    gl_Position = position;                                                                \n"
"                                                                                           \n"
"    vec2 widthStep = vec2(imageWidthFactor, 0.0);                                          \n"
"    vec2 heightStep = vec2(0.0, imageHeightFactor);                                        \n"
"                                                                                           \n"
"    textureCoordinate = texcoord.xy;                                                       \n"
"    leftTextureCoordinate = texcoord.xy - widthStep;                                       \n"
"    rightTextureCoordinate = texcoord.xy + widthStep;                                      \n"
"    topTextureCoordinate = texcoord.xy + heightStep;                                       \n"
"    bottomTextureCoordinate = texcoord.xy - heightStep;                                    \n"
"                                                                                           \n"
"    centerMultiplier = 1.0 + 4.0 * sharpness;                                              \n"
"    edgeMultiplier = sharpness;                                                            \n"
"}                                                                                          \n";

/**
 *
 * 锐化的Effect
 *
 * **/
static char* SHARPEN_EFFECT_FRAGMENT_SHADER =
"precision highp float;                                                                                    \n"
"                                                                                                          \n"
"varying highp vec2 textureCoordinate;                                                                     \n"
"varying highp vec2 leftTextureCoordinate;                                                                 \n"
"varying highp vec2 rightTextureCoordinate;                                                                \n"
"varying highp vec2 topTextureCoordinate;                                                                  \n"
"varying highp vec2 bottomTextureCoordinate;                                                               \n"
"                                                                                                          \n"
"varying highp float centerMultiplier;                                                                     \n"
"varying highp float edgeMultiplier;                                                                       \n"
"                                                                                                          \n"
"uniform sampler2D yuvTexSampler;                                                                          \n"
"                                                                                                          \n"
"void main()                                                                                               \n"
"{                                                                                                         \n"
"   mediump vec3 textureColor = texture2D(yuvTexSampler, textureCoordinate).rgb;                       \n"
"   mediump vec3 leftTextureColor = texture2D(yuvTexSampler, leftTextureCoordinate).rgb;               \n"
"    mediump vec3 rightTextureColor = texture2D(yuvTexSampler, rightTextureCoordinate).rgb;            \n"
"    mediump vec3 topTextureColor = texture2D(yuvTexSampler, topTextureCoordinate).rgb;                \n"
"    mediump vec3 bottomTextureColor = texture2D(yuvTexSampler, bottomTextureCoordinate).rgb;          \n"
"                                                                                                          \n"
"    gl_FragColor = vec4((textureColor * centerMultiplier - (leftTextureColor * edgeMultiplier + rightTextureColor * edgeMultiplier + topTextureColor * edgeMultiplier + bottomTextureColor * edgeMultiplier)), texture2D(yuvTexSampler, bottomTextureCoordinate).w);\n"
"}                                                                                                         \n";

class SharpenEffect: public ImageBaseEffect {
public:
    SharpenEffect();
    virtual ~SharpenEffect();
    virtual bool init();
    void onDrawArraysPre(EffectCallback * filterCallback);
    
private:
    int sharpnessUniform;
    int imageWidthFactorUniform;
    int imageHeightFactorUniform;
    
};



#endif /* PROCESSOR_SHARPEN_EFFECT_H */
