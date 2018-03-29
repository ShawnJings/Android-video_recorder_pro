#ifndef PROCESSOR_PLAYER_CONTRAST_EFFECT_H
#define PROCESSOR_PLAYER_CONTRAST_EFFECT_H

#include "./image_base_effect.h"

/** 亮丽
 * 这里的实现是float 类型的 contrast:1.75
 * 当然也可以通过:
 * 	 1:声明uniform lowp float contrast;
 * 	 2:location = glGetUniformLocation(getProgram(), "contrast");
 * 	 3:glUniform1f(location, floatValue);
 *
 * **/
static char* PLAYER_CONTRAST_EFFECT_FRAGMENT_SHADER =
	"precision mediump float;																									\n"
	"uniform sampler2D yuvTexSampler;																							\n"
	"varying vec2 v_texcoord;																									\n"
	"uniform mediump float contrast;																								\n"
	"void main() {																												\n"
	"  lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);															\n"
	"     																														\n"
	"  gl_FragColor = vec4((textureColor.rgb-0.45*(textureColor.rgb-vec3(0.65))*(textureColor.rgb-vec3(0.65))), textureColor.w);	\n"
	"}																															\n";

class PlayerContrastEffect: public ImageBaseEffect {
public:
	PlayerContrastEffect();
    virtual ~PlayerContrastEffect();

};

#endif // PROCESSOR_PLAYER_CONTRAST_EFFECT_H
