#ifndef MASK_ALPHA_EFFECT_H
#define MASK_ALPHA_EFFECT_H
#include "../base_video_effect.h"

//Shader.frag文件内容
static char* MASK_ALPHA_FRAG_SHADER =
		"precision highp float;														\n"
		"uniform sampler2D yuvTexSampler;  //video									\n"
		"uniform sampler2D inputImageTexture2; //mv									\n"
		"uniform sampler2D inputImageTexture3; //alpha								\n"
		"varying vec2 textureCoordinate;												\n"
		"void main()																	\n"
		"{																			\n"
			"vec4 video = texture2D(yuvTexSampler, textureCoordinate);				\n"
			"vec4 mv    = texture2D(inputImageTexture2, textureCoordinate);			\n"
			"vec4 alpha = texture2D(inputImageTexture3, textureCoordinate);			\n"
			"gl_FragColor = video * (1.0 - alpha.r) + mv;							\n"
		"}\n";

//Shader.vert文件内容
static char* MASK_ALPHA_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 textureCoordinate;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   textureCoordinate = texcoord;  \n"
		"}                            \n";


class MaskAlphaEffect: public BaseVideoEffect {
private:
	GLint uniformLoc_mask_alpha_sampler;
	GLint uniformLoc_mask_sampler;

public:
    MaskAlphaEffect();
    virtual ~MaskAlphaEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // MASK_ALPHA_EFFECT_H
