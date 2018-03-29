package com.timeapp.shawn.recorder.pro.audioeffect;

/**
 * 
 * 注意:这个类千万不要动位置以及方法签名，并且一定不能混淆，
 * 在很多JNI类里面写死了这个类了
 * 
 */
public class SOXCompressorParam {
	public void setGain(int gain) {
		this.gain = gain;
	}

	@Override
	public String toString() {
		return "SOXCompressorParam [attacktime=" + attacktime + ", decaytime="
				+ decaytime + ", threshold=" + threshold + ", ratio=" + ratio
				+ ", expansion_threshold=" + expansion_threshold
				+ ", expansion_ratio=" + expansion_ratio + ", gain=" + gain
				+ "]";
	}

	private float attacktime;
	private float decaytime;
	private int threshold;
	private float ratio;
	private int expansion_threshold;
	private float expansion_ratio;
	private int gain;

	public SOXCompressorParam() {
	}

	public SOXCompressorParam(float attacktime, float decaytime, int threshold,
			float ratio, int expansion_threshold, float expansion_ratio,
			int gain) {
		super();
		this.attacktime = attacktime;
		this.decaytime = decaytime;
		this.threshold = threshold;
		this.ratio = ratio;
		this.expansion_threshold = expansion_threshold;
		this.expansion_ratio = expansion_ratio;
		this.gain = gain;
	}

	public SOXCompressorParam(SOXCompressorParam compressorParam) {
		this(compressorParam.getAttacktime(), compressorParam.getDecaytime(),
				compressorParam.getThreshold(), compressorParam.getRatio(),
				compressorParam.getExpansion_threshold(), compressorParam
						.getExpansion_ratio(), compressorParam.getGain());
	}

	public float getAttacktime() {
		return attacktime;
	}

	public float getDecaytime() {
		return decaytime;
	}

	public int getThreshold() {
		return threshold;
	}

	public float getRatio() {
		return ratio;
	}

	public int getGain() {
		return gain;
	}

	public int getExpansion_threshold() {
		return expansion_threshold;
	}

	public float getExpansion_ratio() {
		return expansion_ratio;
	}

	public static SOXCompressorParam buildDefaultParam() {
		return new SOXCompressorParam(0.02f, 0.04f, 20, 1.5f, 25, 1.3f, 0);
	}
}
