#ifndef PROCESSOR_WHITENING_HORIZONTAL_FILTER_H
#define PROCESSOR_WHITENING_HORIZONTAL_FILTER_H

#include "./shader/whitening_hor_fragment.frag"
#include "./shader/whitening_hor_vertex.vert"
#include "../../base_video_effect.h"

class WhiteningHorizontalEffect: public BaseVideoEffect {
private:
	int width;
	float amplitude;
	float edger;
    GLuint uniformLoc_amplitude;
    GLuint uniformLoc_edger;
    GLuint uniformLoc_offset;

public:
    WhiteningHorizontalEffect();
	virtual ~WhiteningHorizontalEffect();

    bool init();

protected:
    void onDrawArraysPre(EffectCallback * filterCallback);
};

#endif // PROCESSOR_WHITENING_HORIZONTAL_FILTER_H
