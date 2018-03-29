#ifndef PROCESSOR_THIN_EFFECT_H
#define PROCESSOR_THIN_EFFECT_H

#include "../image_base_effect.h"

class ThinEffect: public ImageBaseEffect {
private:
	float aspect_ratio;
	float fromXPosition;
	float toXPosition;
	float stretchXPosition;
	int length;
	bool flag;
	GLfloat* tempTextureCoords;
public:
	ThinEffect();
    virtual ~ThinEffect();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords);

};

#endif // PROCESSOR_THIN_EFFECT_H
