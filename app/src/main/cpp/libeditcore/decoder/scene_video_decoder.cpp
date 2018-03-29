#include "./scene_video_decoder.h"

#define LOG_TAG "SceneVideoDecoder"
using namespace platform;

SceneVideoDecoder::SceneVideoDecoder() {
}

SceneVideoDecoder::~SceneVideoDecoder() {
}

void SceneVideoDecoder::initFFMpegContext() {
	//注册所有支持的文件格式以及编解码器 之后就可以用所有ffmpeg支持的codec了
	avcodec_register_all();
	av_register_all();
}

int SceneVideoDecoder::openFile(char *videoSourceURI){
	int max_analyze_durations[3] = { 50000, 75000, 1000000 };
	int retrySize = 3;
	int probesize = 50 * 1024;
	return this->openFile(videoSourceURI, max_analyze_durations, retrySize, probesize);
}

int SceneVideoDecoder::openFile(char *videoSourceURI, int* max_analyze_duration, int size, int probesize) {
//	LOGI("SceneVideoDecoder::openFile");
	isSubscribe = true;
	isOpenInputSuccess = false;
	position = 0.0f;
	pFormatCtx = NULL;

	videoCodecCtx = NULL;
	videoFrame = NULL;
	videoStreams = NULL;
	scxt = NULL;
	video_frame_rgba_buf = NULL;

	this->initFFMpegContext();

	int errorCode = openInput(videoSourceURI, max_analyze_duration, size, probesize);
	//现在 pFormatCtx->streams 中已经有所有流了，因此现在我们遍历它找出对应的视频流、音频流、字幕流等：
	if (errorCode > 0) {
		int videoErr = openVideoStream();
		if (videoErr < 0) {
			errorCode = -1; // both fails
		}
	}
	isOpenInputSuccess = true;
	return errorCode;
}

int SceneVideoDecoder::openFormatInput(char *videoSourceURI) {
	//打开一个文件 只是读文件头，并不会填充流信息 需要注意的是，此处的pFormatContext必须为NULL或由avformat_alloc_context分配得到
	return avformat_open_input(&pFormatCtx, videoSourceURI, NULL, NULL);
}

int SceneVideoDecoder::initAnalyzeDurationAndProbesize(int* max_analyze_durations, int analyzeDurationSize, int probesize) {
	return 1;
}

bool SceneVideoDecoder::isNeedRetry(){
	return false;
}

int SceneVideoDecoder::openInput(char *videoSourceURI, int* max_analyze_durations, int analyzeDurationSize, int probesize) {
//	LOGI("SceneVideoDecoder::openInput");
	if (-1 == probesize) {
		probesize = DECODE_PROBESIZE_DEFAULT_VALUE;
	}

	readLatestFrameTimemills = getCurrentTime();
	isTimeout = false;
	pFormatCtx = avformat_alloc_context();
    int_cb;
    int_cb.callback = SceneVideoDecoder::interrupt_cb;
    int_cb.opaque = this;
	pFormatCtx->interrupt_callback = int_cb;
	//打开一个文件 只是读文件头，并不会填充流信息 需要注意的是，此处的pFormatContext必须为NULL或由avformat_alloc_context分配得到
	if (this->openFormatInput(videoSourceURI) != 0) {
		LOGI("Video decoder open input file failed... videoSourceURI is %s", videoSourceURI);
		avformat_free_context(pFormatCtx);
		return -1;
	}
	this->initAnalyzeDurationAndProbesize(max_analyze_durations, analyzeDurationSize, probesize);
	//获取文件中的流信息，此函数会读取packet，并确定文件中所有的流信息  设置pFormatCtx->streams指向文件中的流，但此函数并不会改变文件指针，读取的packet会给后面的解码进行处理
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		LOGI("Video decoder Stream info not found...");
		avformat_free_context(pFormatCtx);
		return -1;
	}
	is_eof = false;
	//输出文件的信息，也就是我们在使用ffmpeg时能看到的文件详细信息
//	av_dump_format(pFormatCtx, -1, videoSourceURI, 0);
	if (pFormatCtx->streams[0]->codec->codec_id == AV_CODEC_ID_NONE) {
		avformat_close_input(&pFormatCtx);
		avformat_free_context(pFormatCtx);
		if (this->isNeedRetry()) {
			return openInput(videoSourceURI, max_analyze_durations, analyzeDurationSize, probesize);
		} else {
			return -1;
		}
	}
	return 1;
}

