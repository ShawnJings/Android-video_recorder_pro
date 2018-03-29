#ifndef VIDEO_ENCODER_X264_ENCODER_H
#define VIDEO_ENCODER_X264_ENCODER_H

#include "CommonTools.h"
#include "libvideo_consumer/live_common_packet_pool.h"
#include "../../../liblivecore/common/live_video_packet_queue.h"
#include "./color_conversion/color_conversion.h"
#include "./h264_util.h"
#include "../../../liblivecore/common/publisher_rate_feed_back.h"
#include <vector>

extern "C" {
#include "./../../../3rdparty/ffmpeg/include/libavutil/opt.h"
#include "./../../../3rdparty/ffmpeg/include/libavutil/mathematics.h"
#include "./../../../3rdparty/ffmpeg/include/libavformat/avformat.h"
#include "./../../../3rdparty/ffmpeg/include/libswscale/swscale.h"
#include "./../../../3rdparty/ffmpeg/include/libswresample/swresample.h"
#include "./../../../3rdparty/ffmpeg/include/libavutil/imgutils.h"
#include "./../../../3rdparty/ffmpeg/include/libavutil/samplefmt.h"
#include "./../../../3rdparty/ffmpeg/include/libavutil/timestamp.h"
#include "./../../../3rdparty/ffmpeg/include/libavcodec/avcodec.h"
#include "./../../../3rdparty/ffmpeg/include/libavfilter/avfiltergraph.h"
#include "./../../../3rdparty/ffmpeg/include/libavfilter/avcodec.h"
#include "./../../../3rdparty/ffmpeg/include/libavfilter/buffersink.h"
#include "./../../../3rdparty/ffmpeg/include/libavfilter/buffersrc.h"
#include "./../../../3rdparty/ffmpeg/include/libavutil/avutil.h"
#include "./../../../3rdparty/ffmpeg/include/libswscale/swscale.h"
}

using namespace std;

#define X264_INPUT_COLOR_FORMAT 					AV_PIX_FMT_YUV420P
#define ORIGIN_COLOR_FORMAT     					AV_PIX_FMT_YUYV422

class VideoX264Encoder {
private:
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVPacket pkt;
	/** 扔给x264编码器编码的Frame **/
	uint8_t* picture_buf;
	AVFrame* pFrame;
	/** 从预览线程那边扔过来的YUY2数据 **/
	uint8_t* yuy2_picture_buf;
	AVFrame* video_yuy2_frame;

	LiveCommonPacketPool* packetPool;
	bool isSPSUnWriteFlag;
	const int delta = 30*1000;

	int frameRate;
	int strategy = 0;

	int allocVideoStream(int width, int height, int videoBitRate, float frameRate);
	void allocAVFrame();
public:
	VideoX264Encoder(int strategy);


    virtual ~VideoX264Encoder();

    int init(int width, int height, int videoBitRate, float frameRate, LiveCommonPacketPool* packetPool);
    int encode(LiveVideoPacket *videoPacket);
    void reConfigure(int bitRate);
    void pushToQueue(byte* buffer, int size, int timeMills, int64_t pts, int64_t dts);
    int destroy();
};
#endif // VIDEO_ENCODER_X264_ENCODER_H
