#ifndef HEADER_SCENE_EFFECT_H
#define HEADER_SCENE_EFFECT_H
#include "../base_video_effect.h"

//Shader.frag文件内容
static char* HEADER_SCENE_FRAG_SHADER =
		"precision lowp float;																			\n"
		"varying highp vec2 textureCoordinate;															\n"
		"																								\n"
		"uniform sampler2D yuvTexSampler;																\n"
		"uniform sampler2D headerTexSampler;																\n"
		"uniform sampler2D screenSampler;																\n"
		"																								\n"
		"void main()																						\n"
		"{																								\n"
		"	lowp vec4 originalVideoFrame = texture2D(yuvTexSampler, textureCoordinate);					\n"
		"	lowp vec3 headerVideoFrame = texture2D(headerTexSampler, textureCoordinate).rgb;				\n"
		"	vec4 targetFrame;																			\n"
		"	targetFrame.r = texture2D(screenSampler, vec2(headerVideoFrame.r, originalVideoFrame.r)).r;	\n"
		"	targetFrame.g = texture2D(screenSampler, vec2(headerVideoFrame.g, originalVideoFrame.g)).g;	\n"
		"	targetFrame.b = texture2D(screenSampler, vec2(headerVideoFrame.b, originalVideoFrame.b)).b;	\n"
		"	targetFrame.a = 1.0;																			\n"
		"																								\n"
		"	gl_FragColor = targetFrame;																	\n"
		"}																								\n";

//Shader.vert文件内容
static char* HEADER_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 textureCoordinate;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   textureCoordinate = texcoord;  \n"
		"}                            \n";


class HeaderSceneEffect: public BaseVideoEffect {
private:
    GLint headerTexSampler;
    GLint screenSampler;

public:
    HeaderSceneEffect();
    virtual ~HeaderSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // HEADER_SCENE_EFFECT_H
