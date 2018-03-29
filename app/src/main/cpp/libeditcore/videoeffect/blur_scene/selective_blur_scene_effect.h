#ifndef SELECTIVE_BLUR_SCENE_EFFECT_H
#define SELECTIVE_BLUR_SCENE_EFFECT_H

#include "./blur_scene_effect.h"

//Shader.vert文件内容
static char* SELECTIVE_BLUR_SCENE_VERTEX_SHADER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"attribute vec2 texcoord2;   \n"
		"varying vec2 v_texcoord;     \n"
		"varying vec2 v_texcoord2;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   v_texcoord = texcoord;  \n"
		"   v_texcoord2 = texcoord2;  \n"
		"}                            \n";

//Shader.frag文件内容
static char* SELECTIVE_BLUR_SCENE_FRAG_SHADER =
		"precision highp float;																																	\n"
		"varying highp vec2 v_texcoord;																															\n"
		"varying highp vec2 v_texcoord2;																															\n"
		"uniform sampler2D yuvTexSampler;																														\n"
		"uniform sampler2D yuvTexSampler2;																														\n"
		"																																						\n"
		"uniform lowp float excludeCircleRadius;																													\n"
		"uniform lowp vec2 excludeCirclePoint;																													\n"
		"uniform lowp float excludeBlurSize;																														\n"
		"uniform highp float aspectRatio;																														\n"
		"void main() {																																			\n"
		"	lowp vec4 sharpImageColor = texture2D(yuvTexSampler, v_texcoord);																					\n"
		"	lowp vec4 blurredImageColor = texture2D(yuvTexSampler2, v_texcoord2);																				\n"
		"																																						\n"
		"	highp vec2 textureCoordinateToUse = vec2(v_texcoord2.x, (v_texcoord2.y * aspectRatio + 0.5 - 0.5 * aspectRatio));						\n"
		"	highp float distanceFromCenter = distance(excludeCirclePoint, textureCoordinateToUse);																\n"
		"	gl_FragColor = mix(sharpImageColor, blurredImageColor, smoothstep(excludeCircleRadius - excludeBlurSize, excludeCircleRadius, distanceFromCenter));	\n"
		"}																																						\n";

class SelectiveBlurSceneEffect: public BlurSceneEffect {
private:
    GLuint selectiveProgId;
    GLuint selectiveVertexCoords;
    GLuint selectiveSharpTextureCoords;
    GLuint selectiveBlurTextureCoords;
    GLint selectiveUniformSharpTexture;
    GLint selectiveUniformBlurTexture;
    GLint selectiveUniformExcludeCircleRadius;
    GLint selectiveUniformExcludeCirclePoint;
    GLint selectiveUniformExcludeBlurSize;
    GLint selectiveUniformAspectRatio;

	/** 做过blur的纹理ID **/
	GPUTexture* blurTexCache;
	OpenglVideoFrame* blurVideoFrame;
public:
	SelectiveBlurSceneEffect();
    virtual ~SelectiveBlurSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void destroy();
};

#endif // SELECTIVE_BLUR_SCENE_EFFECT_H
