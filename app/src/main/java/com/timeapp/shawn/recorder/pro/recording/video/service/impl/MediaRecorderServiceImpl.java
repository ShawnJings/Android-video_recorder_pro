package com.timeapp.shawn.recorder.pro.recording.video.service.impl;

import android.content.res.AssetManager;

import com.timeapp.shawn.recorder.pro.audioeffect.AudioEffect;
import com.timeapp.shawn.recorder.pro.encoder.MediaCodecSurfaceEncoder;
import com.timeapp.shawn.recorder.pro.recording.camera.exception.CameraParamSettingException;
import com.timeapp.shawn.recorder.pro.recording.camera.preview.ChangbaRecordingPreviewScheduler;
import com.timeapp.shawn.recorder.pro.recording.camera.preview.PreviewFilterType;
import com.timeapp.shawn.recorder.pro.recording.exception.AudioConfigurationException;
import com.timeapp.shawn.recorder.pro.recording.exception.StartRecordingException;
import com.timeapp.shawn.recorder.pro.recording.service.RecorderService;
import com.timeapp.shawn.recorder.pro.recording.video.service.MediaRecorderService;


public class MediaRecorderServiceImpl implements MediaRecorderService {

	private RecorderService audioRecorderService;
	private ChangbaRecordingPreviewScheduler previewScheduler;

	public MediaRecorderServiceImpl(RecorderService recorderService, ChangbaRecordingPreviewScheduler scheduler) {
		this.audioRecorderService = recorderService;
		this.previewScheduler = scheduler;
	}

	public void switchCamera() throws CameraParamSettingException {
		previewScheduler.switchCameraFacing();
	}
	
	@Override
	public void initMetaData() throws AudioConfigurationException {
		audioRecorderService.initMetaData();
	}

	@Override
	public boolean initMediaRecorderProcessor(AudioEffect audioEffect) {
		return audioRecorderService.initAudioRecorderProcessor(audioEffect);
	}

	@Override
	public boolean start(int width, int height, int videoBitRate, int frameRate, boolean useHardWareEncoding, int strategy) throws StartRecordingException {
		audioRecorderService.start();
		if(useHardWareEncoding){
			if(MediaCodecSurfaceEncoder.IsInNotSupportedList()){
				useHardWareEncoding = false;
			}
		}
		previewScheduler.startEncoding(width, height, videoBitRate, frameRate, useHardWareEncoding, strategy);
		return useHardWareEncoding;
	}

	@Override
	public void destoryMediaRecorderProcessor() {
		audioRecorderService.destoryAudioRecorderProcessor();
	}

	@Override
	public void stop() {
		audioRecorderService.stop();
		previewScheduler.stop();
	}

	@Override
	public int getAudioBufferSize() {
		return audioRecorderService.getAudioBufferSize();
	}

	@Override
	public int getSampleRate() {
		return audioRecorderService.getSampleRate();
	}

	@Override
	public void switchPreviewFilter(AssetManager assetManager, PreviewFilterType filterType) {
		if(null != previewScheduler){
			previewScheduler.switchPreviewFilter(assetManager, filterType);;
		}
	}

	@Override
	public void enableUnAccom() {
		if (audioRecorderService != null) {
			audioRecorderService.enableUnAccom();
		}
	}

	@Override
	public void setAudioEffect(AudioEffect audioEffect) {
		if (audioRecorderService != null) {
			audioRecorderService.setAudioEffect(audioEffect);
		}
	}

}
