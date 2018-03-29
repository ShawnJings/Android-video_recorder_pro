#include "grayscale_effect.h"

#define LOG_TAG "GrayScaleEffect"

GrayScaleEffect::GrayScaleEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = GRAYSCALE_EFFECT_FRAGMENT_SHADER;
}

GrayScaleEffect::~GrayScaleEffect() {
}
