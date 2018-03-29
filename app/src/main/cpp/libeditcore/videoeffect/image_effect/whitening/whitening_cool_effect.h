#ifndef PROCESSOR_WHITENING_COOL_EFFECT_H
#define PROCESSOR_WHITENING_COOL_EFFECT_H
#include "../image_effect_group.h"
#include "whitening_hor_effect.h"
#include "../../direct_pass_effect.h"
#include "../../saturation_scene/saturation_effect.h"
#include "../highpass/high_pass_effect.h"
#include "../tone_curve_effect.h"
#include "whitening_ver_effect.h"

class WhiteningCoolEffect: public ImageEffectGroup {
public:
    WhiteningCoolEffect();
    virtual ~WhiteningCoolEffect();
};

#endif // PROCESSOR_WHITENING_COOL_EFFECT_H
