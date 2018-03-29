#ifndef TRANSITION_SCENE_EFFECT_H
#define TRANSITION_SCENE_EFFECT_H
#include "../base_video_effect.h"

/**
 * mix(x, y, a)
 * returns the linear blend of x and y, i.e.
 * x*(1-a) + y*a
 */
//Shader.frag文件内容
static char* TRANSITION_SCENE_FRAG_SHADER =
	"precision lowp float;\n"
	"varying vec2 srcTexCoord;\n"
	"varying vec2 dstTexCoord;\n"
	"uniform sampler2D yuvTexSampler;\n"
	"uniform sampler2D dstSampler;\n"
	"uniform float progress;\n"
	"void main()\n"
	"{\n"
	"    lowp vec4 srcColor = texture2D(yuvTexSampler, srcTexCoord);\n"
	"    lowp vec4 dstColor = texture2D(dstSampler, dstTexCoord);\n"
	"    gl_FragColor = mix(srcColor, dstColor, progress);\n"
	"}\n";

//Shader.vert文件内容
static char* TRANSITION_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"attribute vec2 dstTexCoordAttr;\n"
		"varying highp vec2 srcTexCoord;\n"
		"varying highp vec2 dstTexCoord;\n"
		"void main(void)               \n"
		"{                            \n"
		"   srcTexCoord = texcoord;  \n"
		"   dstTexCoord = dstTexCoordAttr;\n"
		"   gl_Position = position;  \n"
		"}                            \n";

class TransitionSceneEffect: public BaseVideoEffect {
private:
	GLint uniformLoc_dstSampler;
	GLint uniformLoc_progress;
	GLint attrLoc_dstTexCoord;

public:
    TransitionSceneEffect();
    virtual ~TransitionSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // TRANSITION_SCENE_EFFECT_H
