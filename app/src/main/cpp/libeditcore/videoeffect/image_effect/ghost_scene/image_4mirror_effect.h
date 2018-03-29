//
//  image_4mirror_effect.hpp
//  video_player
//
//  Created by apple on 16/9/29.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_4mirror_effect_hpp
#define image_4mirror_effect_hpp

#include "../image_base_effect.h"
static const GLfloat leftTopImageVertices[] = {
    -1.0f, 0.0f,
    0.0f, 0.0f,
    -1.0f,  1.0f,
    0.0f,  1.0f,
};

static const GLfloat rightTopImageVertices[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f,  1.0f,
    1.0f,  1.0f,
};

static const GLfloat leftBottomImageVertices[] = {
    -1.0f, -1.0f,
    0.0f, -1.0f,
    -1.0f,  0.0f,
    0.0f,  0.0f,
};

static const GLfloat rightBottomImageVertices[] = {
    0.0f, -1.0f,
    1.0f, -1.0f,
    0.0f,  0.0f,
    1.0f,  0.0f,
};

static const GLfloat leftTopTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

static const GLfloat rightTopTextureCoordinates[] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};

static const GLfloat leftBottomTextureCoordinates[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
};

static const GLfloat rightBottomTextureCoordinates[] = {
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};

class Image4MirrorEffect: public ImageBaseEffect {
public:
    Image4MirrorEffect();
    virtual ~Image4MirrorEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
private:
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords);
};

#endif /* image_4mirror_effect_hpp */
