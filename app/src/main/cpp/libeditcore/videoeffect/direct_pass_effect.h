#ifndef DIRECT_PASS_EFFECT_H
#define DIRECT_PASS_EFFECT_H
#include "base_video_effect.h"

static char* VERTEX_SHADER_FILTER1 =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 v_texcoord;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   v_texcoord = texcoord;  \n"
		"}                            \n";

static char* FRAG_SHADER_FILTER1 =
		"precision highp float;\n"
	    "varying highp vec2 v_texcoord;\n"
	    "uniform sampler2D yuvTexSampler;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
		"}\n";

class DirectPassEffect : public BaseVideoEffect {
private:
    GLfloat* mGLVertexBuffer;
    GLfloat* mGLTextureBuffer;
public:
	DirectPassEffect();
    virtual ~DirectPassEffect();

    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
};

#endif // DIRECT_PASS_EFFECT_H
