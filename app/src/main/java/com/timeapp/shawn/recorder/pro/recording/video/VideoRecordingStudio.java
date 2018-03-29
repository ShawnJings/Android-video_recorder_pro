package com.timeapp.shawn.recorder.pro.recording.video;

import android.content.res.AssetManager;
import android.util.Log;

import com.timeapp.shawn.recorder.pro.Videostudio;
import com.timeapp.shawn.recorder.pro.audioeffect.AudioEffect;
import com.timeapp.shawn.recorder.pro.recording.RecordingImplType;
import com.timeapp.shawn.recorder.pro.recording.camera.exception.CameraParamSettingException;
import com.timeapp.shawn.recorder.pro.recording.camera.preview.ChangbaRecordingPreviewScheduler;
import com.timeapp.shawn.recorder.pro.recording.camera.preview.PreviewFilterType;
import com.timeapp.shawn.recorder.pro.recording.exception.InitRecorderFailException;
import com.timeapp.shawn.recorder.pro.recording.exception.RecordingStudioException;
import com.timeapp.shawn.recorder.pro.recording.exception.StartRecordingException;
import com.timeapp.shawn.recorder.pro.recording.service.PlayerService;
import com.timeapp.shawn.recorder.pro.recording.video.service.MediaRecorderService;
import com.timeapp.shawn.recorder.pro.recording.video.service.factory.MediaRecorderServiceFactory;


public abstract class VideoRecordingStudio {

    public interface RecordingStudioStateCallback {
        void onConnectRTMPServerFailed();

        void onConnectRTMPServerSuccessed();

        void onStartRecordingException(StartRecordingException exception);

        void onPublishTimeOut();

        void adaptiveVideoQuality(int videoQuality);

        void hotAdaptiveVideoQuality(int maxBitrate, int avgBitrate, int fps);
        
        void statisticsBitrateCallback(int sendBitrate, int compressedBitrate);

        void statisticsCallback(long startTimeMills, int connectTimeMills, int publishDurationInSec,
                                float discardFrameRatio, float publishAVGBitRate, float expectedBitRate, String adaptiveBitrateChart);
    }

    public static final int HIGHT_VIDEO_QUALITY = 0;
    public static final int MIDDLE_VIDEO_QUALITY = 1;
    public static final int LOW_VIDEO_QUALITY = 2;
    public static final int INVLAID_QUALITY = -1;

    public static final int COMMON_VIDEO_BIT_RATE = 520 * 1000;
    public static int initializeVideoBitRate = COMMON_VIDEO_BIT_RATE;
    
    public static final int VIDEO_FRAME_RATE = 24;
    public static final int MIDDLE_VIDEO_BIT_RATE = 390 * 1000;
    public static final int MIDDLE_VIDEO_FRAME_RATE = 20;
    public static final int LOW_VIDEO_BIT_RATE = 300 * 1000;
    public static final int LOW_VIDEO_FRAME_RATE = 15;
    public static final int audioSampleRate = 44100;
    public static final int audioChannels = 2;
    public static final int audioBitsDepth = 16;
    public static final int audioBitRate = 64 * 1000;
    protected static int SAMPLE_RATE_IN_HZ = 44100;

    public static final int MEDIA_CODEC_NOSIE_DELTA = 80*1024;

    protected PlayerService playerService = null;

    protected MediaRecorderService recorderService = null;

    protected RecordingStudioStateCallback recordingStudioStateCallback = null;

    // 输出video的路径
    protected RecordingImplType recordingImplType;

    public VideoRecordingStudio() {
    }

    public VideoRecordingStudio(RecordingImplType recordingImplType, RecordingStudioStateCallback recordingStudioStateCallback) {
        this.recordingImplType = recordingImplType;
        this.recordingStudioStateCallback = recordingStudioStateCallback;
    }

    public void initRecordingResource(ChangbaRecordingPreviewScheduler scheduler, AudioEffect audioEffect) throws RecordingStudioException {
        // 实例化以及init录音器
        /**
         * 这里一定要注意顺序，先初始化record在初始化player，因为player中要用到recorder中的samplerateSize
         **/
        recorderService = MediaRecorderServiceFactory.getInstance().getRecorderService(scheduler, recordingImplType);
        recorderService.initMetaData();
        if (!recorderService.initMediaRecorderProcessor(audioEffect)) {
            throw new InitRecorderFailException();
        }
    }

