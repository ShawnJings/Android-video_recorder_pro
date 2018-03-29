#ifndef PROCESSOR_GRAYSCALE_EFFECT_H
#define PROCESSOR_GRAYSCALE_EFFECT_H

#include "./image_base_effect.h"

/**
 *
 * 灰度
 *
 * **/
static char* GRAYSCALE_EFFECT_FRAGMENT_SHADER =
	"precision mediump float;\n"
	"uniform sampler2D yuvTexSampler;\n"
	"varying vec2 v_texcoord;\n"
	"const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);\n"
	"void main(){\n"
	"	lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);\n"
	"	float luminance = dot(textureColor.rgb, W);\n"
	"	gl_FragColor = vec4(vec3(luminance), textureColor.a);\n"
	"}\n";

class GrayScaleEffect: public ImageBaseEffect {
public:
	GrayScaleEffect();
    virtual ~GrayScaleEffect();

};

#endif // PROCESSOR_GRAYSCALE_EFFECT_H
