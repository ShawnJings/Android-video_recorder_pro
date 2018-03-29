#ifndef PROCESSOR_BEAUTIFY_FACE_COOL_EFFECT_H
#define PROCESSOR_BEAUTIFY_FACE_COOL_EFFECT_H
#include "../image_effect_group.h"
#include "../../direct_pass_effect.h"
#include "./smooth_skin_effect.h"
#include "../sharpen_effect.h"
#include "../hue_effect.h"
#include "../tone_curve_effect.h"

class BeautifyFaceCoolEffect: public ImageEffectGroup {
public:
    BeautifyFaceCoolEffect();
    virtual ~BeautifyFaceCoolEffect();
};

#endif // PROCESSOR_BEAUTIFY_FACE_COOL_EFFECT_H
