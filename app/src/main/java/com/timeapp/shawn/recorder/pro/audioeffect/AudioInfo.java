package com.timeapp.shawn.recorder.pro.audioeffect;

import java.io.Serializable;

public class AudioInfo implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1457193481087407247L;
	/** 声道数 **/
	int channels;
	/** 人声的采样频率 **/
	int audioSampleRate;
	/** 录制的时间 **/
	int recordedTimeMills;
	/** 歌曲总时间 **/
	int totalTimeMills;
	/** 伴奏的AGC增益 **/
	float accompanyAGCVolume;
	/** 人声的AGC增益 **/
	float audioAGCVolume;
	/** 电音 Mel文件路径 **/
	private String melFilePath;
	/** 人声的AGC增益 **/
	float accompanyPitch;
	/** Harmonic音效是否仅副歌 **/
	boolean isHarmonicOnlyChorus;
	/** Harmonic音效是否是FastMode **/
	boolean isHarmonicFastMode;
	/** 录音伴奏变调的级别[-3,3] **/
	int pitchShiftLevel;
	
	public AudioInfo(int channels, int audioSampleRate, int recordedTimeMills, int totalTimeMills,
			float accompanyAGCVolume, float audioAGCVolume, float accompanyPitch, String melFilePath,
			int pitchShiftLevel) {
		this.channels = channels;
		this.audioSampleRate = audioSampleRate;
		this.recordedTimeMills = recordedTimeMills;
		this.totalTimeMills = totalTimeMills;
		this.accompanyAGCVolume = accompanyAGCVolume;
		this.audioAGCVolume = audioAGCVolume;
		this.accompanyPitch = accompanyPitch;
		this.melFilePath = melFilePath;
		this.pitchShiftLevel = pitchShiftLevel;
	}

	public int getChannels() {
		return channels;
	}

	public int getAudioSampleRate() {
		return audioSampleRate;
	}

	public int getRecordedTimeMills() {
		return recordedTimeMills;
	}

	public int getTotalTimeMills() {
		return totalTimeMills;
	}

	public float getAccompanyAGCVolume() {
		return accompanyAGCVolume;
	}

	public float getAudioAGCVolume() {
		return audioAGCVolume;
	}

	public float getAccompanyPitch() {
		return accompanyPitch;
	}	
	public String getMelFilePath() {
		return melFilePath;
	}
	public void setMelFilePath(String melFilePath) {
		this.melFilePath = melFilePath;
	}
	
	public void setHarmonicOnlyChorus(boolean only) {
		isHarmonicOnlyChorus = only;
	}
	public boolean isHarmonicOnlyChorus() {
		return isHarmonicOnlyChorus;
	}
	
	public void setHarmonicFastMode(boolean fastMode) {
		isHarmonicFastMode = fastMode;
	}
	
	public boolean isHarmonicFastMode() {
		return isHarmonicFastMode;
	}
	
	public int getPitchShiftLevel() {
		return pitchShiftLevel;
	}

	public void setAccomanyPitch(float accompanyPitch, int pitchShiftLevel) {
		this.accompanyPitch = accompanyPitch;
		this.pitchShiftLevel = pitchShiftLevel;
	}
	
}
