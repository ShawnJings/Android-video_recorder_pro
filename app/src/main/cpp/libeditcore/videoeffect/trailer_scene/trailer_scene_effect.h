#ifndef TRAILER_SCENE_EFFECT_H
#define TRAILER_SCENE_EFFECT_H
#include "../../gpu_texture_cache.h"
#include "../base_video_effect.h"

static char* TRAILER_SCENE_OVERLAY_FRAG_SHADER =
		"precision lowp float;																		\n"
		"uniform float progress;																		\n"
		"uniform sampler2D yuvTexSampler;															\n"
		"uniform sampler2D overlayTexSampler;														\n"
		"varying vec2 v_texcoord;																\n"
		"varying vec2 v_overlayTexCoord;																\n"
		"float tt; \n"
		"																							\n"
		"void main()																					\n"
		"{																							\n"
		"	lowp vec4 fragColor = texture2D(yuvTexSampler, v_texcoord);																\n"
		"	vec4 overlay = texture2D(overlayTexSampler, v_overlayTexCoord);							\n"
		"	gl_FragColor.r = (progress * overlay.r) + (1.0 - overlay.a*progress) * fragColor.r; \n"
		"	gl_FragColor.g = (progress * overlay.g) + (1.0 - overlay.a*progress) * fragColor.g; \n"
		"	gl_FragColor.b = (progress * overlay.b) + (1.0 - overlay.a*progress) * fragColor.b; \n"
		"	gl_FragColor.a = 1.0;	\n"
		"}																							\n";
static char* TRAILER_SCENE_OVERLAY_VERTEX_SHADER =
		"attribute vec4 position;    			   												\n"
		"attribute vec2 texcoord;   	   															\n"
		"attribute vec2 overlay_texcoord;   														\n"
		"varying vec2 v_texcoord;																\n"
		"varying vec2 v_overlayTexCoord;   														\n"
		"																						\n"
		"void main(void)               															\n"
		"{                            															\n"
		"   gl_Position = position;  															\n"
		"   v_overlayTexCoord = overlay_texcoord;    											\n"
		"   v_texcoord = texcoord;    															\n"
		"}                            															\n";

class TrailerSceneEffect: public BaseVideoEffect {
private:
	/** Overlay Program attr&uniform **/
    GLuint attr_overlay_OverlayTextureCoords;
    GLint uniformLoc_overlay_Overlay_Texture;
    GLint uniformLoc_overlay_Progress;
public:
	TrailerSceneEffect();
    virtual ~TrailerSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // TRAILER_SCENE_EFFECT_H
