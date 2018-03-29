#ifndef PROCESSOR_CONTRAST_EFFECT_H
#define PROCESSOR_CONTRAST_EFFECT_H

#include "./image_base_effect.h"

/** 亮丽
 * 这里的实现是float 类型的 contrast:1.75
 * 当然也可以通过:
 * 	 1:声明uniform lowp float contrast;
 * 	 2:location = glGetUniformLocation(getProgram(), "contrast");
 * 	 3:glUniform1f(location, floatValue);
 *
 * **/
static char* CONTRAST_EFFECT_FRAGMENT_SHADER =
	"precision mediump float;\n"
	"uniform sampler2D yuvTexSampler;\n"
	"varying vec2 v_texcoord;\n"
	"uniform mediump float contrast;\n"
	"void main() {\n"
	"  lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);\n"
	"     \n"
	"  gl_FragColor = vec4(((textureColor.rgb - vec3(0.5)) * contrast + vec3(0.5)), textureColor.w);\n"
	"}\n";

class ContrastEffect: public ImageBaseEffect {
public:
	ContrastEffect();
    virtual ~ContrastEffect();

    virtual bool init();
    void onDrawArraysPre(EffectCallback * filterCallback);

private:
    int contrastSlopeUniform;
};

#endif // PROCESSOR_CONTRAST_EFFECT_H
