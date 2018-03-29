#ifndef HEADER_FADE_SCENE_EFFECT_H
#define HEADER_FADE_SCENE_EFFECT_H
#include "../base_video_effect.h"

//Shader.frag文件内容
static char* HEADER_FADE_SCENE_FRAG_SHADER =
		"precision lowp float;																			\n"
		"varying highp vec2 textureCoordinate;															\n"
		"																								\n"
		"uniform sampler2D yuvTexSampler;																\n"
		"uniform sampler2D headerTexSampler;																\n"
		"uniform float progress;																			\n"
		"																								\n"
		"void main()																						\n"
		"{																								\n"
		"	lowp vec4 originalVideoFrame = texture2D(yuvTexSampler, textureCoordinate);					\n"
		"	lowp vec4 headerVideoFrame = texture2D(headerTexSampler, textureCoordinate);					\n"
		"	gl_FragColor = mix(headerVideoFrame, originalVideoFrame, progress);							\n"
		"}																								\n";

//Shader.vert文件内容
static char* HEADER_FADE_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 textureCoordinate;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   textureCoordinate = texcoord;  \n"
		"}                            \n";


class HeaderFadeSceneEffect: public BaseVideoEffect {
private:
    GLint headerTexSampler;
    GLint uniformLoc_progress;

public:
    HeaderFadeSceneEffect();
    virtual ~HeaderFadeSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // HEADER_FADE_SCENE_EFFECT_H
