#include "image_base_effect.h"

#define LOG_TAG "ImageBaseEffect"

ImageBaseEffect::ImageBaseEffect() {
}

ImageBaseEffect::~ImageBaseEffect() {
}

void ImageBaseEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords){
	BaseVideoEffect::renderEffect(inputFrame, outputFrame, filterCallback, vertexCoords, textureCoords);
}

void ImageBaseEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback){
	float zoomRatio = 1.0f;
	if (filterCallback) {
		ParamVal val;
		bool suc = filterCallback->getParamValue(string(IMAGE_EFFECT_ZOOM_VIEW_RATIO), val);
		if (suc) {
			zoomRatio = val.u.fltVal;
		}
	}
	GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	if(zoomRatio < 1.0f){
		float fromPosition = 0.0f;
		float toPosition = 1.0f;
		float yDistance = (toPosition - fromPosition) * (1 - zoomRatio) / 2;
		fromPosition+=yDistance;
		toPosition-=yDistance;
		for(int i = 0; i < 8; i++){
			if(0.0f == texCoords[i]){
				texCoords[i] = fromPosition;
			} else{
				texCoords[i] = toPosition;
			}
		}
	}
	this->renderEffect(inputFrame, outputFrame, filterCallback, _vertices, texCoords);
}
