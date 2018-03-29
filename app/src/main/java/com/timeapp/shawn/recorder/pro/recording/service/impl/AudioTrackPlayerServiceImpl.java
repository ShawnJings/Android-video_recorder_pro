package com.timeapp.shawn.recorder.pro.recording.service.impl;

import android.media.AudioManager;

import com.timeapp.shawn.recorder.pro.accompany.NativeMp3Player;
import com.timeapp.shawn.recorder.pro.audioeffect.AudioEffect;
import com.timeapp.shawn.recorder.pro.recording.service.AbstractPlayerServiceImpl;
import com.timeapp.shawn.recorder.pro.recording.service.PlayerService;


public class AudioTrackPlayerServiceImpl extends AbstractPlayerServiceImpl
		implements PlayerService, PlayerService.OnCompletionListener {

	public NativeMp3Player mediaPlayer;
	
	public AudioTrackPlayerServiceImpl() {
		try {
			if (mediaPlayer == null) {
				mediaPlayer = new NativeMp3Player();
				mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
				mediaPlayer.setOnCompletionListener(this);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	@Override
	public boolean setAudioDataSource(int vocalSampleRate) {
		boolean result = false;
		if (mediaPlayer != null) {
			result = mediaPlayer.prepare(vocalSampleRate);
		}
		return result;
	}

	@Override
	public int getAccompanySampleRate() {
		try {
			if (mediaPlayer != null) {
				return mediaPlayer.getAccompanySampleRate();
			}
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
		return -1;
	}

	@Override
	public void start() {
		super.start();
		try {
			if (mediaPlayer != null) {
				mediaPlayer.start();
			}
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void stop() {
		super.stop();
		try {
			if (mediaPlayer != null) {
				mediaPlayer.stop();
			}
		} catch (IllegalStateException e) {
			e.printStackTrace();
		}
	}

	@Override
	public int getPlayerCurrentTimeMills() {
		return mediaPlayer.getCurrentTimeMills();
	}

	@Override
	public void onCompletion() {
		
	}
	
	@Override
	public void setVolume(float volume, float accompanyMax) {
		mediaPlayer.setVolume(volume, accompanyMax);
	}

	@Override
	public void startAccompany(String path) {
		mediaPlayer.startAccompany(path);
	}
	
	@Override
	public void pauseAccompany() {
		mediaPlayer.pauseAccompany();
	}

	@Override
	public void resumeAccompany() {
		mediaPlayer.resumeAccompany();
	}

	@Override
	public void stopAccompany() {
		mediaPlayer.stopAccompany();
	}

	@Override
	public int getPlayedAccompanyTimeMills() {
		return mediaPlayer.getPlayedAccompanyTimeMills();
	}
	
	@Override
	public void setAudioEffect(AudioEffect effect) {
		mediaPlayer.setAudioEffect(effect);
	}

	public boolean isPlayingAccompany(){
		boolean ret = false;
		if(null != mediaPlayer){
			ret = mediaPlayer.isPlayingAccompany();
		}
		return ret;
	}
}
