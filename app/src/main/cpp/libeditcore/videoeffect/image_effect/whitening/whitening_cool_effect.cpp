#include "whitening_cool_effect.h"

#define LOG_TAG "WhiteningCoolEffect"

WhiteningCoolEffect::WhiteningCoolEffect(){
    this->addFilter(new DirectPassEffect());
    this->addFilter(new HighPassEffect());
    this->addFilter(new WhiteningHorizontalEffect());
    this->addFilter(new WhiteningVerticalEffect());
    this->addFilter(new ToneCurveEffect());
}

WhiteningCoolEffect::~WhiteningCoolEffect() {
}
