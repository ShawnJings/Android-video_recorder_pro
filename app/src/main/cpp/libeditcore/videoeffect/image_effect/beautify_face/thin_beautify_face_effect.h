#ifndef PROCESSOR_THIN_BEAUTIFY_FACE_EFFECT_H
#define PROCESSOR_THIN_BEAUTIFY_FACE_EFFECT_H
#include "../image_effect_group.h"
#include "../../direct_pass_effect.h"
#include "./smooth_skin_effect.h"
#include "../sharpen_effect.h"
#include "../hue_effect.h"
#include "../thin/thin_effect.h"

class ThinBeautifyFaceEffect: public ImageEffectGroup {
public:
	ThinBeautifyFaceEffect();
	virtual ~ThinBeautifyFaceEffect();
};

#endif // PROCESSOR_THIN_BEAUTIFY_FACE_EFFECT_H
