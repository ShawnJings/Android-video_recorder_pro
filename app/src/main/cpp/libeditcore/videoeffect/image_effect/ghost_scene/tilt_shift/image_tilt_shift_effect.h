//
//  tilt_shift_effect.hpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_tilt_shift_effect_h
#define image_tilt_shift_effect_h

#include "../../image_base_effect.h"
#include "../../../../gpu_texture_cache.h"
#include "../../../blur_scene/blur_scene_effect.h"

static char* TILT_SHIFT_VERTEX_SHADER_FILTER =
    "attribute vec4 position;                                                   \n"
    "attribute vec4 texcoord;                                                   \n"
    "attribute vec4 inputTextureCoordinate2;                                    \n"
    "varying vec2 textureCoordinate;                                            \n"
    "varying vec2 textureCoordinate2;                                           \n"
    "                                                                           \n"
    "void main() {                                                              \n"
    "                                                                           \n"
    "   gl_Position = position;                                                 \n"
    "   textureCoordinate = texcoord.xy;                                        \n"
    "   textureCoordinate2 = inputTextureCoordinate2.xy;                        \n"
    "                                                                           \n"
    "}\n";

static char* TILT_SHIFT_FRAG_SHADER_FILTER =
    "precision highp float;\n"
    "varying highp vec2 textureCoordinate;                                              \n"
    "varying highp vec2 textureCoordinate2;                                             \n"
    "                                                                                   \n"
    "uniform sampler2D yuvTexSampler;                                                   \n"
    "uniform sampler2D inputImageTexture2;                                              \n"
    "                                                                                   \n"
    "uniform highp float topFocusLevel;                                                 \n"
    "uniform highp float bottomFocusLevel;                                              \n"
    "uniform highp float focusFallOffRate;                                              \n"
    "                                                                                   \n"
    "const highp vec3 luminanceFactor = vec3(0.2125, 0.7154, 0.0721);                   \n"
    "                                                                                   \n"
    "void main() {                                                                      \n"
    "                                                                                   \n"
    "   lowp vec4 sharpImageColor = texture2D(yuvTexSampler, textureCoordinate);        \n"
    "   lowp vec4 blurredImageColor = texture2D(inputImageTexture2, textureCoordinate2);\n"
    "   float luminance = dot(blurredImageColor.rgb, luminanceFactor);                  \n"
    "   lowp vec4 blurLuminanceImageColor = vec4(vec3(luminance), blurredImageColor.a); \n"
    "   lowp float blurIntensity = 1.0 - smoothstep(topFocusLevel - focusFallOffRate, topFocusLevel, textureCoordinate2.x);\n"
    "   blurIntensity += smoothstep(bottomFocusLevel, bottomFocusLevel + focusFallOffRate, textureCoordinate2.x);\n"
    "   gl_FragColor = mix(sharpImageColor, blurLuminanceImageColor, blurIntensity);    \n"
    "                                                                                   \n"
    "}                                                                                  \n";

#define MOVE_SCREEN_WIDTH_DURATION_IN_TIMEMILLS         1500
#define MOVE_SLIDER_WIDTH_RATIO                         0.4f
#define FOCUS_FALL_OFF_RATE                             0.0f

class ImageTiltShiftEffect: public ImageBaseEffect {
public:
    ImageTiltShiftEffect();
    virtual ~ImageTiltShiftEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
    virtual bool init();
    void destroy();
    
private:
    GLuint texture2VertexCoords;
    
    GLint mGLUniformTexture2;
    float leftFocusLevel;
    float rightFocusLevel;
    float focusFallOffRate;
    GLint topFocusLevelUniform;
    GLint bottomFocusLevelUniform;
    GLint focusFallOffRateUniform;
    
    BlurSceneEffect* blurEffect;
    GPUTexture* blurFrameTexture;
    bool isInitFrameBufferFlag;
    GPUTexture* initFramebuffers(EffectCallback * filterCallback);
    
    float lastSolidFramePosition;
    OpenglVideoFrame * blurVideoFrame;
    void solidBlurVideoFrame(OpenglVideoFrame* inputFrame, EffectCallback* filterCallback);
    
    void calSliderPosition(float position);
    bool detectCpyBlurFrame();
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, OpenglVideoFrame * blurFrame, GLfloat* vertices, GLfloat* textureCoords);
};

#endif /* image_tilt_shift_effect_h */
