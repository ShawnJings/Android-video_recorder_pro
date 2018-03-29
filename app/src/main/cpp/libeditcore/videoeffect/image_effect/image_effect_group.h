#ifndef PROCESSOR_IMAGE_EFFECT_GROUP_H
#define PROCESSOR_IMAGE_EFFECT_GROUP_H

#include <list>
#include "./image_base_effect.h"

/**
 *
 * Filter Group
 * Resembles a filter that consists of multiple filters applied after each other.
 *
 * **/
class ImageEffectGroup: public ImageBaseEffect {
protected:
	std::list<BaseVideoEffect*> *mFilters;

	bool isInitFrameBufferFlag;
    GLuint* mFrameBufferTextures;
    void initFramebuffers(EffectCallback * filterCallback);
    void destroyFramebuffers();

    GLfloat* mGLVertexBuffer;
    GLfloat* mGLTextureBuffer;
    GLfloat* mGLTextureFlipBuffer;

	OpenglVideoFrame * 	inputVideoFrame;
	OpenglVideoFrame * 	outputVideoFrame;
public:
    ImageEffectGroup();
	virtual ~ImageEffectGroup();

	void addFilter(BaseVideoEffect* filter);

    virtual bool init();

    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords);

    virtual void destroy();
};

#endif // PROCESSOR_IMAGE_EFFECT_GROUP_H
