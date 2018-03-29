#ifndef TEXT_SCENE_EFFECT_H
#define TEXT_SCENE_EFFECT_H
#include "../base_video_effect.h"

//Shader.frag文件内容
static char* TEXT_SCENE_FRAG_SHADER =
		"precision highp float;																				\n"
		"varying highp vec2 v_texcoord;																		\n"
		"uniform sampler2D yuvTexSampler;																	\n"
		"uniform sampler2D overlayTexSampler;																\n"
		"varying vec2 overlayTexCoord;																		\n"
		"varying float v_progress;																			\n"
		"void main() {																						\n"
		"	vec4 fragColor = texture2D(yuvTexSampler, v_texcoord);												\n"
		"	vec4 textOverlay = texture2D(overlayTexSampler, overlayTexCoord);								\n"
		"	gl_FragColor.r = (v_progress * textOverlay.r) + (1.0 - textOverlay.a*v_progress) * fragColor.r; \n"
		"	gl_FragColor.g = (v_progress * textOverlay.g) + (1.0 - textOverlay.a*v_progress) * fragColor.g; \n"
		"	gl_FragColor.b = (v_progress * textOverlay.b) + (1.0 - textOverlay.a*v_progress) * fragColor.b; \n"
		"	gl_FragColor.a = 1.0;	\n"
		"}																									\n";

//"	mediump vec4 whiteColor = vec4(1.0);																\n"
//"	gl_FragColor = whiteColor - ((whiteColor - textOverlay * v_progress) * (whiteColor - fragColor));\n"

//Shader.vert文件内容
static char* TEXT_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    															\n"
		"attribute vec2 texcoord;   																\n"
		"attribute vec2 overlay_texcoord;   														\n"
		"attribute float progress;   															\n"
		"varying vec2 v_texcoord;     															\n"
		"varying vec2 overlayTexCoord;   														\n"
		"varying float v_progress;  																\n"
		"void main(void)               															\n"
		"{                            															\n"
		"   gl_Position = position;  															\n"
		"   v_texcoord = texcoord;  																\n"
		"   v_progress = progress;    															\n"
		"   overlayTexCoord = overlay_texcoord;    												\n"
		"}                            															\n";


class TextSceneEffect: public BaseVideoEffect {
private:
    GLuint attr_ver_Progress;
    GLuint attr_ver_OverlayTextureCoords;
    GLint uniformLoc_ver_text_Sampler;

public:
    TextSceneEffect();
    virtual ~TextSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // TEXT_SCENE_EFFECT_H