int SceneVideoDecoder::openVideoStream() {
	int errCode = -1;
	videoStreamIndex = -1;
	videoStreams = collectStreams(AVMEDIA_TYPE_VIDEO);
	LOGI("videoStreams size is %d", videoStreams->size());
	std::list<int>::iterator i;
	for (i = videoStreams->begin(); i != videoStreams->end(); ++i) {
		int iStream = *i;
		LOGI("videoStreamIndex is %d", iStream);
		if (0 == (pFormatCtx->streams[iStream]->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
			errCode = openVideoStream(iStream);
			if (errCode < 0)
				break;
		}
	}
	return errCode;
}

int SceneVideoDecoder::openVideoStream(int streamIndex) {
	LOGI("SceneVideoDecoder::openVideoStream");
	//1、get a pointer to the codec context for the video stream
	AVStream *videoStream = pFormatCtx->streams[streamIndex];
	videoCodecCtx = videoStream->codec;
	//2、通过codecContext的codec_id 找出对应的decoder
	videoCodec = avcodec_find_decoder(videoCodecCtx->codec_id);
	LOGI("CODEC_ID_H264 is %d videoCodecCtx->codec_id is %d", CODEC_ID_H264, videoCodecCtx->codec_id);
	if (videoCodec == NULL) {
		LOGI("can not find the videoStream's Codec ...");
		return -1;
	}
	//3、打开找出的decoder
	if (avcodec_open2(videoCodecCtx, videoCodec, NULL) < 0) {
		LOGI("open video codec failed...");
		return -1;
	}
	//4、分配图像缓存:准备给即将解码的图片分配内存空间 调用 avcodec_alloc_frame 分配帧,videoFrame用于存储解码后的数据
	videoFrame = avcodec_alloc_frame();
	if (videoFrame == NULL) {
		LOGI("alloc video frame failed...");
		avcodec_close(videoCodecCtx);
		return -1;
	}
	//5、now: we think we can Correctly identify the video stream
	this->videoStreamIndex = streamIndex;
	//6、determine fps and videoTimeBase
	avStreamFPSTimeBase(videoStream, 0.04, &fps, &videoTimeBase);
//	LOGI("video codec size: fps: %.3f tb: %f", fps, videoTimeBase);
//	LOGI("video start time %f", videoStream->start_time * videoTimeBase);
//	LOGI("video disposition %d", videoStream->disposition);
	//get video width and height
	width = videoCodecCtx->width;
	height = videoCodecCtx->height;
	int gop_size= videoCodecCtx->gop_size;
	int max_b_frames= videoCodecCtx->max_b_frames;
	AVPixelFormat originColorFormat = videoCodecCtx->pix_fmt;
	LOGI("AV_PIX_FMT_BGRA is %d", AV_PIX_FMT_BGRA);
	LOGI("originColorFormat is %d", originColorFormat);
	LOGI("gop_size is %d max_b_frames is %d fps is %f width is %d height is %d", gop_size, max_b_frames, fps, width, height);
	if (originColorFormat != DEST_COLOR_FORMAT) {
		LOGI("we must convert to RGB");
		scxt = sws_getContext(width, height, originColorFormat, width, height, DEST_COLOR_FORMAT,
		SWS_POINT, NULL, NULL, NULL);
		destColorFormatSize = avpicture_get_size(DEST_COLOR_FORMAT, width, height);
		video_frame_rgba_buf = (uint8_t *) av_malloc(destColorFormatSize);
	}
	return 1;
}

std::list<int>* SceneVideoDecoder::collectStreams(enum AVMediaType codecType) {
	std::list<int> *ma = new std::list<int>();
	for (int i = 0; i < pFormatCtx->nb_streams; i++) {
		if (codecType == pFormatCtx->streams[i]->codec->codec_type) {
			ma->push_back(i);
		}
	}
	return ma;
}

int SceneVideoDecoder::interrupt_cb(void *ctx) {
	SceneVideoDecoder* sceneVideoDecoder = (SceneVideoDecoder*) ctx;
	return sceneVideoDecoder->detectInterrupted();
}

int SceneVideoDecoder::detectInterrupted() {
	// do something
	if (getCurrentTime() - readLatestFrameTimemills > SUBSCRIBE_VIDEO_DATA_TIME_OUT) {
		LOGI("SceneVideoDecoder::interrupt_cb");
		isTimeout = true;
		return 1;
	} else {
		return 0;
	}
}

void SceneVideoDecoder::avStreamFPSTimeBase(AVStream *st, float defaultTimeBase, float *pFPS, float *pTimeBase) {
	float fps, timebase;

	if (st->time_base.den && st->time_base.num)
		timebase = av_q2d(st->time_base);
	else if (st->codec->time_base.den && st->codec->time_base.num)
		timebase = av_q2d(st->codec->time_base);
	else
		timebase = defaultTimeBase;

	if (st->codec->ticks_per_frame != 1) {
		LOGI("WARNING: st.codec.ticks_per_frame=%d", st->codec->ticks_per_frame);
	}

	if (st->avg_frame_rate.den && st->avg_frame_rate.num)
		fps = av_q2d(st->avg_frame_rate);
	else if (st->r_frame_rate.den && st->r_frame_rate.num)
		fps = av_q2d(st->r_frame_rate);
	else
		fps = 1.0 / timebase;
	if (pFPS) {
		LOGI("assign fps value");
		*pFPS = fps;
	}
	if (pTimeBase) {
		LOGI("assign pTimeBase value");
		*pTimeBase = timebase;
	}
}

void SceneVideoDecoder::closeFile() {
	this->stopSubscribe();
	closeVideoStream();

	if (NULL != pFormatCtx) {
		pFormatCtx->interrupt_callback.opaque = NULL;
		pFormatCtx->interrupt_callback.callback = NULL;
		avformat_close_input(&pFormatCtx);
		avformat_free_context(pFormatCtx);
		pFormatCtx = NULL;
	}
}

void SceneVideoDecoder::closeVideoStream() {
	videoStreamIndex = -1;
	if(video_frame_rgba_buf){
		av_free(video_frame_rgba_buf);
	}
	if(scxt){
		sws_freeContext(scxt);
	}
	if (NULL != videoFrame) {
		avcodec_free_frame(&videoFrame);
		videoFrame = NULL;
	}
	if (NULL != videoCodecCtx) {
		avcodec_close(videoCodecCtx);
		videoCodecCtx = NULL;
	}
	if (NULL != videoStreams) {
		delete videoStreams;
		videoStreams = NULL;
	}
}

void SceneVideoDecoder::seek_frame(float seek_seconds) {
	this->seek_seconds = seek_seconds;
	this->seek_frame();
}

void SceneVideoDecoder::seek_frame() {
	LOGI("enter SceneVideoDecoder::seek_frame seek_seconds is %f", seek_seconds);
	int64_t seek_target = seek_seconds * 1000000;
	int64_t seek_min = INT64_MIN;
	int64_t seek_max = INT64_MAX;
//	LOGI("before avformat_seek_file...");
	int ret = avformat_seek_file(pFormatCtx, -1, seek_min, seek_target, seek_max, 0);
	LOGI("after avformat_seek_file... ret is %d", ret);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "cc: error while seeking\n");
	} else {
		avcodec_flush_buffers(videoCodecCtx);
	}
	LOGI("leave SceneVideoDecoder::seek_frame");
}

