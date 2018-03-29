package com.timeapp.shawn.recorder.pro.audioeffect;

/**
 * 超级混响，调节echo,reverb
 * 
 * 注意:这个类千万不要动位置以及方法签名，并且一定不能混淆，
 * 在很多JNI类里面写死了这个类了
 * 
 */
public class AdjustEchoReverbParam {
	/** 超级混响音效echo,范围[0,1]**/
	private float adjustEchoEffectRatio;
	/** 超级混响音效reverb,范围[0,1]**/
	private float adjustReverbEffectRatio;

	public AdjustEchoReverbParam() {
	}
	
	public AdjustEchoReverbParam(float echoRatio, float reverbRatio) {
		adjustEchoEffectRatio = echoRatio;
		adjustReverbEffectRatio = reverbRatio;
	}

	public AdjustEchoReverbParam(AdjustEchoReverbParam param) {
		this(param.adjustEchoEffectRatio, param.adjustReverbEffectRatio);
	}
	
	public float getAdjustEchoEffectRatio() {
		return adjustEchoEffectRatio;
	}
	
	public void setAdjustEchoEffectRatio(float ratio) {
		adjustEchoEffectRatio = ratio;
	}
	
	public float getAdjustReverbEffectRatio() {
		return adjustReverbEffectRatio;
	}
	
	public void setAdjustReverbEffectRatio(float ratio) {
		adjustReverbEffectRatio = ratio;
	}
	
	public static AdjustEchoReverbParam buildDefaultAdjustEchoReverbParam() {
		return new AdjustEchoReverbParam(1.0f, 1.0f);
	}
	
	@Override
	public String toString() {
		return "AdjustEchoReverbParam : adjustEchoEffectRatio=" + adjustEchoEffectRatio 
				+ " ,adjustReverbEffectRatio="+adjustReverbEffectRatio;
	}
	
}
