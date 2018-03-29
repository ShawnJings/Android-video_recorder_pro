#ifndef BLUR_SCENE_EFFECT_H
#define BLUR_SCENE_EFFECT_H
#include "../../gpu_texture_cache.h"
#include "../base_video_effect.h"
#include "./shader/optimized_blur_shader.glsl"
#include "./shader/smooth_blur_shader.glsl"

class BlurSceneEffect: public BaseVideoEffect {
private:
	/** Horizontal Program extra uniform **/
	GLint uniformLoc_hor_texelWidthOffset;
	GLint uniformLoc_hor_texelHeightOffset;

	/** 中间的临时纹理ID **/
	GPUTexture* blurTempTexCache;

	/** Vertical Program attr&uniform **/
    GLuint verticalProgId;
    GLuint attr_ver_VertexCoords;
    GLuint attr_ver_TextureCoords;
    GLint uniformLoc_ver_Texture;
	GLint uniformLoc_ver_texelWidthOffset;
	GLint uniformLoc_ver_texelHeightOffset;

	char* getHorizontalBlurVertexShader(){
		#ifdef __ANDROID__
			return OPTIMIZED_BLUR_SCENE_HORIZONTAL_VERTEX_SHADER;
		#elif defined(__APPLE__)	// IOS or OSX
			return SMOOTH_BLUR_SCENE_HORIZONTAL_VERTEX_SHADER;
		#endif
	};
	char* getHorizontalBlurFragmentShader(){
		#ifdef __ANDROID__
			return OPTIMIZED_BLUR_SCENE_HORIZONTAL_FRAG_SHADER;
		#elif defined(__APPLE__)	// IOS or OSX
			return SMOOTH_BLUR_SCENE_HORIZONTAL_FRAG_SHADER;
		#endif
	};
	char* getVerticalBlurVertexShader(){
		#ifdef __ANDROID__
			return OPTIMIZED_BLUR_SCENE_VERTICAL_VERTEX_SHADER;
		#elif defined(__APPLE__)	// IOS or OSX
			return SMOOTH_BLUR_SCENE_VERTICAL_VERTEX_SHADER;
		#endif
	};
	char* getVerticalBlurFragmentShader(){
		#ifdef __ANDROID__
			return OPTIMIZED_BLUR_SCENE_VERTICAL_FRAG_SHADER;
		#elif defined(__APPLE__)	// IOS or OSX
			return SMOOTH_BLUR_SCENE_VERTICAL_FRAG_SHADER;
		#endif
	};
public:
	BlurSceneEffect();
    virtual ~BlurSceneEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void buildParamDefaultValue();
    virtual void destroy();
};

#endif // BLUR_SCENE_EFFECT_H