    public synchronized void destroyRecordingResource() {
        if (recorderService != null) {
            recorderService.stop();
            recorderService.destoryMediaRecorderProcessor();
            recorderService = null;
        }
    }
    
    public static int getInitializeVideoBitRate(){
    	return initializeVideoBitRate;
    }

    // here define use MediaCodec or not
    public void startVideoRecording(final String outputPath, final int bitRate, final int videoWidth, final int videoHeight,
                                    final int audioSampleRate,
                                    final int qualityStrategy, final int adaptiveBitrateWindowSizeInSecs, final int adaptiveBitrateEncoderReconfigInterval,
                                    final int adaptiveBitrateWarCntThreshold, final int adaptiveMinimumBitrate,
                                    final int adaptiveMaximumBitrate, final boolean useHardWareEncoding) {
    	//设置初始编码率
    	if (bitRate > 0){
    		initializeVideoBitRate = bitRate*1000;
    		Log.i("problem", "initializeVideoBitRate:" + initializeVideoBitRate + ",useHardWareEncoding:" + useHardWareEncoding);
    	}
    	
        new Thread() {
            public void run() {
                try {
                    //这里面不应该是根据Producer是否选用硬件编码器再去看Consumer端, 这里应该对于Consumer端是透明的
                    int ret = startConsumer(outputPath, videoWidth, videoHeight, audioSampleRate,
                            qualityStrategy,adaptiveBitrateWindowSizeInSecs, 
                            adaptiveBitrateEncoderReconfigInterval, adaptiveBitrateWarCntThreshold,adaptiveMinimumBitrate, adaptiveMaximumBitrate);
                    if (ret < 0) {
                        destroyRecordingResource();
                        VideoRecordingStudio.this.recordingStudioStateCallback.onConnectRTMPServerFailed();
                    } else {
                        Videostudio.getInstance().connectSuccess();
                        VideoRecordingStudio.this.recordingStudioStateCallback.onConnectRTMPServerSuccessed();
                        startProducer(videoWidth, videoHeight, useHardWareEncoding, qualityStrategy);
                    }
                } catch (StartRecordingException exception) {
                    //启动录音失败，需要把资源销毁，并且把消费者线程停止掉
                    stopRecording();
                    VideoRecordingStudio.this.recordingStudioStateCallback.onStartRecordingException(exception);
                }
            }
        }.start();
    }

    protected abstract int startConsumer(final String outputPath, final int videoWidth, final int videoHeight, final int audioSampleRate,
                                         int qualityStrategy, int adaptiveBitrateWindowSizeInSecs, int adaptiveBitrateEncoderReconfigInterval,
                                         int adaptiveBitrateWarCntThreshold, int adaptiveMinimumBitrate,
                                         int adaptiveMaximumBitrate);

    protected abstract boolean startProducer(final int videoWidth, int videoHeight, boolean useHardWareEncoding, int strategy) throws StartRecordingException;

    public void stopRecording() {
        destroyRecordingResource();
        Videostudio.getInstance().stopRecord();
    }

    /**
     * 获取录音器的参数
     **/
    public int getRecordSampleRate() {
        if (recorderService != null) {
            return recorderService.getSampleRate();
        }
        return SAMPLE_RATE_IN_HZ;
    }

    public int getAudioBufferSize() {
        if (recorderService != null) {
            return recorderService.getAudioBufferSize();
        }
        return SAMPLE_RATE_IN_HZ;
    }

    public void switchCamera() throws CameraParamSettingException {
        if (recorderService != null) {
            recorderService.switchCamera();
        }
    }

    public void switchPreviewFilter(AssetManager assetManager, PreviewFilterType filterType) {
        if (null != recorderService) {
            recorderService.switchPreviewFilter(assetManager, filterType);
        }
    }

    public void setAudioEffect(AudioEffect audioEffect) {
        if (null != recorderService) {
            recorderService.setAudioEffect(audioEffect);
        }
    }

}
