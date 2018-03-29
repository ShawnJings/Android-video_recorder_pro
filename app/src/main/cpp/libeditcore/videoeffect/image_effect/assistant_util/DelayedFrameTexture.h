//
//  DelayedFrameTexture.hpp
//  video_player
//
//  Created by apple on 16/10/17.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#ifndef DelayedFrameTexture_h
#define DelayedFrameTexture_h

#include "../../../gpu_texture_cache.h"
#include "../../../common/opengl_video_frame.h"

class DelayTextureFrame {
public:
    DelayTextureFrame(ImagePosition& imgPos, float position);
    virtual ~DelayTextureFrame();
    
    GLuint getTextureId();
    float getPosition();
private:
    OpenglVideoFrame* videoFrame;
    GPUTexture* texture;
    
};

#endif /* DelayedFrameTexture_h */
