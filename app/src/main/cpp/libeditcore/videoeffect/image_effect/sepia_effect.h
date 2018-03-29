#ifndef PROCESSOR_SEPIA_EFFECT_H
#define PROCESSOR_SEPIA_EFFECT_H

#include "./image_base_effect.h"

/**
 *
 * 怀旧
 *
 * **/
static char* SEPIA_EFFECT_FRAGMENT_SHADER =
		"precision mediump float;\n"
		"uniform sampler2D yuvTexSampler;\n"
		"varying vec2 v_texcoord;\n"
		"\n"
		"uniform lowp mat4 colorMatrix;\n"
		"uniform lowp float intensity;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);\n"
		"   lowp vec4 outputColor = textureColor * colorMatrix;\n"
		"   \n"
		"   gl_FragColor = (intensity * outputColor) + ((1.0 - intensity) * textureColor);\n"
		"}";

class SepiaEffect: public ImageBaseEffect {
public:
	SepiaEffect();
	virtual ~SepiaEffect();
    virtual bool init();

private:
	float mIntensity;
	float* mColorMatrix;
	int mColorMatrixLocation;
	int mIntensityLocation;
};

#endif // PROCESSOR_SEPIA_EFFECT_H
