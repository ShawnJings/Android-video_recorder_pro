#ifndef OVERLAY_SCENE_EFFECT_H
#define OVERLAY_SCENE_EFFECT_H
#include "../base_video_effect.h"

//Shader.frag文件内容
static char* OVERLAY_SCENE_FRAG_SHADER =
		"precision lowp float;\n "
		"varying highp vec2 textureCoordinate;\n "
		"\n "
		"uniform sampler2D yuvTexSampler;\n "
		"uniform sampler2D inputImageTexture2; //blowout;\n "
		"uniform sampler2D inputImageTexture3; //overlay;\n "
		"uniform sampler2D inputImageTexture4; //map\n "
		"\n "
		"void main()\n"
		"{\n    "
			"vec4 texel = texture2D(yuvTexSampler, textureCoordinate);\n    "
			"vec3 bbTexel = texture2D(inputImageTexture2, textureCoordinate).rgb;\n   "
			"\n    "
			"texel.r = texture2D(inputImageTexture3, vec2(bbTexel.r, texel.r)).r;\n    "
			"texel.g = texture2D(inputImageTexture3, vec2(bbTexel.g, texel.g)).g;\n   "
			"texel.b = texture2D(inputImageTexture3, vec2(bbTexel.b, texel.b)).b;\n  "
			"\n    "
			"vec4 mapped;\n    "
			"mapped.r = texel.r;\n    "
			"mapped.g = texel.g;\n    "
			"mapped.b = texel.b;\n    "
			"mapped.a = 1.0;\n    "
			"\n    "
			"gl_FragColor = mapped;\n"
		"}\n";

//Shader.vert文件内容
static char* OVERLAY_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 textureCoordinate;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   textureCoordinate = texcoord;  \n"
		"}                            \n";


class OverlaySceneEffect: public BaseVideoEffect {
private:
    GLint blackBoardSampler;
    GLint overlayMapSampler;
    GLint amaroMapSampler;

public:
    OverlaySceneEffect();
    virtual ~OverlaySceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // OVERLAY_SCENE_EFFECT_H
