#ifndef PROCESSOR_HUE_EFFECT_H
#define PROCESSOR_HUE_EFFECT_H

#include "./image_base_effect.h"

/**
 *
 * 色相变化的Effect
 *
 * **/
static char* HUE_EFFECT_FRAGMENT_SHADER =
    "precision highp float;                                                                     \n"
    "varying vec2 v_texcoord;                                                                   \n"
    "                                                                                           \n"
    "uniform sampler2D yuvTexSampler;                                                           \n"
    "uniform mediump float hueAdjust;                                                           \n"
    "const highp  vec4  kRGBToYPrime = vec4 (0.299, 0.587, 0.114, 0.0);                         \n"
    "const highp  vec4  kRGBToI     = vec4 (0.595716, -0.274453, -0.321263, 0.0);               \n"
    "const highp  vec4  kRGBToQ     = vec4 (0.211456, -0.522591, 0.31135, 0.0);                 \n"
    "                                                                                           \n"
    "const highp  vec4  kYIQToR   = vec4 (1.0, 0.9563, 0.6210, 0.0);                            \n"
    "const highp  vec4  kYIQToG   = vec4 (1.0, -0.2721, -0.6474, 0.0);                          \n"
    "const highp  vec4  kYIQToB   = vec4 (1.0, -1.1070, 1.7046, 0.0);                           \n"
    "                                                                                           \n"
    "void main ()                                                                               \n"
    "{                                                                                          \n"
    "    // Sample the input pixel                                                              \n"
    "    highp vec4 color   = texture2D(yuvTexSampler, v_texcoord);                             \n"
    "                                                                                           \n"
    "    // Convert to YIQ                                                                      \n"
    "    highp float   YPrime  = dot (color, kRGBToYPrime);                                     \n"
    "    highp float   I      = dot (color, kRGBToI);                                           \n"
    "    highp float   Q      = dot (color, kRGBToQ);                                           \n"
    "                                                                                           \n"
    "    // Calculate the hue and chroma                                                        \n"
    "    highp float   hue     = atan (Q, I);                                                   \n"
    "    highp float   chroma  = sqrt (I * I + Q * Q);                                          \n"
    "                                                                                           \n"
    "    // Make the user's adjustments                                                         \n"
    "    highp float hue1 = hue;                                                                \n"
    "    highp float weight = 1.0;                                                              \n"
    "                                                                                           \n"
    "    if ((hue1 < 0.2617993833) && (hue1 > -1.8325956833))                                   \n"
    "    {                                                                                      \n"
    "        hue1 += (-hueAdjust);                                                              \n"
    "                                                                                           \n"
    "        weight = hue;                                                                      \n"
    "        weight += 0.78539815;                                                              \n"
    "        weight = abs(weight);                                                              \n"
    "        weight /= 1.0471975333;                                                            \n"
    "        weight *= 4.0;                                                                     \n"
    "        weight -= 3.0;                                                                     \n"
    "        if (weight < 0.0)                                                                  \n"
    "        {                                                                                  \n"
    "            weight = 0.0;                                                                  \n"
    "        }                                                                                  \n"
    "        hue = hue1;                                                                        \n"
    "        Q = chroma * sin (hue);                                                            \n"
    "        I = chroma * cos (hue);                                                            \n"
    "                                                                                           \n"
    "        // Convert back to RGB                                                             \n"
    "        highp vec4    yIQ   = vec4 (YPrime, I, Q, 0.0);                                    \n"
    "        color.r = color.r * weight + (1.0 - weight) * dot (yIQ, kYIQToR);                  \n"
    "        color.g = color.g * weight + (1.0 - weight) * dot (yIQ, kYIQToG);                  \n"
    "        color.b = color.b * weight + (1.0 - weight) * dot (yIQ, kYIQToB);                  \n"
    "    }                                                                                      \n"
    "                                                                                           \n"
    "    // Convert back to YIQ                                                                 \n"
    "    // Save the result                                                                     \n"
    "    gl_FragColor = color;                                                                  \n"
    "}                                                                                          \n";

class HueEffect: public ImageBaseEffect {
public:
    HueEffect();
    virtual ~HueEffect();
    virtual bool init();
    void onDrawArraysPre(EffectCallback * filterCallback);
    
private:
    int hueAdjustUniform;
};


#endif /* PROCESSOR_HUE_EFFECT_H */
