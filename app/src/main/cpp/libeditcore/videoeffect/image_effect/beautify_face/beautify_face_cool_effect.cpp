#include "beautify_face_cool_effect.h"

#define LOG_TAG "BeautifyFaceCoolEffect"

BeautifyFaceCoolEffect::BeautifyFaceCoolEffect(){
    this->addFilter(new DirectPassEffect());
    this->addFilter(new SmoothSkinEffect());
    this->addFilter(new HueEffect());
    this->addFilter(new SharpenEffect());
    this->addFilter(new ToneCurveEffect());
}

BeautifyFaceCoolEffect::~BeautifyFaceCoolEffect() {
}
