#include "player_contrast_effect.h"

#define LOG_TAG "PlayerContrastEffect"

PlayerContrastEffect::PlayerContrastEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = PLAYER_CONTRAST_EFFECT_FRAGMENT_SHADER;
}

PlayerContrastEffect::~PlayerContrastEffect() {
}
