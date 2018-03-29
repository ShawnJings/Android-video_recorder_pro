#ifndef PROCESSOR_WHITENING_EFFECT_H
#define PROCESSOR_WHITENING_EFFECT_H
#include "../image_effect_group.h"
#include "whitening_hor_effect.h"
#include "../../direct_pass_effect.h"
#include "../../saturation_scene/saturation_effect.h"
#include "../highpass/high_pass_effect.h"
#include "../hue_effect.h"
#include "whitening_ver_effect.h"

class WhiteningEffect: public ImageEffectGroup {
public:
	WhiteningEffect();
	virtual ~WhiteningEffect();
};

#endif // PROCESSOR_WHITENING_EFFECT_H
