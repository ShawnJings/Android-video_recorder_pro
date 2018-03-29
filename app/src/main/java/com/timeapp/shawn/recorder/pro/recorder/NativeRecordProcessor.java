package com.timeapp.shawn.recorder.pro.recorder;


import com.timeapp.shawn.recorder.pro.audioeffect.AudioEffect;

public class NativeRecordProcessor implements RecordProcessor {
	private int handle;

	@Override
	public void initAudioBufferSize(int audioSampleRate, int audioBufferSize, AudioEffect audioEffect) {
		handle = init(audioSampleRate, audioBufferSize, audioEffect);
	}

	@Override
	public void pushAudioBufferToQueue(short[] audioSamples,
			int audioSampleSize) {
		pushAudioBufferToQueue(handle, audioSamples, audioSampleSize);
	}

	@Override
	public void flushAudioBufferToQueue() {
		flushAudioBufferToQueue(handle);
	}

	private native int init(int audioSampleRate, int audioBufferSize, AudioEffect audioEffect);

	@Override
	public void setAudioEffect(AudioEffect audioEffect){
		setAudioEffect(handle, audioEffect);
	}
	public native void setAudioEffect(int handle, AudioEffect audioEffect);
	
	private native void flushAudioBufferToQueue(int handle);
	private native void destroy(int handle);

	private native int pushAudioBufferToQueue(int handle, short[] audioSamples,
			int audioSampleSize);

	@Override
	public void destroy() {
		destroy(handle);
	}

}
