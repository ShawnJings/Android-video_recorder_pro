#ifndef PROCESSOR_BEAUTIFY_FACE_EFFECT_H
#define PROCESSOR_BEAUTIFY_FACE_EFFECT_H
#include "../image_effect_group.h"
#include "../../direct_pass_effect.h"
#include "./smooth_skin_effect.h"
#include "../sharpen_effect.h"
#include "../hue_effect.h"

class BeautifyFaceEffect: public ImageEffectGroup {
public:
	BeautifyFaceEffect();
	virtual ~BeautifyFaceEffect();
};

#endif // PROCESSOR_BEAUTIFY_FACE_EFFECT_H
