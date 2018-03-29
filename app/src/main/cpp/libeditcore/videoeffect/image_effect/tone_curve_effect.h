#ifndef PROCESSOR_TONE_CURVE_EFFECT_H
#define PROCESSOR_TONE_CURVE_EFFECT_H

#include <list>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include "./image_base_effect.h"

/**
 * 色调曲线 可以实现:小清新、优雅、幻彩、日系、枫叶等 效果
 * **/
static char* TONE_CURVE_EFFECT_FRAGMENT_SHADER =
	"precision lowp float;\n"
	"precision lowp sampler2D;\n"
	"varying lowp vec2 v_texcoord;\n"
	"uniform sampler2D yuvTexSampler;\n"
	"uniform sampler2D toneCurveTexture;\n"
	"void main() {\n"
		"lowp vec4 textureColor = texture2D(yuvTexSampler, v_texcoord);\n"
		"float redCurveValue = texture2D(toneCurveTexture, vec2(textureColor.r, 0.0)).r;\n"
		"float greenCurveValue = texture2D(toneCurveTexture, vec2(textureColor.g, 0.0)).g;\n"
		"float blueCurveValue = texture2D(toneCurveTexture, vec2(textureColor.b, 0.0)).b;\n"
		"\n"
		"gl_FragColor = vec4(redCurveValue, greenCurveValue, blueCurveValue, textureColor.a);\n"
	"}\n";

typedef struct PointF
{
	float x;
	float y;
	bool operator < (PointF *point) const
	{
		return x < point->x;
	}
	bool operator > (PointF *point) const
	{
		return x > point->x;
	}
	PointF(float x, float y):x(x),y(y){
		//DO NOTHING HERE
	};
} PointF;
class ToneCurveEffect: public ImageBaseEffect {
public:
	ToneCurveEffect();
    virtual ~ToneCurveEffect();
    virtual bool init();

protected:
    virtual void onDrawArraysPre(EffectCallback * filterCallback);

private:
    GLuint mToneCurveTexture;
    GLint mToneCurveTextureUniformLocation;
    byte* mACVBuffer;
    int mACVBufferSize;

    void fillControlPointsFromACVBuffer();
    short readShortFromACVBuffer();

    std::vector<PointF*>* mRgbCompositeControlPoints;
    std::vector<PointF*>* mRedControlPoints;
    std::vector<PointF*>* mGreenControlPoints;
    std::vector<PointF*>* mBlueControlPoints;

    std::vector<float>* mRgbCompositeCurve;
    std::vector<float>* mRedCurve;
    std::vector<float>* mGreenCurve;
    std::vector<float>* mBlueCurve;

    std::vector<float>* createSplineCurve(std::vector<PointF*>* points);
    std::vector<double>* createSecondDerivative(std::vector<PointF*>* points);
    std::vector<PointF*>* createSplineCurve2(std::vector<PointF*>* convertedPoints);

    void updateToneCurveTexture();
};

#endif // PROCESSOR_TONE_CURVE_EFFECT_H
