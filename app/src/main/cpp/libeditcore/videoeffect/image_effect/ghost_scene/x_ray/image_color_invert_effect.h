//
//  color_invert_effect.hpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_color_invert_effect_h
#define image_color_invert_effect_h

#include "../../image_base_effect.h"
#include "../../../../gpu_texture_cache.h"

static char* COLOR_INVERT_FRAG_SHADER_FILTER =
    "precision highp float;                                                                     \n"
    "varying highp vec2 v_texcoord;                                                             \n"
    "uniform sampler2D yuvTexSampler;                                                           \n"
    "uniform bool colorInvertFlag;                                                              \n"
    "                                                                                           \n"
    "void main() {                                                                              \n"
    "   lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);                          \n"
    "   if(colorInvertFlag) {                                                                   \n"
    "       gl_FragColor = vec4((1.0 - textureColor.rgb), textureColor.w);                      \n"
    "   } else {                                                                                \n"
    "       gl_FragColor = vec4(textureColor.rgb, textureColor.w);                              \n"
    "   }                                                                                       \n"
    "}                                                                                          \n";

#define COLOR_INVERT_FRAME_RENDER_DURATION_IN_TIMEMILLS                                         180
#define COMMON_FRAME_RENDER_DURATION_IN_TIMEMILLS                                               250

class ImageColorInvertEffect: public ImageBaseEffect {
public:
    ImageColorInvertEffect();
    virtual ~ImageColorInvertEffect();
    
    virtual bool init();
    void destroy();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
private:
    GLint colorInvertFlagUniform;
    
    GPUTexture* colorInvertFrameTexture;
    bool isInitFrameBufferFlag;
    GPUTexture* initFramebuffers(EffectCallback * filterCallback);
    OpenglVideoFrame * colorInvertVideoFrame;
    
    float lastColorInvertFrameRenderPos;
    OpenglVideoFrame* getRenderFrame(OpenglVideoFrame* inputFrame);
    bool detectCpyColorInvertFrame(float position);
    void cpyColorInvertTexture(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords);
    
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords);
};

#endif /* image_color_invert_effect_h */
