#include "./blur_scene_effect.h"

#define LOG_TAG "BlurSceneEffect"

BlurSceneEffect::BlurSceneEffect() {
	mVertexShader = getHorizontalBlurVertexShader();
	mFragmentShader = getHorizontalBlurFragmentShader();
	blurTempTexCache = NULL;
}

BlurSceneEffect::~BlurSceneEffect() {
}

bool BlurSceneEffect::init(){
	/** default build Horizontal Program **/
	BaseVideoEffect::init();
	glUseProgram(mGLProgId);
	uniformLoc_hor_texelWidthOffset = glGetUniformLocation(mGLProgId, "texelWidthOffset");
	checkGlError("BlurSceneEffect glGetUniformLocation texelWidthOffset");
	uniformLoc_hor_texelHeightOffset = glGetUniformLocation(mGLProgId, "texelHeightOffset");
	checkGlError("BlurSceneEffect glGetUniformLocation texelHeightOffset");
	blurTempTexCache = NULL;
	/** build Vertical Program **/
	verticalProgId = loadProgram(getVerticalBlurVertexShader(), getVerticalBlurFragmentShader());
	if (!verticalProgId) {
		LOGE("Could not create program Trailer Scene verticalProgId.");
		return false;
	}
	glUseProgram(verticalProgId);
	attr_ver_VertexCoords = glGetAttribLocation(verticalProgId, "position");
	checkGlError("glGetAttribLocation attr_ver_VertexCoords");
	attr_ver_TextureCoords = glGetAttribLocation(verticalProgId, "texcoord");
	checkGlError("glGetAttribLocation attr_ver_TextureCoords");
	uniformLoc_ver_Texture = glGetUniformLocation(verticalProgId, "yuvTexSampler");
	checkGlError("glGetAttribLocation uniformLoc_ver_Texture");
	uniformLoc_ver_texelWidthOffset = glGetUniformLocation(verticalProgId, "texelWidthOffset");
	checkGlError("glGetAttribLocation uniformLoc_ver_texelHeightOffset");
	uniformLoc_ver_texelHeightOffset = glGetUniformLocation(verticalProgId, "texelHeightOffset");
	checkGlError("glGetAttribLocation uniformLoc_ver_texelHeightOffset");

	return true;
}

/**  Template Method  **/
void BlurSceneEffect::buildParamDefaultValue() {
	mMapParamDef.clear();

	ParamVal paramValVertexCoords;
	paramValVertexCoords.type = EffectParamTypeArbitrary;
	float _vertexs[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
	float * vertexCoords = new float[8];
	memcpy(vertexCoords, _vertexs, sizeof(float) * 8);
	paramValVertexCoords.u.arbData = (void *) vertexCoords;
	mMapParamDef[string(CONTRAST_PARAM_ID_VERTEX_COORDS)] = paramValVertexCoords;

	ParamVal paramValTextureCoords;
	paramValTextureCoords.type = EffectParamTypeArbitrary;
	float _textures[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	float * texCoords = new float[8];
	memcpy(texCoords, _textures, sizeof(float) * 8);
	paramValTextureCoords.u.arbData = (void *) texCoords;
	mMapParamDef[string(CONTRAST_PARAM_ID_TEXTURE_COORDS)] = paramValTextureCoords;
}

void BlurSceneEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback) {
	if (inputFrame == NULL || outputFrame == NULL) {
		LOGE("BlurSceneEffect::renderEffect input or output frame pointer is NULL!");
		return;
	}
	if (!mIsInitialized) {
		LOGE("BlurSceneEffect::renderEffect effect not initialized!");
		return;
	}

	ImagePosition imgPos;
	outputFrame->GetImagePosition(&imgPos);

	//LOGI("BlurSceneEffect getParamValue ");
	if(NULL == blurTempTexCache){
		blurTempTexCache = GPUTextureCache::GetInstance()->fetchTexture(imgPos.width, imgPos.height);
		blurTempTexCache->lock();
	}

	float progress = 0.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(BLUR_SCENE_PARAM_ID_PROGRESS), val);
		if (suc) {
			progress = val.u.fltVal;
//			LOGI("get success, progress:%.4f", progress);
		} else {
//			LOGI("get progress failed, use default value");
		}
	}
