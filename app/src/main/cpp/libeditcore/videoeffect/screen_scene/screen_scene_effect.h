#ifndef SCREEN_SCENE_EFFECT_H
#define SCREEN_SCENE_EFFECT_H
#include "../base_video_effect.h"

//Shader.frag文件内容
static char* SCREEN_SCENE_FRAG_SHADER =
		"precision highp float;\n"
		"uniform sampler2D yuvTexSampler; //video\n"
		"uniform sampler2D inputImageTexture2; //screen\n"
		"varying vec2 textureCoordinate;\n"
		"void main()\n"
		"{\n"
			"vec4 video = texture2D(yuvTexSampler, textureCoordinate);\n"
			"vec4 screen = texture2D(inputImageTexture2, textureCoordinate);\n"
			"mediump vec4 whiteColor = vec4(1.0);\n"
			"gl_FragColor = whiteColor - ((whiteColor - screen) * (whiteColor - video));\n"
		"}\n";

//Shader.vert文件内容
static char* SCREEN_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 textureCoordinate;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   textureCoordinate = texcoord;  \n"
		"}                            \n";


class ScreenSceneEffect: public BaseVideoEffect {
private:
	GLint uniformLoc_screen_sampler;

public:
	ScreenSceneEffect();
    virtual ~ScreenSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // SCREEN_SCENE_EFFECT_H
