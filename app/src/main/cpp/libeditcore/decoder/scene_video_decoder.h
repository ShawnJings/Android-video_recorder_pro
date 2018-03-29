#ifndef SCENE_VIDEO_DECODER_H
#define SCENE_VIDEO_DECODER_H

#ifndef UINT64_C
#define UINT64_C(value)__CONCAT(value,ULL)
#endif

#ifndef INT64_MIN
#define INT64_MIN  (-9223372036854775807LL - 1)
#endif

#ifndef INT64_MAX
#define INT64_MAX	9223372036854775807LL
#endif

#include <list>
#include <vector>

#include "../platform_dependent/platform_common.h"
#include "../platform_dependent/platform_ffmpeg.h"
#include "./rgba_frame.h"

#ifndef SUBSCRIBE_VIDEO_DATA_TIME_OUT
#define SUBSCRIBE_VIDEO_DATA_TIME_OUT 20 * 1000
#endif

#ifndef DECODE_PROBESIZE_DEFAULT_VALUE
#define DECODE_PROBESIZE_DEFAULT_VALUE 50 * 1024
#endif

#define DEST_COLOR_FORMAT 			AV_PIX_FMT_RGBA

/**
 * 视频解码
 */
class SceneVideoDecoder {
protected:
	//保存需要读入的文件的格式信息，比如流的个数以及流数据等
	AVFormatContext *pFormatCtx;
	virtual int openInput(char *videoSourceURI, int* max_analyze_duration, int size, int probesize);
	virtual int openFormatInput(char *videoSourceURI);
	virtual void initFFMpegContext();
	virtual bool isNeedRetry();
	virtual int initAnalyzeDurationAndProbesize(int* max_analyze_durations, int analyzeDurationSize, int probesize);


protected:
	SwsContext * scxt;
	int destColorFormatSize;
	uint8_t* video_frame_rgba_buf;

	bool isSubscribe;
	bool isOpenInputSuccess;
	/** 超时的设置 **/
	AVIOInterruptCB int_cb;
	long readLatestFrameTimemills;
	bool isTimeout;

	float seek_seconds;

	/** 总的解码变量 **/
	bool is_Network;
	bool is_eof;
    float position;

	/** 视频流解码变量 **/
	//保存了相应流的详细编码信息，比如视频的宽、高，编码类型等
	AVCodecContext *videoCodecCtx;
	//真正的编解码器，其中有编解码需要调用的函数
	AVCodec *videoCodec;
	AVFrame *videoFrame;
	int width;
	int height;
	float fps;
	float videoTimeBase;
	std::list<int>* videoStreams;
	int videoStreamIndex;

	int openVideoStream();
	int openVideoStream(int streamIndex);
	std::list<int> *collectStreams(enum AVMediaType codecType);
	void avStreamFPSTimeBase(AVStream *st, float defaultTimeBase, float *pFPS, float *pTimeBase);

	RGBAFrame * handleVideoFrame();
	RGBAFrame * buildPicFrameByAVFrame(AVFrame *videoFrame, AVFrame *originalVideoFrame);
	void copyFrameData(uint8_t * dst, uint8_t * src, int width, int height, int linesize);
	void closeVideoStream();
public:
	SceneVideoDecoder();
	/** 开启解码线程 **/
	virtual ~SceneVideoDecoder();
	/** 打开指定URI的文件 **/
	/** 打开本地文件不需要传递 探针的参数以及重试策略 **/
	virtual int openFile(char *videoSourceURI);
	/** 打开网络文件需要传递 探针的参数以及重试策略 **/
	int openFile(char *videoSourceURI, int* max_analyze_duration, int size, int probesize);
	//仅仅解码一帧视频帧
	RGBAFrame* decodeVideoFrame(int* decodeVideoErrorState);
	std::vector<RGBAFrame*>* getCorrectFrame(float seconds, int* decodeVideoErrorState);

	bool isOpenInput(){
		return isOpenInputSuccess;
	};
	void stopSubscribe(){
		isSubscribe = false;
	};
	bool isSubscribed(){
		return isSubscribe;
	};
	float getVideoFrameRate(){
		return fps;
	};
	/** 关于超时的设置 **/
	static int interrupt_cb(void *ctx);
	int detectInterrupted();

	bool isEOF(){
		return is_eof;
	};
	bool isNetwork(){
		return is_Network;
	};
	bool validVideo(){
	    return videoStreamIndex != -1;
	};

	/** 关闭文件 **/
	virtual void closeFile();
	virtual void seek_frame();
	virtual void seek_frame(float seek_seconds);
	/** 当前解码到什么位置了，单位是秒，但是后边3位小数，其实是精确到毫秒 **/
	float getPosition(){
		return position;
	};

	/** 当前源的大小，单位是秒，但是后边3位小数，其实是精确到毫秒 **/
	float getDuration(){
		if(!pFormatCtx){
			return 0;
		}
	    if (pFormatCtx->duration == AV_NOPTS_VALUE){
	        return -1;
	    }
	    return (float)pFormatCtx->duration / AV_TIME_BASE;
	};

	/** 获得视频帧的宽度 **/
	int getVideoFrameWidth(){
		if(videoCodecCtx){
			return videoCodecCtx->width;
		}
		return -1;
	};

	/** 获得视频帧的高度 **/
	int getVideoFrameHeight(){
		if(videoCodecCtx){
			return videoCodecCtx->height;
		}
		return -1;
	};

};

#endif //SCENE_VIDEO_DECODER_H

