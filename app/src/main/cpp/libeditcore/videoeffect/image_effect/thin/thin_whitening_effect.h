#ifndef PROCESSOR_THIN_WHITENING_EFFECT_H
#define PROCESSOR_THIN_WHITENING_EFFECT_H
#include "../image_effect_group.h"
#include "../whitening/whitening_hor_effect.h"
#include "../whitening/whitening_ver_effect.h"
#include "../../direct_pass_effect.h"
#include "../highpass/high_pass_effect.h"
#include "../hue_effect.h"
#include "thin_effect.h"

class ThinWhiteningEffect: public ImageEffectGroup {
public:
	ThinWhiteningEffect();
	virtual ~ThinWhiteningEffect();
};

#endif // PROCESSOR_THIN_WHITENING_EFFECT_H
