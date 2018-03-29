//
//  carousel_effect.hpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_carousel_effect_h
#define image_carousel_effect_h

#include <queue>
#include "../../image_base_effect.h"
#include "../../../../gpu_texture_cache.h"
#include "../../assistant_util/DelayedFrameTexture.h"

using namespace std;

#define DELAY_TIMEMILLS     200

class ImageCarouselEffect: public ImageBaseEffect {
public:
    ImageCarouselEffect();
    virtual ~ImageCarouselEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
    void destroy();
private:
    float currentFramePos;
    queue<DelayTextureFrame*> rightTopFrameBucket;
    float rightTopFramePos;
    queue<DelayTextureFrame*> leftBottomFrameBucket;
    float leftBottomFramePos;
    queue<DelayTextureFrame*> rightBottomFrameBucket;
    
    GPUTexture* initFramebuffers(EffectCallback * filterCallback);
    
    void cpyTextureFrame(OpenglVideoFrame * inputFrame);
    
    void drawRightBottomTextureWithCoords(GLfloat* textureCoords);
    void drawLeftBottomTextureWithCoords(GLfloat* textureCoords);
    void drawRightTopTextureWithCoords(GLfloat* textureCoords);
    void drawLeftTopTextureWithCoords(GLfloat* textureCoords);
    
};

#endif /* image_carousel_effect_h */
