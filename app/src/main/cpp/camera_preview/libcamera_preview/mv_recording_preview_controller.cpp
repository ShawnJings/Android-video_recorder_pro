#include "mv_recording_preview_controller.h"

#define LOG_TAG "MVRecordingPreviewController"

MVRecordingPreviewController::MVRecordingPreviewController() {
    LOGI("MVRecordingPreviewController instance created");
    facingId = CAMERA_FACING_FRONT;
    filterType = 0;
    startTime = -1;
    eglCore = NULL;
    _window = NULL;
    encoder = NULL;
    previewSurface = EGL_NO_SURFACE;
    isEncoding = false;
    queue = new MessageQueue("MVRecordingPreviewController message queue");
    handler = new MVRecordingPreviewHandler(this, queue);
}

MVRecordingPreviewController::~MVRecordingPreviewController() {
    LOGI("MVRecordingPreviewController instance destroyed");
}

void MVRecordingPreviewController::prepareEGLContext(ANativeWindow *window, JavaVM *g_jvm,
                                                     jobject obj, int screenWidth, int screenHeight,
                                                     int cameraFacingId) {
    LOGI("Creating MVRecordingPreviewController thread");
    this->g_jvm = g_jvm;
    this->obj = obj;
    this->_window = window;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->facingId = cameraFacingId;
    handler->postMessage(new Message(MSG_EGL_THREAD_CREATE));
    pthread_create(&_threadId, 0, threadStartCallback, this);
}

void *MVRecordingPreviewController::threadStartCallback(void *myself) {
    MVRecordingPreviewController *previewController = (MVRecordingPreviewController *) myself;
    previewController->processMessage();
    pthread_exit(0);
    return NULL;
}

void MVRecordingPreviewController::processMessage() {
    bool renderingEnabled = true;
    while (renderingEnabled) {
        Message *msg = NULL;
        if (queue->dequeueMessage(&msg, true) > 0) {
            if (MESSAGE_QUEUE_LOOP_QUIT_FLAG == msg->execute()) {
                renderingEnabled = false;
            }
            delete msg;
        }
    }
}

void MVRecordingPreviewController::notifyFrameAvailable() {

    if (handler && !isInSwitchingCamera)
        handler->postMessage(new Message(MSG_RENDER_FRAME));
}

void MVRecordingPreviewController::updateTexMatrix(float *texMatrix) {
    //TODO:
//	memcpy(mTexMatrix, texMatrix, sizeof(float)*16);
}

int MVRecordingPreviewController::setBeautifyParam(int key, float value) {
	if (renderer) {
//		renderer->setBeautifyParam(key, value);
	}
}

void MVRecordingPreviewController::switchPreviewFilter(int filterType, byte *acvBuffer,
                                                       int bufferSize) {
    /*notify render thread that preview filter has changed*/
    if (-1 != bufferSize && NULL != acvBuffer) {
        mACVBuffer = new byte[bufferSize];
        memcpy(mACVBuffer, acvBuffer, bufferSize);
        mACVBufferSize = bufferSize;
    }
    this->filterType = filterType;
    if (handler)
        handler->postMessage(new Message(MSG_SWITCH_FILTER));
}

void MVRecordingPreviewController::startEncoding(int width, int height, int videoBitRate,
                                                 float frameRate, bool useHardWareEncoding,int strategy) {
    if (NULL != encoder) {
        delete encoder;
        encoder = NULL;
    }

    if (useHardWareEncoding) {
        encoder = new HWEncoderAdapter(g_jvm, obj);
    } else {
        encoder = new SoftEncoderAdapter(strategy);
    }

    encoder->init(width, height, videoBitRate, frameRate);
    if (handler)
        handler->postMessage(new Message(MSG_START_RECORDING));
}

void MVRecordingPreviewController::stopEncoding() {
    LOGI("stopEncoding");
    if (handler)
        handler->postMessage(new Message(MSG_STOP_RECORDING));
}

void MVRecordingPreviewController::adaptiveVideoQuality(int maxBitRate, int avgBitRate, int fps) {
    if (encoder) {
        encoder->reConfigure(maxBitRate, avgBitRate, fps);
    }
}

void MVRecordingPreviewController::switchCameraFacing() {
    LOGI("MVRecordingPreviewController::refereshCameraFacing");
    isInSwitchingCamera = true;
    /*notify render thread that camera has changed*/
    if (facingId == CAMERA_FACING_BACK) {
        facingId = CAMERA_FACING_FRONT;
    } else {
        facingId = CAMERA_FACING_BACK;
    }
    if (handler)
        handler->postMessage(new Message(MSG_SWITCH_CAMERA_FACING));
    LOGI("leave MVRecordingPreviewController::refereshCameraFacing");
}

