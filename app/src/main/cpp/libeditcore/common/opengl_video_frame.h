#ifndef OPENGL_VIDEO_FRAME_H
#define OPENGL_VIDEO_FRAME_H

#include "../platform_dependent/platform_common.h"
#include "../platform_dependent/platform_gl.h"

void GL_CHECK_ERROR(const char* op);

struct ImagePosition {
	ImagePosition () {

	}

	ImagePosition(GLint x, GLint y, GLsizei width, GLsizei height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	GLint	x;
	GLint 	y;
	GLsizei	width;
	GLsizei	height;
};

class OpenglVideoFrame {
public:
	OpenglVideoFrame();
	OpenglVideoFrame(GLuint textureId, const ImagePosition & imgPos);
    OpenglVideoFrame(GLuint textureId, const ImagePosition & imgPos, float position);
    void init(GLuint textureId, const ImagePosition & imgPos);
    void init(GLuint textureId, const ImagePosition & imgPos, float position);
	void setTextureId(GLuint textureId);
	virtual ~OpenglVideoFrame();

public:
	void setStreamPosition(float videoPosition);
	float getStreamPosition();

	GLuint getTextureId();
	virtual void GetImagePosition(ImagePosition * imgPos);


protected:
	GLuint 			mTexId;
	float 			mPosition;
	ImagePosition 	mImagePos;
};

#endif // OPENGL_VIDEO_FRAME_H
