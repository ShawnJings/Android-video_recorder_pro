#include "beautify_face_effect.h"

#define LOG_TAG "BeautifyFaceEffect"

BeautifyFaceEffect::BeautifyFaceEffect(){
    this->addFilter(new SmoothSkinEffect());
    this->addFilter(new HueEffect());
    this->addFilter(new SharpenEffect());
}

BeautifyFaceEffect::~BeautifyFaceEffect() {
}
