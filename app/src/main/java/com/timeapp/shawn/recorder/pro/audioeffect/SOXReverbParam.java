package com.timeapp.shawn.recorder.pro.audioeffect;

/**
 * 
 * 注意:这个类千万不要动位置以及方法签名，并且一定不能混淆，
 * 在很多JNI类里面写死了这个类了
 * 
 */
public class SOXReverbParam {
	private int reverberance;
	private int damping;
	private int roomScale;
	private int preDelay;
	private int wetGain;
	private int stereoDepth;
	private int dryWetMix;
	private int echoDelay;
	private int echoPredecay;
	private int echoPeriod;
	private int echoDecayRatio;
	private int echoPhaseDiff;
	private int echoNum;

	public SOXReverbParam() {
	}

	public SOXReverbParam(int reverberance, int damping, int roomScale,
			int preDelay, int wetGain, int stereoDepth, int dryWetMix,
			int echoDelay, int echoPredecay, int echoPeriod, int echoDecayRatio,
			int echoPhaseDiff, int echoNum) {
		super();
		this.reverberance = reverberance;
		this.damping = damping;
		this.roomScale = roomScale;
		this.preDelay = preDelay;
		this.wetGain = wetGain;
		this.stereoDepth = stereoDepth;
		this.dryWetMix = dryWetMix;
		this.echoDelay = echoDelay;
		this.echoPredecay = echoPredecay;
		this.echoPeriod = echoPeriod;
		this.echoDecayRatio = echoDecayRatio;
		this.echoPhaseDiff = echoPhaseDiff;
		this.echoNum = echoNum;
	}

	public SOXReverbParam(SOXReverbParam reverbParam) {
		this(reverbParam.getReverberance(), reverbParam.getDamping(),
				reverbParam.getRoomScale(), reverbParam.getPreDelay(),
				reverbParam.getWetGain(), reverbParam.getStereoDepth(),
				reverbParam.getDryWetMix(), reverbParam.getEchoDelay(),
				reverbParam.getEchoPredecay(), reverbParam.getEchoPeriod(), reverbParam.getEchoDecayRatio(),
				reverbParam.getEchoPhaseDiff(), reverbParam.getEchoNum());
	}

	public int getReverberance() {
		return reverberance;
	}

	public int getDamping() {
		return damping;
	}

	public int getRoomScale() {
		return roomScale;
	}

	public int getPreDelay() {
		return preDelay;
	}

	public int getWetGain() {
		return wetGain;
	}

	public int getStereoDepth() {
		return stereoDepth;
	}

	public int getDryWetMix() {
		return dryWetMix;
	}

	public int getEchoDelay() {
		return echoDelay;
	}

	public int getEchoPredecay() {
		return echoPredecay;
	}

	public int getEchoDecayRatio() {
		return echoDecayRatio;
	}

	public int getEchoPhaseDiff() {
		return echoPhaseDiff;
	}

	public int getEchoNum() {
		return echoNum;
	}

	@Override
	public String toString() {
		return "SOXReverbParam [reverberance=" + reverberance + ", damping="
				+ damping + ", roomScale=" + roomScale + ", preDelay="
				+ preDelay + ", wetGain=" + wetGain + ", stereoDepth="
				+ stereoDepth + ", dryWetMix=" + dryWetMix + ", echoDelay="
				+ echoDelay + ", echoPredecay=" + echoPredecay
				+ ", echoDecayRatio=" + echoDecayRatio + ", echoPhaseDiff="
				+ echoPhaseDiff + ", echoNum=" + echoNum + "]";
	}


	public static SOXReverbParam buildDefaultReverbParam() {
		return new SOXReverbParam(50, 50, 50, 50, 8, 80, 20, 20, 15, 30, 80, 20, 2);
	}


	public int getEchoPeriod() {
		return echoPeriod;
	}
}
