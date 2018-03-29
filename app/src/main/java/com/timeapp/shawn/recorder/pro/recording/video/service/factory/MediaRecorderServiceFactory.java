package com.timeapp.shawn.recorder.pro.recording.video.service.factory;


import com.timeapp.shawn.recorder.pro.recording.RecordingImplType;
import com.timeapp.shawn.recorder.pro.recording.camera.preview.ChangbaRecordingPreviewScheduler;
import com.timeapp.shawn.recorder.pro.recording.service.RecorderService;
import com.timeapp.shawn.recorder.pro.recording.service.impl.AudioRecordRecorderServiceImpl;
import com.timeapp.shawn.recorder.pro.recording.video.service.MediaRecorderService;
import com.timeapp.shawn.recorder.pro.recording.video.service.impl.MediaRecorderServiceImpl;

public class MediaRecorderServiceFactory {
	private static MediaRecorderServiceFactory instance = new MediaRecorderServiceFactory();
	private MediaRecorderServiceFactory() {}
	public static MediaRecorderServiceFactory getInstance() {
		return instance;
	}

	public MediaRecorderService getRecorderService(ChangbaRecordingPreviewScheduler scheduler, RecordingImplType recordingImplType) {
		RecorderService recorderService = getAudioRecorderService(recordingImplType);
		MediaRecorderService result = new MediaRecorderServiceImpl(recorderService, scheduler);
		return result;
	}

	protected RecorderService getAudioRecorderService(
			RecordingImplType recordingImplType) {
		RecorderService recorderService = null;
		switch (recordingImplType) {
		case ANDROID_PLATFORM:
			recorderService = AudioRecordRecorderServiceImpl.getInstance();
			break;
		default:
			break;
		}
		return recorderService;
	}

}
