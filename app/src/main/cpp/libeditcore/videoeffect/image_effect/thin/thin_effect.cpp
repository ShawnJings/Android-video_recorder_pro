#include "thin_effect.h"

#define LOG_TAG "ThinEffect"

ThinEffect::ThinEffect() {
	mVertexShader = VERTEX_SHADER_FILTER;
	mFragmentShader = FRAG_SHADER_FILTER;
	length = 8;
	tempTextureCoords = new GLfloat[length];
}

ThinEffect::~ThinEffect() {
	delete[] tempTextureCoords;
}

void ThinEffect::renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, GLfloat* vertexCoords, GLfloat* textureCoords){
    memcpy(tempTextureCoords, textureCoords, length * sizeof(GLfloat));
    //	tempTextureCoords[0] = 0.0f;
    //	tempTextureCoords[2] = 1.0f;
    bool vFlipFlag = false;
    if (filterCallback) {
        ParamVal val;
        bool suc = filterCallback->getParamValue(string(THIN_EFFECT_VFLIP_FLAG), val);
        if (suc) {
            vFlipFlag = val.u.boolVal;
        }
    }
    if(!vFlipFlag){
        tempTextureCoords[0] = 0.0f;
        tempTextureCoords[2] = 1.0f;
    } else{
        tempTextureCoords[4] = 0.0f;
        tempTextureCoords[6] = 1.0f;
    }
    BaseVideoEffect::renderEffect(inputFrame, outputFrame, filterCallback, vertexCoords, tempTextureCoords);
}