void MVRecordingPreviewController::resetRenderSize(int screenWidth, int screenHeight) {
    LOGI("MVRecordingPreviewController::resetSize screenWidth:%d; screenHeight:%d", screenWidth,
         screenHeight);
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

void MVRecordingPreviewController::switchPauseRecordingPreviewState() {
    pauseFlag = true;
}

void MVRecordingPreviewController::switchCommonPreviewState() {
    continueFlag = true;
}

void MVRecordingPreviewController::destroyEGLContext() {
    LOGI("Stopping MVRecordingPreviewController thread");
    if (handler) {
        handler->postMessage(new Message(MSG_EGL_THREAD_EXIT));
        handler->postMessage(new Message(MESSAGE_QUEUE_LOOP_QUIT_FLAG));
    }
    pthread_join(_threadId, 0);
    if (queue) {
        queue->abort();
        delete queue;
        queue = NULL;
    }
    delete handler;
    handler = NULL;
    LOGI("MVRecordingPreviewController thread stopped");
}

void MVRecordingPreviewController::createWindowSurface(ANativeWindow *window) {
    LOGI("enter MVRecordingPreviewController::createWindowSurface");
    if (this->_window == NULL) {
        this->_window = window;
        if (handler)
            handler->postMessage(new Message(MSG_EGL_CREATE_PREVIEW_SURFACE));
    }
}

void MVRecordingPreviewController::destroyWindowSurface() {
    LOGI("enter MVRecordingPreviewController::destroyWindowSurface");
    if (handler)
        handler->postMessage(new Message(MSG_EGL_DESTROY_PREVIEW_SURFACE));
}

void MVRecordingPreviewController::buildRenderInstance() {
    renderer = new RecordingPreviewRenderer();
}

bool MVRecordingPreviewController::initialize() {
    const EGLint attribs[] = {EGL_BUFFER_SIZE, 32, EGL_ALPHA_SIZE, 8, EGL_BLUE_SIZE, 8,
                              EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_RENDERABLE_TYPE,
                              EGL_OPENGL_ES2_BIT,
                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE};
    LOGI("Initializing context");
    eglCore = new EGLCore();
    eglCore->init();
    this->createPreviewSurface();

	this->buildRenderInstance();
	this->configCamera();

	renderer->init(degress, facingId == CAMERA_FACING_FRONT, textureWidth, textureHeight, cameraWidth, cameraHeight);
	this->startCameraPreview();

    isInSwitchingCamera = false;
    pauseFlag = false;
    continueFlag = false;

    LOGI("leave Initializing context");
    return true;
}

void MVRecordingPreviewController::createPreviewSurface() {
    previewSurface = eglCore->createWindowSurface(_window);
    if (previewSurface != NULL) {
        eglCore->makeCurrent(previewSurface);
    }
}

void MVRecordingPreviewController::destroyPreviewSurface() {
    if (EGL_NO_SURFACE != previewSurface) {
        eglCore->releaseSurface(previewSurface);
        previewSurface = EGL_NO_SURFACE;
        if (_window) {
            LOGI("VideoOutput Releasing surfaceWindow");
            ANativeWindow_release(_window);
            _window = NULL;
        }
    }
}

void MVRecordingPreviewController::switchCamera() {
    this->releaseCamera();
    this->configCamera();
    renderer->setDegress(degress, facingId == CAMERA_FACING_FRONT);
    this->startCameraPreview();
    isInSwitchingCamera = false;
}

void MVRecordingPreviewController::destroy() {
    LOGI("enter MVRecordingPreviewController::destroy...");
    this->destroyPreviewSurface();
    if (renderer) {
        renderer->dealloc();
        delete renderer;
        renderer = NULL;
    }
    this->releaseCamera();
    eglCore->release();
    delete eglCore;
    eglCore = NULL;
    LOGI("leave MVRecordingPreviewController::destroy...");
}

void MVRecordingPreviewController::switchFilter() {
    renderer->setFilter(filterType, mACVBuffer, mACVBufferSize);
}

void MVRecordingPreviewController::startRecording() {
    encoder->createEncoder(eglCore, renderer->getOutputTexId());
    isEncoding = true;
}

void MVRecordingPreviewController::stopRecording() {
    LOGI("stopRecording....");
    isEncoding = false;
    if (encoder) {
        encoder->destroyEncoder();
        delete encoder;
        encoder = NULL;
    }
}

void MVRecordingPreviewController::renderFrame() {
    if (NULL != eglCore && !isInSwitchingCamera) {
//		long startTimeMills = getCurrentTime();
        if (startTime == -1) {
            startTime = getCurrentTime();
        }
        float position = ((float) (getCurrentTime() - startTime)) / 1000.0f;
        this->processVideoFrame(position);
        if (previewSurface != EGL_NO_SURFACE) {
            this->draw();
        }
//			LOGI("process Frame waste TimeMills 【%d】", (int)(getCurrentTime() - startTime));
        if (isEncoding) {
            encoder->encode();
        }
    }
}

void MVRecordingPreviewController::draw() {
    eglCore->makeCurrent(previewSurface);
    renderer->drawToViewWithAutofit(screenWidth, screenHeight, textureWidth, textureHeight);
    if (!eglCore->swapBuffers(previewSurface)) {
        LOGE("eglSwapBuffers(previewSurface) returned error %d", eglGetError());
    }
}

void MVRecordingPreviewController::processVideoFrame(float position) {
    updateTexImage();
    if (pauseFlag) {
        renderer->preparePausedState();
        pauseFlag = false;
    }
    if (continueFlag) {
        renderer->releasePausedState();
        continueFlag = false;
    }
    renderer->processFrame(position);
}

void MVRecordingPreviewController::updateTexImage() {
//	LOGI("MVRecordingPreviewController::updateTexImage");
    JNIEnv *env;
    if (g_jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }
    jclass jcls = env->GetObjectClass(obj);
    if (NULL != jcls) {
        jmethodID updateTexImageCallback = env->GetMethodID(jcls, "updateTexImageFromNative",
                                                            "()V");
        if (NULL != updateTexImageCallback) {
            env->CallVoidMethod(obj, updateTexImageCallback);
        }
    }
    if (g_jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
    }
//	LOGI("leave MVRecordingPreviewController::updateTexImage");
}

void MVRecordingPreviewController::configCamera() {
    LOGI("MVRecordingPreviewController::configCamera");
    JNIEnv *env;
    if (g_jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }
    jclass jcls = env->GetObjectClass(obj);
    if (NULL != jcls) {
        jmethodID configCameraCallback = env->GetMethodID(jcls, "configCameraFromNative",
                                                          "(I)Lcom/timeapp/shawn/recorder/pro/recording/camera/preview/CameraConfigInfo;");
        if (NULL != configCameraCallback) {
            jobject cameraConfigInfo = env->CallObjectMethod(obj, configCameraCallback, facingId);
            jclass cls_CameraConfigInfo = env->GetObjectClass(cameraConfigInfo);
            jmethodID cameraConfigInfo_getDegress = env->GetMethodID(cls_CameraConfigInfo,
                                                                     "getDegress", "()I");
            degress = env->CallIntMethod(cameraConfigInfo, cameraConfigInfo_getDegress);

            jmethodID cameraConfigInfo_getCameraFacingId = env->GetMethodID(cls_CameraConfigInfo,
                                                                            "getCameraFacingId",
                                                                            "()I");
            facingId = env->CallIntMethod(cameraConfigInfo, cameraConfigInfo_getCameraFacingId);

            jmethodID cameraConfigInfo_getTextureWidth = env->GetMethodID(cls_CameraConfigInfo,
                                                                          "getTextureWidth", "()I");
            int previewWidth = env->CallIntMethod(cameraConfigInfo,
                                                  cameraConfigInfo_getTextureWidth);
            jmethodID cameraConfigInfo_getTextureHeight = env->GetMethodID(cls_CameraConfigInfo,
                                                                           "getTextureHeight",
                                                                           "()I");
            int previewHeight = env->CallIntMethod(cameraConfigInfo,
                                                   cameraConfigInfo_getTextureHeight);

            this->cameraWidth = previewWidth;
            this->cameraHeight = previewHeight;

//			int previewMin = MIN(previewWidth, previewHeight);
//			textureWidth = previewMin >= 480 ? 480 : previewMin;
//			textureHeight = textureWidth;

            textureWidth = 360;
            textureHeight = 640;
//			textureWidth = 720;
//			textureHeight = 1280;
            LOGI("camera : {%d, %d}", previewWidth, previewHeight);
            LOGI("Texture : {%d, %d}", textureWidth, textureHeight);
        }
    }
    if (g_jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
        return;
    }
}

void MVRecordingPreviewController::releaseCamera() {
    LOGI("MVRecordingPreviewController::releaseCamera");
    JNIEnv *env;
    if (g_jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }
    jclass jcls = env->GetObjectClass(obj);
    if (NULL != jcls) {
        jmethodID releaseCameraCallback = env->GetMethodID(jcls, "releaseCameraFromNative", "()V");
        if (NULL != releaseCameraCallback) {
            env->CallVoidMethod(obj, releaseCameraCallback);
        }
    }
    if (g_jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
        return;
    }
}

void MVRecordingPreviewController::startCameraPreview() {
    LOGI("MVRecordingPreviewController::setCameraPreviewTexture");
    JNIEnv *env;
    if (g_jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }
    jclass jcls = env->GetObjectClass(obj);
    if (NULL != jcls) {
        jmethodID startPreviewCallback = env->GetMethodID(jcls, "startPreviewFromNative", "(I)V");
        if (NULL != startPreviewCallback) {
            env->CallVoidMethod(obj, startPreviewCallback, renderer->getCameraTexId());
        }
    }
    if (g_jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
        return;
    }
}

void MVRecordingPreviewController::hotConfigQuality(int maxBitrate, int avgBitrate, int fps) {
    if (encoder) {
        encoder->hotConfig(maxBitrate, avgBitrate, fps);
    }

}

void MVRecordingPreviewController::hotConfig(int bitRate, int fps, int gopSize) {
    LOGI("MVRecordingPreviewController::hotConfig");
    if (encoder) {
        encoder->hotConfig(bitRate, fps, gopSize);
    }
}


