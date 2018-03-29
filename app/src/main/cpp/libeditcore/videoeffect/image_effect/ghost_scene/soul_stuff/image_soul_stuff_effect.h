//
//  soul_stuff_effect.hpp
//  video_player
//
//  Created by apple on 16/10/15.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_soul_stuff_effect_h
#define image_soul_stuff_effect_h

#include "../../image_base_effect.h"
#include "../../mix_effect.h"
#include "../../../../gpu_texture_cache.h"
#include "../../assistant_util/image_transform_effect.h"

#define SOUL_STUFF_FREQUENCY        15

class ImageSoulStuffEffect: public ImageBaseEffect {
public:
    ImageSoulStuffEffect();
    virtual ~ImageSoulStuffEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
    virtual bool init();
    
    virtual void destroy();
    
private:
    OpenglVideoFrame * soulVideoFrame;
    GPUTexture* soulFrameTexture;
    bool isInitFrameBufferFlag;
    GPUTexture* initFramebuffers(EffectCallback * filterCallback);
    
    int soulFrameCounter;
    bool detectCpySoulTexture();
    void cpySoulTextureWithVertices(OpenglVideoFrame * inputFrame);
    
    ImageTransformEffect* transformEffect;
    OpenglVideoFrame * mixedVideoFrame;
    GPUTexture* mixedFrameTexture;
    void transformMixVideoFrame(EffectCallback * filterCallback, float progress);
    
    MixEffect* mixEffect;
    void outputFrame(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback, float progress);
    
};

#endif /* image_soul_stuff_effect_h */
