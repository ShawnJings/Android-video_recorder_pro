#ifndef VIDEO_EFFECT_DEF_H
#define VIDEO_EFFECT_DEF_H

#include <string>
using namespace std;

enum TransitionType
{
	TRANSITION_TYPE_FADE_IN,
	TRANSITION_TYPE_SPLASH_WHITE,
	TRANSITION_TYPE_SPLASH_BLACK
};
/* effect parameter  mechanism */
enum EffectParamType
{
	EffectParamTypeInvalid = -1,
	EffectParamTypeArbitrary = 0,
	EffectParamTypeInt,
	EffectParamTypeFloat,
	EffectParamTypeBoolean,
	EffectParamTypeString,
	EffectParamTypeColor,
	EffectParamTypePosition2D,
	EffectParamTypePosition3D,
	EffectParamTypePath,	// path， must be string type, just for merge with folder path
	EffectParamTypeCount
};

struct Color {
    float r, g, b, a;
};

struct Position2D {
    float x, y;
};

struct Position3D {
    float x, y, z;
};

struct TextCanvasParam {
	/** 画布宽度 默认480 **/
	int width;
	/** 画布高度 默认480 **/
	int height;
	/** 文本绘制距离画布左边的距离 **/
	int textLabelLeft;
	/** 文本绘制距离画布上边的距离 **/
	int textLabelTop;
	/** 文本绘制的宽度 **/
	int textLabelWidth;
	/** 文本绘制的高度 **/
	int textLabelHeight;
	/** 文字的颜色 int rgb = (r << 16) | (g << 8) | b **/
	int textColor;
	/** 文字的阴影半径 x偏移 y偏移 **/
	float textShadowRadius;
	float textShadowXOffset;
	float textShadowYOffset;
	/** 文字的阴影颜色 **/
	int textShadowColor;
	/** 文本大小 **/
	int textSize;
	/** 文本对齐方式 -1左对齐 0居中对齐 1右对齐 **/
	int textAlignment;
	/** 文字类型 0代表songName 1代表nickName **/
	int textType;
	TextCanvasParam(int width, int height, int textLabelLeft, int textLabelTop,
			int textLabelWidth, int textLabelHeight, int textColor, float textShadowRadius,
			float textShadowXOffset, float textShadowYOffset, int textShadowColor,
			int textSize, int textAlignment, int textType){
		this->width = width;
		this->height = height;
		this->textLabelLeft = textLabelLeft;
		this->textLabelTop = textLabelTop;
		this->textLabelWidth = textLabelWidth;
		this->textLabelHeight = textLabelHeight;
		this->textColor = textColor;
		this->textShadowRadius = textShadowRadius;
		this->textShadowXOffset = textShadowXOffset;
		this->textShadowYOffset = textShadowYOffset;
		this->textShadowColor = textShadowColor;
		this->textSize = textSize;
		this->textAlignment = textAlignment;
		this->textType = textType;
	}
};

struct ParamVal {
    union {
        void *arbData;          // Used when parameter type is arbitrary
        int intVal;             // Used when parameter type is integer
        double fltVal;          // Used when parameter type is float
        bool boolVal;           // Used when parameter type is boolean
        Color colorVal;      	// Used when parameter type is color
        Position2D pos2dVal; 	// Used when parameter type is position 2D
        Position3D pos3dVal; 	// Used when parameter type is position 3D
    } u;

    string strVal;              // used when parameter type is string

    EffectParamType type;
};

class EffectCallback
{
public:
    virtual bool getParamValue(string paramName, ParamVal & val) = 0;
    virtual void setParamValue(string paramName, ParamVal val) = 0;
};

#endif
