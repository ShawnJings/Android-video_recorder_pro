//
//  image_transform_effect.hpp
//  video_player
//
//  Created by apple on 16/10/17.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_transform_effect_hpp
#define image_transform_effect_hpp

#include "../image_base_effect.h"

//Shader.vert文件内容
static char* IMAGE_TRANSFORM_VERTEX_SHADER =
    "attribute vec4 position;                                                                                               \n"
    "attribute vec2 texcoord;                                                                                               \n"
    "varying vec2 v_texcoord;                                                                                               \n"
    "uniform mat4 trans;                                                                                                    \n"
    "                                                                                                                       \n"
    "void main(void)                                                                                                        \n"
    "{                                                                                                                      \n"
    "   gl_Position = trans * position;                                                                                     \n"
    "   v_texcoord = texcoord;                                                                                              \n"
    "}                                                                                                                      \n";

//Shader.frag文件内容
static char* IMAGE_TRANSFORM_FRAG_SHADER =
    "precision highp float;                                                                                                 \n"
    "varying highp vec2 v_texcoord;                                                                                         \n"
    "uniform sampler2D yuvTexSampler;                                                                                       \n"
    "                                                                                                                       \n"
    "void main() {                                                                                                          \n"
    "   vec4 fragColor = texture2D(yuvTexSampler, v_texcoord);                                                              \n"
    "   if(fragColor.a > 0.001){                                                                                            \n"
    "       fragColor = vec4(fragColor.r / fragColor.a, fragColor.g / fragColor.a, fragColor.b / fragColor.a, fragColor.a); \n"
    "   }                                                                                                                   \n"
    "   gl_FragColor = fragColor;                                                                                           \n"
    "}                                                                                                                      \n";

class ImageTransformEffect: public ImageBaseEffect {
public:
    ImageTransformEffect();
    virtual ~ImageTransformEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
    virtual bool init();
    
private:
    GLint mUniformTransforms;
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, GLfloat* vertices, GLfloat* textureCoords, EffectCallback * filterCallback);
};

#endif /* image_transform_effect_hpp */
