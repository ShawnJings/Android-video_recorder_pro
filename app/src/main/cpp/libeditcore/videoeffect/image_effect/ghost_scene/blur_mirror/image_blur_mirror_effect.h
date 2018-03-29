//
//  blur_mirror_effect.hpp
//  video_player
//
//  Created by apple on 16/10/1.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef image_blur_mirror_effect_h
#define image_blur_mirror_effect_h

#include "../../image_base_effect.h"
#include "../../../../gpu_texture_cache.h"
#include "../../../blur_scene/blur_scene_effect.h"

class ImageBlurMirrorEffect: public ImageBaseEffect {
public:
    ImageBlurMirrorEffect();
    virtual ~ImageBlurMirrorEffect();
    
    virtual void renderEffect(OpenglVideoFrame * inputFrame, OpenglVideoFrame * outputFrame, EffectCallback * filterCallback);
    
    virtual bool init();

    void destroy();
private:
    BlurSceneEffect* blurEffect;
    GPUTexture* blurFrameTexture;
    bool isInitFrameBufferFlag;
    GPUTexture* initFramebuffers(EffectCallback * filterCallback);
    
    OpenglVideoFrame * blurVideoFrame;
    void drawTextureWithVertices(OpenglVideoFrame * inputFrame, GLfloat* vertices, GLfloat* textureCoords);
};

#endif /* image_blur_mirror_effect_h */
