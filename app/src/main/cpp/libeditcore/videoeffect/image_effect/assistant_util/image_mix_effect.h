//
//  image_mix_effect.hpp
//  video_player
//
//  Created by apple on 16/9/29.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_mix_effect_hpp
#define image_mix_effect_hpp

#include "../image_base_effect.h"

typedef enum MixMode_t{
    Screen,
    Add,
    Overlay,
    HightLight,
    SoftLight,
} MixMode;

class ImageMixEffect: public ImageBaseEffect {
public:
    ImageMixEffect();
    virtual ~ImageMixEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
    void addInputEffect(ImageBaseEffect* effect, int busNum);
private:
    MixMode                 mixMode;
    
};

#endif /* image_mix_effect_hpp */
