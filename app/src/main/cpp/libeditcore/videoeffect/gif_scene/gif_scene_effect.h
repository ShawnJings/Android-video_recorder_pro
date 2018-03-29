#ifndef GIF_SCENE_EFFECT_H
#define GIF_SCENE_EFFECT_H
#include "../base_video_effect.h"

//Shader.vert文件内容
static char* GIF_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 v_texcoord;     \n"
		"uniform mat4 trans; \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = trans * position;  \n"
		"   v_texcoord = texcoord;  \n"
		"}                            \n";

//Shader.frag文件内容
static char* GIF_SCENE_FRAG_SHADER =
		"precision highp float;\n"
		"varying highp vec2 v_texcoord;\n"
		"uniform sampler2D yuvTexSampler;\n"
		"void main() {\n"
			"gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
		"}\n";

class GifSceneEffect: public BaseVideoEffect {
private:
	GLuint cpyTextureProgId;
    GLuint cpyTexturePosition;
    GLuint cpyTextureCoords;
    GLint cpyTextureUniformSampler;

    GLint mUniformTransforms;

public:
	GifSceneEffect();
    virtual ~GifSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();

//protected:
//    void enableVertexCoord(float x, float y, float width, int gifWidth, int gifHeight);
};

#endif // GIF_SCENE_EFFECT_H