//	LOGI("after BlurSceneEffect getParamValue ");

	/** 首先以原始纹理作为输入, 做一遍横向的blur, 输出到blurTempTexCache中 **/
	this->switchFBOAttachmentTexture(blurTempTexCache->getTexId());

	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	glUseProgram(mGLProgId);
	static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
	glEnableVertexAttribArray(mGLVertexCoords);
	static const GLfloat texCoords[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
	glEnableVertexAttribArray(mGLTextureCoords);

	/** 这个是原始的纹理 **/
	glActiveTexture(GL_TEXTURE0);
	this->bindTexture(inputFrame->getTextureId());
	glUniform1i(mGLUniformTexture, 0);
	/** 设置FadeIn的Progress **/
	glUniform1f(uniformLoc_hor_texelWidthOffset, 0.0);
	glUniform1f(uniformLoc_hor_texelHeightOffset, (GLfloat)(1.0 * progress / imgPos.width));

	this->draw();
	glDisableVertexAttribArray(mGLVertexCoords);
	glDisableVertexAttribArray(mGLTextureCoords);

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("11111TrailerSceneEffect::renderEffect glBindFramebuffer 0 ");

	/** 然后以blurTempTexCache这个纹理作为输入纹理，再加上FadeInTexId作为输入纹理，以及Progress输出到我们的目标FBO上面 **/
	this->switchFBOAttachmentTexture(outputFrame->getTextureId());
	glViewport(imgPos.x, imgPos.y, imgPos.width, imgPos.height);
	glUseProgram(verticalProgId);
	static const GLfloat _vertices1[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(attr_ver_VertexCoords, 2, GL_FLOAT, 0, 0, _vertices1);
	glEnableVertexAttribArray(attr_ver_VertexCoords);
	static const GLfloat texCoords1[] =  { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(attr_ver_TextureCoords, 2, GL_FLOAT, 0, 0, texCoords1);
	glEnableVertexAttribArray(attr_ver_TextureCoords);

	/** 这个是临时过渡的纹理 **/
	glActiveTexture(GL_TEXTURE0);
	this->bindTexture(blurTempTexCache->getTexId());
	glUniform1i(uniformLoc_ver_Texture, 0);
	/** 设置FadeIn的Progress **/
	glUniform1f(uniformLoc_ver_texelWidthOffset, (GLfloat)(1.0 * progress / imgPos.height));
	glUniform1f(uniformLoc_ver_texelHeightOffset, 0.0);

	this->draw();
	glDisableVertexAttribArray(attr_ver_VertexCoords);
	glDisableVertexAttribArray(attr_ver_TextureCoords);

	glBindTexture(GL_TEXTURE_2D, 0);
	this->switchFBODefaultAttachment();
	GL_CHECK_ERROR("2: BlurSceneEffect::renderEffect vertical blur bindframebuffer 0");
}

void BlurSceneEffect::destroy() {
	BaseVideoEffect::destroy();

	glDeleteProgram(verticalProgId);

	if(NULL != blurTempTexCache){
		blurTempTexCache->unLock();
	}
	// must delete arbitrary data
	map<string, ParamVal>::iterator itor;
	itor = mMapParamDef.find(string(CONTRAST_PARAM_ID_VERTEX_COORDS));
	if (itor != mMapParamDef.end()) {
		float * pointer = (float *) ((itor->second).u.arbData);
		delete[] pointer;
	}

	itor = mMapParamDef.find(string(CONTRAST_PARAM_ID_TEXTURE_COORDS));
	if (itor != mMapParamDef.end()) {
		float * pointer = (float *) ((itor->second).u.arbData);
		delete[] pointer;
	}

	mMapParamDef.clear();
}
