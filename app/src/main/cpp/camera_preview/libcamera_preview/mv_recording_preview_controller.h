#ifndef MV_RECORDING_PREVIEW_CONTROLLER_H
#define MV_RECORDING_PREVIEW_CONTROLLER_H

#include <unistd.h>
#include <pthread.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "egl_core/egl_core.h"
#include "libvideo_encoder/hw_encoder/hw_encoder_adapter.h"
#include "recording_preview_renderer.h"
#include "libvideo_encoder/soft_encoder/soft_encoder_adapter.h"

#define CAMERA_FACING_BACK                                        0
#define CAMERA_FACING_FRONT                                        1

enum RenderThreadMessage {
    MSG_RENDER_FRAME = 0,
    MSG_EGL_THREAD_CREATE,
    MSG_EGL_CREATE_PREVIEW_SURFACE,
    MSG_SWITCH_CAMERA_FACING,
    MSG_SWITCH_FILTER,
    MSG_START_RECORDING,
    MSG_STOP_RECORDING,
    MSG_EGL_DESTROY_PREVIEW_SURFACE,
    MSG_EGL_THREAD_EXIT
};

class MVRecordingPreviewHandler;

class MVRecordingPreviewController {
public:
    MVRecordingPreviewController();

    virtual ~MVRecordingPreviewController();

    /** 1:准备EGL Context与EGLThread **/
    void prepareEGLContext(ANativeWindow *window, JavaVM *g_jvm, jobject obj, int screenWidth,
                           int screenHeight, int cameraFacingId);

    /** 2:当Camera捕捉到新的一帧图像会调用 **/
    void notifyFrameAvailable();

    /** 3:切换视频滤镜 **/
    void switchPreviewFilter(int filterType, byte *acvBuffer, int bufferSize);

    /** 4:切换摄像头转向 **/
    void switchCameraFacing();

    /** 调整视频编码质量 **/
    void adaptiveVideoQuality(int maxBitRate, int avgBitRate, int fps);

    void hotConfig(int bitRate, int fps, int gopSize);

    void hotConfigQuality(int maxBitrate, int avgBitrate, int fps);

    /** 5:重置绘制区域大小 **/
    void resetRenderSize(int screenWidth, int screenHeight);

    /** 6:切换预览状态(暂停录音的预览状态,普通预览状态) **/
    void switchPauseRecordingPreviewState();

    void switchCommonPreviewState();

    /** 7:销毁EGLContext与EGLThread **/
    virtual void destroyEGLContext();

    // 保存从java层传下来的纹理矩阵
    void updateTexMatrix(float *texMatrix);

    int setBeautifyParam(int key, float value);

protected:
    /** 启动预览线程的五重要个参数 **/
    ANativeWindow *_window;
    JavaVM *g_jvm;
    jobject obj;
    int screenWidth;
    int screenHeight;

    bool isInSwitchingCamera;

    int64_t startTime;

    /** 将输入纹理传回Camera配置结束之后，返回CameraInfo **/
    int facingId;
    int degress;
    int textureWidth;
    int textureHeight;

    int cameraWidth;
    int cameraHeight;

    /** 切换视频滤镜的参数 **/
    int filterType;
    //色调曲线调整的参数
    byte *mACVBuffer;
    int mACVBufferSize;

    /** 切换预览的状态用到的变量 **/
    bool pauseFlag;
    bool continueFlag;

    /** 保存camera过来的纹理矩阵 **/
    float mTexMatrix[16];

    MVRecordingPreviewHandler *handler;
    MessageQueue *queue;
    pthread_t _threadId;

    static void *threadStartCallback(void *myself);

    void processMessage();

    /** 线程中核心的处理方法 **/
    //创建EGL资源以及调用Android创建Camera
    EGLCore *eglCore;
    EGLSurface previewSurface;
    //它负责处理:拷贝纹理(copier)、处理纹理(processor)、输出纹理(render) 核心操作
    RecordingPreviewRenderer *renderer;

    virtual void buildRenderInstance();

    virtual void processVideoFrame(float position);

    void draw();

    void configCamera();

    void startCameraPreview();

    void updateTexImage();

    void releaseCamera();

protected:
    bool isEncoding;
    VideoEncoderAdapter *encoder;

public:
    void startEncoding(int width, int height, int videoBitRate, float frameRate,
                       bool useHardWareEncoding,int strategy);

    void stopEncoding();

    void createWindowSurface(ANativeWindow *window);

    void destroyWindowSurface();

public:
    virtual bool initialize();

    //销毁EGL资源并且调用Andorid销毁Camera
    virtual void destroy();

    void createPreviewSurface();

    void destroyPreviewSurface();

    void switchCamera();

    void switchFilter();

    void startRecording();

    void stopRecording();

    void renderFrame();
};


class MVRecordingPreviewHandler : public Handler {
private:
    MVRecordingPreviewController *previewController;

public:
    MVRecordingPreviewHandler(MVRecordingPreviewController *previewController, MessageQueue *queue)
            :
            Handler(queue) {
        this->previewController = previewController;
    }

    void handleMessage(Message *msg) {
        int what = msg->getWhat();
        switch (what) {
            case MSG_EGL_THREAD_CREATE:
                previewController->initialize();
                break;
            case MSG_EGL_CREATE_PREVIEW_SURFACE:
                previewController->createPreviewSurface();
                break;
            case MSG_SWITCH_CAMERA_FACING:
                previewController->switchCamera();
                break;
            case MSG_SWITCH_FILTER:
                previewController->switchFilter();
                break;
            case MSG_START_RECORDING:
                previewController->startRecording();
                break;
            case MSG_STOP_RECORDING:
                previewController->stopRecording();
                break;
            case MSG_EGL_DESTROY_PREVIEW_SURFACE:
                previewController->destroyPreviewSurface();
                break;
            case MSG_EGL_THREAD_EXIT:
                previewController->destroy();
                break;
            case MSG_RENDER_FRAME:
                previewController->renderFrame();
                break;
        }
    }
};

#endif // MV_RECORDING_PREVIEW_CONTROLLER_H