std::vector<RGBAFrame*>* SceneVideoDecoder::getCorrectFrame(float seconds, int* decodeVideoErrorState) {
	std::vector<RGBAFrame*>* frames = new std::vector<RGBAFrame*>();
	seek_seconds = seconds - 0.5;
	this->seek_frame();
	RGBAFrame* fadeInFrame = NULL;
	RGBAFrame* fadeOutFrame = NULL;
	fadeInFrame = this->decodeVideoFrame(decodeVideoErrorState);
	LOGI("expect seconds is %.3f once seek position is %.3f", seconds, fadeInFrame->position);
	while (fadeInFrame->position < seconds) {
		if(NULL != fadeOutFrame){
			delete fadeOutFrame;
		}
		fadeOutFrame = fadeInFrame;
		fadeInFrame = this->decodeVideoFrame(decodeVideoErrorState);
	}
	if(NULL == fadeOutFrame){
		fadeOutFrame = fadeInFrame->clone();
	}
	LOGI("fadeOutFrame->position is %.3f expect seconds is %.3f fadeInFrame->position is %.3f",
			fadeOutFrame->position, seconds, fadeInFrame->position);
	seek_seconds = 0.0f;
	this->seek_frame();
	LOGI("leave getCorrectFrame ");
	frames->push_back(fadeOutFrame);
	frames->push_back(fadeInFrame);
	return frames;
}

