#ifndef BASE_VIDEO_EFFECT_H
#define BASE_VIDEO_EFFECT_H
#include "../platform_dependent/platform_common.h"
#include "../platform_dependent/platform_gl.h"
#include "../common/video_effect_params.h"
#include "../common/opengl_video_frame.h"
#include "../common/video_effect_def.h"
#include "../common/matrix.h"
#include <map>

using namespace std;
static char* VERTEX_SHADER_FILTER =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 v_texcoord;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   v_texcoord = texcoord;  \n"
		"}                            \n";

static char* FRAG_SHADER_FILTER =
		"precision highp float;\n"
	    "varying highp vec2 v_texcoord;\n"
	    "uniform sampler2D yuvTexSampler;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
		"}\n";

class BaseVideoEffect {
public:
	BaseVideoEffect();
    virtual ~BaseVideoEffect();

    virtual bool init();
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords);
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    virtual void draw();
    virtual void bindTexture(int texId);
    virtual void destroy();
    virtual void buildParamDefaultValue();

protected:
    char* mVertexShader;
    char* mFragmentShader;

    bool mIsInitialized;

    GLuint mGLProgId;
    GLuint mGLVertexCoords;
    GLuint mGLTextureCoords;
    GLint mGLUniformTexture;

    virtual void onDrawArraysPre(EffectCallback * filterCallback){};

    void checkGlError(const char* op);

    void switchFBOAttachmentTexture(GLuint textureId);
    void switchFBODefaultAttachment();
protected:
    GLuint loadProgram(char* pVertexSource, char* pFragmentSource);
    GLuint loadShader(GLenum shaderType, const char* pSource);
    ParamVal getParamValHelper(string paramName, EffectCallback * filterCallback);

protected:
    map<string, ParamVal>	mMapParamDef;	// effect params default value
};

#endif // BASE_VIDEO_EFFECT_H
