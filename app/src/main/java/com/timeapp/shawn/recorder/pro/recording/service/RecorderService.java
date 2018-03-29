package com.timeapp.shawn.recorder.pro.recording.service;


import com.timeapp.shawn.recorder.pro.audioeffect.AudioEffect;
import com.timeapp.shawn.recorder.pro.recording.exception.AudioConfigurationException;
import com.timeapp.shawn.recorder.pro.recording.exception.StartRecordingException;

public interface RecorderService {

	/**
	 * 初始化录音器的硬件部分
	 */
	public void initMetaData()throws AudioConfigurationException;

	/**
	 * 初始化我们后台处理数据部分
	 */
	public boolean initAudioRecorderProcessor(AudioEffect audioEffect);
	/**
	 * 销毁我们的后台处理数据部分
	 */
	public void destoryAudioRecorderProcessor();
	/**
	 * 获得后台处理数据部分的buffer大小
	 */
	public int getAudioBufferSize();

	/**
	 * 开始录音
	 */
	public void start() throws StartRecordingException;

	/**
	 * 结束录音
	 */
	public void stop();

	public int getSampleRate();

	/** 清唱的时候获取当前音高 来渲染音量动画 **/
	public int getRecordVolume();
	public void enableUnAccom();
	
	public void pause();
	
	public void continueRecord();
	
	public boolean isPaused();

	public void setAudioEffect(AudioEffect audioEffect);
	
}
