//
//  DelayedFrameTexture.cpp
//  video_player
//
//  Created by apple on 16/10/17.
//  Copyright © 2016年 xiaokai.zhan. All rights reserved.
//

#include "DelayedFrameTexture.h"

#define LOG_TAG "DelayTextureFrame"

DelayTextureFrame::DelayTextureFrame(ImagePosition& imgPos, float position) {
    int width = imgPos.width;
    int height = imgPos.height;
    this->texture = GPUTextureCache::GetInstance()->fetchTexture(width, height);
    texture->lock();
    this->videoFrame = new OpenglVideoFrame(texture->getTexId(), imgPos, position);
}

float DelayTextureFrame::getPosition(){
    return videoFrame->getStreamPosition();
}

GLuint DelayTextureFrame::getTextureId() {
    return texture->getTexId();
}

DelayTextureFrame::~DelayTextureFrame() {
    if(texture){
        texture->unLock();
    }
    if(videoFrame){
        delete videoFrame;
        videoFrame = NULL;
    }
}
