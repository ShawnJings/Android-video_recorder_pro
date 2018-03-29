#include "thin_whitening_effect.h"

#define LOG_TAG "ThinWhiteningEffect"

ThinWhiteningEffect::ThinWhiteningEffect(){
    this->addFilter(new ThinEffect());
    this->addFilter(new HighPassEffect());
	this->addFilter(new WhiteningHorizontalEffect());
    this->addFilter(new WhiteningVerticalEffect());
    this->addFilter(new HueEffect());
}

ThinWhiteningEffect::~ThinWhiteningEffect() {
}
