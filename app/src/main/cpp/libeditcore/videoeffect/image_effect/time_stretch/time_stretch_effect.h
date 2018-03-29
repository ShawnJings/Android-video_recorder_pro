//
//  image_4mirror_effect.hpp
//  video_player
//
//  Created by apple on 16/9/29.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef time_stretch_effect_hpp
#define time_stretch_effect_hpp

#include "../image_base_effect.h"

class TimeStretchEffect: public ImageBaseEffect {
public:
    TimeStretchEffect();
    virtual ~TimeStretchEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
private:
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords);
};

#endif /* time_stretch_effect_hpp */
