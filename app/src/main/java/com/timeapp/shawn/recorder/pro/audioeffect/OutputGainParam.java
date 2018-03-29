package com.timeapp.shawn.recorder.pro.audioeffect;

/**
 * 
 * 注意:这个类千万不要动位置以及方法签名，并且一定不能混淆，
 * 在很多JNI类里面写死了这个类了
 * 
 */
public class OutputGainParam {
	private float gain;

	public OutputGainParam() {
	}

	public OutputGainParam(float gain) {
		this.gain = gain;
	}

	public OutputGainParam(OutputGainParam outputGainParam) {
		this(outputGainParam.gain);
	}

	public float getGain() {
		return gain;
	}

	@Override
	public String toString() {
		return "OutputGainParam [gain=" + gain + "]";
	}


	public static OutputGainParam buildDefaultOutputGainParam() {
		return new OutputGainParam(1.0f);
	}
}
