package com.timeapp.shawn.recorder.pro.recorder;


import com.timeapp.shawn.recorder.pro.audioeffect.AudioEffect;

public class MockRecordProcessor implements RecordProcessor{

	@Override
	public void initAudioBufferSize(int audioSampleRate, int audioBufferSize, AudioEffect audioEffect) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void pushAudioBufferToQueue(short[] audioSamples,
			int audioSampleSize) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void flushAudioBufferToQueue() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void destroy() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setAudioEffect(AudioEffect audioEffect) {
		// TODO Auto-generated method stub
		
	}

}
