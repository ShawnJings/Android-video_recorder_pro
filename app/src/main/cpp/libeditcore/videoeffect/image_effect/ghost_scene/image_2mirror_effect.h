//
//  image_4mirror_effect.hpp
//  video_player
//
//  Created by apple on 16/9/29.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_2mirror_effect_hpp
#define image_2mirror_effect_hpp

#include "../image_base_effect.h"

static const GLfloat leftImageVertices[] = {
    -1.0f, -1.0f,
    0.0f, -1.0f,
    -1.0f,  1.0f,
    0.0f,  1.0f,
};

static const GLfloat rightImageVertices[] = {
    0.0f, -1.0f,
    1.0f, -1.0f,
    0.0f,  1.0f,
    1.0f,  1.0f,
};

static const GLfloat leftTextureCoordinates[] = {
    0.25f, 0.0f,
    0.75f, 0.0f,
    0.25f, 1.0f,
    0.75f, 1.0f,
};
static const GLfloat rightTextureCoordinates[] = {
    0.75f, 0.0f,
    0.25f, 0.0f,
    0.75f, 1.0f,
    0.25f, 1.0f,
};

class Image2MirrorEffect: public ImageBaseEffect {
public:
    Image2MirrorEffect();
    virtual ~Image2MirrorEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
private:
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords);
};

#endif /* image_2mirror_effect_hpp */
