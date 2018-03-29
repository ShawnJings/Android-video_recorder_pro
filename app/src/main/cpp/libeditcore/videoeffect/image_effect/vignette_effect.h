#ifndef PROCESSOR_VIGNETTE_EFFECT_H
#define PROCESSOR_VIGNETTE_EFFECT_H

#include "./image_base_effect.h"

/**
 *
 * 晕影
 *
 *"uniform highp float vignetteStart;\n"
 *"uniform highp float vignetteEnd;\n"


 * **/
static char* VIGNETTE_EFFECT_FRAGMENT_SHADER =
		"precision highp float;\n"
		"uniform sampler2D yuvTexSampler;\n"
		"varying vec2 v_texcoord;\n"
		"\n"
		"uniform lowp vec2 vignetteCenter;\n"
		"uniform lowp vec3 vignetteColor;\n"
		"const float vignetteStart = 0.3;\n"
		"const float vignetteEnd = 0.75;\n"
		"\n"
		"void main()\n"
		"{\n"
		"lowp vec3 rgb = texture2D(yuvTexSampler, v_texcoord).rgb;\n"
		"lowp float d = distance(v_texcoord, vec2(vignetteCenter.x, vignetteCenter.y));\n"
		"lowp float percent = smoothstep(vignetteStart, vignetteEnd, d);\n"
		"gl_FragColor = vec4(mix(rgb.x, vignetteColor.x, percent), mix(rgb.y, vignetteColor.y, percent), mix(rgb.z, vignetteColor.z, percent), 1.0);\n"
		"}";

class VignetteEffect: public ImageBaseEffect {
public:
	VignetteEffect();
	virtual ~VignetteEffect();
    virtual bool init();

private:
	int mVignetteCenterLocation;
	int mVignetteColorLocation;
};

#endif // PROCESSOR_VIGNETTE_EFFECT_H
