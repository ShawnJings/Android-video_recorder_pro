#ifndef SATURATION_SCENE_EFFECT_H
#define SATURATION_SCENE_EFFECT_H
#include "../base_video_effect.h"

/** 饱和度调节(0.0-2.0 默认效果1.0) **/
static char* SATURATION_EFFECT_FRAGMENT_SHADER =
			" precision highp float;																				\n"
			" varying highp vec2 v_texcoord;																		\n"
			" 																									\n"
			" uniform sampler2D yuvTexSampler;																	\n"
			" uniform lowp float saturation;																		\n"
			" 																									\n"
			" // Values from \"Graphics Shaders: Theory and Practice\" by Bailey and Cunningham					\n"
			" const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);								\n"
			" 																									\n"
			" void main()																						\n"
			" {																									\n"
			"    lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);									\n"
			"    lowp float luminance = dot(textureColor.rgb, luminanceWeighting);								\n"
			"    lowp vec3 greyScaleColor = vec3(luminance);														\n"
			"    																								\n"
			"    gl_FragColor = vec4(mix(greyScaleColor, textureColor.rgb, saturation), textureColor.w);			\n"
			"     																								\n"
			" }";

class SaturationEffect: public BaseVideoEffect {
public:
	SaturationEffect();
    virtual ~SaturationEffect();

    virtual bool init();

    virtual void onDrawArraysPre(EffectCallback* filterCallback);
private:
    int mSaturationLocation;
};

#endif // SATURATION_SCENE_EFFECT_H
