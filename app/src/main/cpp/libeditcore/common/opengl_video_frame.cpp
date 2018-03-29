#include "./opengl_video_frame.h"

#define LOG_TAG "OpenglVideoFrame"

void GL_CHECK_ERROR(const char* op)
{
	GLenum glErr = glGetError();
	if (glErr != GL_NO_ERROR) {
		LOGE("after %s() GL error: 0x%x", op, glErr);
	}
}

OpenglVideoFrame::OpenglVideoFrame(){}
OpenglVideoFrame::OpenglVideoFrame(GLuint textureId, const ImagePosition & imgPos) {
	mTexId = textureId;
	mImagePos = imgPos;
}
OpenglVideoFrame::OpenglVideoFrame(GLuint textureId, const ImagePosition & imgPos, float position) {
    mTexId = textureId;
    mImagePos = imgPos;
    mPosition = position;
}

void OpenglVideoFrame::init(GLuint textureId, const ImagePosition & imgPos) {
	mTexId = textureId;
	mImagePos = imgPos;
}

void OpenglVideoFrame::init(GLuint textureId, const ImagePosition & imgPos, float position) {
    mTexId = textureId;
    mImagePos = imgPos;
    mPosition = position;
}

void OpenglVideoFrame::setTextureId(GLuint textureId){
	mTexId = textureId;
}

OpenglVideoFrame::~OpenglVideoFrame() {

}

void OpenglVideoFrame::setStreamPosition(float videoPosition) {
	mPosition = videoPosition;
}

float OpenglVideoFrame::getStreamPosition() {
	return mPosition;
}

GLuint OpenglVideoFrame::getTextureId() {
	return mTexId;
}

void OpenglVideoFrame::GetImagePosition(ImagePosition * imgPos) {
	*imgPos = mImagePos;
}
