#include "thin_beautify_face_effect.h"

#define LOG_TAG "ThinBeautifyFaceEffect"

ThinBeautifyFaceEffect::ThinBeautifyFaceEffect(){
    this->addFilter(new ThinEffect());
    this->addFilter(new SmoothSkinEffect());
    this->addFilter(new HueEffect());
    this->addFilter(new SharpenEffect());
    this->addFilter(new DirectPassEffect());
}

ThinBeautifyFaceEffect::~ThinBeautifyFaceEffect() {
}