RGBAFrame* SceneVideoDecoder::decodeVideoFrame(int* decodeVideoErrorState) {
//	LOGI("enter SceneVideoDecoder::decodeVideoFrame...");
	RGBAFrame *frame = NULL;
	if (!isSubscribe || NULL == pFormatCtx) {
		return frame;
	}
	if (-1 == videoStreamIndex) {
		return frame;
	}
	readLatestFrameTimemills = getCurrentTime();
	AVPacket packet;
	float decodedDuration = 0.0f;
//	LOGI("read a packet...");
	while (true) {
		if (av_read_frame(pFormatCtx, &packet) < 0) {
			LOGI("decode file eof!!!");
			av_free_packet(&packet);
//			is_eof = true;
//			return frame;
			seek_seconds = 0.0f;
			this->seek_frame();
			continue;
		}
		if (packet.stream_index == videoStreamIndex) {
			int pktSize = packet.size;
			while (pktSize > 0) {
				int gotframe = 0;
				int len = avcodec_decode_video2(videoCodecCtx, videoFrame, &gotframe, &packet);
				if (len < 0) {
					LOGI("decode video error, skip packet");
					*decodeVideoErrorState = 1;
					break;
				}
				if (gotframe) {
					if (videoFrame->interlaced_frame) {
						avpicture_deinterlace((AVPicture*) videoFrame, (AVPicture*) videoFrame, videoCodecCtx->pix_fmt, videoCodecCtx->width, videoCodecCtx->height);
					}
					//				LOGI("decode a packet to a frame successed...");
					frame = handleVideoFrame();
				}
				if (0 == len) {
					av_free_packet(&packet);
					break;
				}
				pktSize -= len;
			}
			if(NULL != frame){
				av_free_packet(&packet);
				break;
			}
		}
		av_free_packet(&packet);
	}
//	LOGI("leave SceneVideoDecoder::decodeVideoFrame...");
	return frame;
}


RGBAFrame * SceneVideoDecoder::handleVideoFrame() {
//	LOGI("enter PicDecoder::handleVideoFrame()...");
	if (!videoFrame->data[0]) {
		LOGI("videoFrame->data[0] is 0... why...");
		return NULL;
	}
	RGBAFrame *rgbaFrame = NULL;
	if (NULL != scxt) {
//		LOGI("use sws scale ...");
		AVFrame *video_rgba_frame = avcodec_alloc_frame();
		avpicture_fill((AVPicture *) video_rgba_frame, video_frame_rgba_buf,
		DEST_COLOR_FORMAT, width, height);

		sws_scale(scxt, videoFrame->data, videoFrame->linesize, 0, height, video_rgba_frame->data, video_rgba_frame->linesize);
		rgbaFrame = buildPicFrameByAVFrame(video_rgba_frame, videoFrame);
		avcodec_free_frame(&video_rgba_frame);
	} else {
		rgbaFrame = buildPicFrameByAVFrame(videoFrame, videoFrame);
	}
//	LOGI("VFD: %.4f %.4f | %lld ", yuvFrame->position, yuvFrame->duration, av_frame_get_pkt_pos(videoFrame));
//	LOGI("leave PicDecoder::handleVideoFrame()...");
	return rgbaFrame;
}

RGBAFrame * SceneVideoDecoder::buildPicFrameByAVFrame(AVFrame *videoFrame, AVFrame *originalVideoFrame) {
	//	LOGI("enter buildPicFrameByAVFrame...");
		RGBAFrame *rgbaFrame = new RGBAFrame();
		int width = MIN(videoFrame->linesize[0], videoCodecCtx->width);
		int height = videoCodecCtx->height;
		int pixelsLength = width * height * 4;
		uint8_t * pixels = new uint8_t[pixelsLength];
		memcpy(pixels, videoFrame->data[0], pixelsLength);
		rgbaFrame->pixels = pixels;

		rgbaFrame->width = videoCodecCtx->width;
		rgbaFrame->height = videoCodecCtx->height;
		/** av_frame_get_best_effort_timestamp 实际上获取AVFrame的 int64_t best_effort_timestamp; 这个Filed **/
		rgbaFrame->position = av_frame_get_best_effort_timestamp(originalVideoFrame) * videoTimeBase;

		const int64_t frameDuration = av_frame_get_pkt_duration(originalVideoFrame);
		if (frameDuration) {
			rgbaFrame->duration = frameDuration * videoTimeBase;
			rgbaFrame->duration += originalVideoFrame->repeat_pict * videoTimeBase * 0.5;
		} else {
			rgbaFrame->duration = 1.0 / fps;
		}
	//	LOGI("leave buildPicFrameByAVFrame...");
		return rgbaFrame;
}

void SceneVideoDecoder::copyFrameData(uint8_t * dst, uint8_t * src, int width, int height, int linesize) {
	for (int i = 0; i < height; ++i) {
		memcpy(dst, src, width);
		dst += width;
		src += linesize;
	}
}
