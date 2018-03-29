#include "whitening_effect.h"

#define LOG_TAG "WhiteningEffect"

WhiteningEffect::WhiteningEffect(){
    this->addFilter(new DirectPassEffect());
    this->addFilter(new HighPassEffect());
	this->addFilter(new WhiteningHorizontalEffect());
    this->addFilter(new WhiteningVerticalEffect());
    this->addFilter(new HueEffect());
}

WhiteningEffect::~WhiteningEffect() {
}
