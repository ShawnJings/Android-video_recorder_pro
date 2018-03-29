#ifndef PROCESSOR_IMAGE_BASE_EFFECT_H
#define PROCESSOR_IMAGE_BASE_EFFECT_H

#include "../base_video_effect.h"

class ImageBaseEffect: public BaseVideoEffect {
public:
	ImageBaseEffect();
    virtual ~ImageBaseEffect();

    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords);

    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);

};

#endif // PROCESSOR_IMAGE_BASE_EFFECT_H
