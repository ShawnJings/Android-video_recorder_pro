#ifndef PROCESSOR_WHITENING_VERTICAL_EFFECT_H
#define PROCESSOR_WHITENING_VERTICAL_EFFECT_H

#include "./shader/whitening_ver_fragment.frag"
#include "./shader/whitening_ver_vertex.vert"
#include "../../base_video_effect.h"

class WhiteningVerticalEffect: public BaseVideoEffect {
private:
    GLuint uniformLoc_amplitude;
    GLuint uniformLoc_edger;
    GLuint uniformLoc_offset;
    GLuint uniformLoc_softLightBlendR;
    GLuint uniformLoc_softLightBlendG;
    GLuint uniformLoc_softLightBlendB;
    GLuint uniformLoc_softLightBlendA;

public:
    WhiteningVerticalEffect();
	virtual ~WhiteningVerticalEffect();

    bool init();

protected:
    void onDrawArraysPre(EffectCallback * filterCallback);
};

#endif // PROCESSOR_WHITENING_VERTICAL_EFFECT_H
