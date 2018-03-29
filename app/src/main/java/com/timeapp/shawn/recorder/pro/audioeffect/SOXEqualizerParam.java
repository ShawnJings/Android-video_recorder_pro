package com.timeapp.shawn.recorder.pro.audioeffect;

/**
 * 
 * 注意:这个类千万不要动位置以及方法签名，并且一定不能混淆，
 * 在很多JNI类里面写死了这个类了
 * 
 */
public class SOXEqualizerParam {
	private boolean enable1;
	private int frequency1;
	private float bandWidth1;
	private float gain1;
	private boolean enable2;
	private int frequency2;
	private float bandWidth2;
	private float gain2;
	private boolean enable3;
	private int frequency3;
	private float bandWidth3;
	private float gain3;
	private boolean enable4;
	private int frequency4;
	private float bandWidth4;
	private float gain4;
	private boolean lowpassEnabled;
	private int lowpassFrequency;
	private float lowpassBandWidth;
	private boolean highpassEnabled;
	private int highpassFrequency;
	private float highpassBandWidth;

	public SOXEqualizerParam() {
		this.enable1 = false;
		this.enable2 = false;
		this.enable3 = false;
		this.enable4 = false;
		this.lowpassEnabled = false;
		this.highpassEnabled = false;
	}

	public SOXEqualizerParam(boolean enable1, int frequency1, float bandWidth1,
			float gain1, boolean enable2, int frequency2, float bandWidth2,
			float gain2, boolean enable3, int frequency3, float bandWidth3,
			float gain3, boolean enable4, int frequency4, float bandWidth4,
			float gain4, boolean lowpassEnabled, int lowpassFrequency,
			float lowpassBandWidth, boolean highpassEnabled,
			int highpassFrequency, float highpassBandWidth) {
		super();
		// equalizer
		this.enable1 = enable1;
		this.frequency1 = frequency1;
		this.bandWidth1 = bandWidth1;
		this.gain1 = gain1;
		this.enable2 = enable2;
		this.frequency2 = frequency2;
		this.bandWidth2 = bandWidth2;
		this.gain2 = gain2;
		this.enable3 = enable3;
		this.frequency3 = frequency3;
		this.bandWidth3 = bandWidth3;
		this.gain3 = gain3;
		this.enable4 = enable4;
		this.frequency4 = frequency4;
		this.bandWidth4 = bandWidth4;
		this.gain4 = gain4;
		// 低通
		this.lowpassEnabled = lowpassEnabled;
		this.lowpassFrequency = lowpassFrequency;
		this.lowpassBandWidth = lowpassBandWidth;
		// 高通
		this.highpassEnabled = highpassEnabled;
		this.highpassFrequency = highpassFrequency;
		this.highpassBandWidth = highpassBandWidth;
	}

	public SOXEqualizerParam(SOXEqualizerParam equalizerParam) {
		this(equalizerParam.isEnable1(), equalizerParam.getFrequency1(), equalizerParam.getBandWidth1(), equalizerParam.getGain1(),
				equalizerParam.isEnable2(), equalizerParam.getFrequency2(), equalizerParam.getBandWidth2(), equalizerParam.getGain2(),
				equalizerParam.isEnable3(), equalizerParam.getFrequency3(), equalizerParam.getBandWidth3(), equalizerParam.getGain3(),
				equalizerParam.isEnable4(), equalizerParam.getFrequency4(), equalizerParam.getBandWidth4(), equalizerParam.getGain4(),
				equalizerParam.isLowpassEnabled(), equalizerParam.getLowpassFrequency(), equalizerParam.getLowpassBandWidth(),
				equalizerParam.isHighpassEnabled(), equalizerParam.getHighpassFrequency(), equalizerParam.getHighpassBandWidth());
	}

	public boolean isEnable1() {
		return enable1;
	}

	public int getFrequency1() {
		return frequency1;
	}

	public float getBandWidth1() {
		return bandWidth1;
	}

	public float getGain1() {
		return gain1;
	}

	public boolean isEnable2() {
		return enable2;
	}

	public int getFrequency2() {
		return frequency2;
	}

	public float getBandWidth2() {
		return bandWidth2;
	}

	public float getGain2() {
		return gain2;
	}

	public boolean isEnable3() {
		return enable3;
	}

	public int getFrequency3() {
		return frequency3;
	}

	public float getBandWidth3() {
		return bandWidth3;
	}

	public float getGain3() {
		return gain3;
	}

	public boolean isEnable4() {
		return enable4;
	}

	public int getFrequency4() {
		return frequency4;
	}

	public float getBandWidth4() {
		return bandWidth4;
	}

	public float getGain4() {
		return gain4;
	}
	public boolean isLowpassEnabled() {
		return lowpassEnabled;
	}

	public int getLowpassFrequency() {
		return lowpassFrequency;
	}

	public float getLowpassBandWidth() {
		return lowpassBandWidth;
	}

	public boolean isHighpassEnabled() {
		return highpassEnabled;
	}

	public int getHighpassFrequency() {
		return highpassFrequency;
	}

	public float getHighpassBandWidth() {
		return highpassBandWidth;
	}

	@Override
	public String toString() {
		return "SOXEqualizerParam [enable1=" + enable1 + ", frequency1="
				+ frequency1 + ", bandWidth1=" + bandWidth1 + ", gain1="
				+ gain1 + ", enable2=" + enable2 + ", frequency2=" + frequency2
				+ ", bandWidth2=" + bandWidth2 + ", gain2=" + gain2
				+ ", enable3=" + enable3 + ", frequency3=" + frequency3
				+ ", bandWidth3=" + bandWidth3 + ", gain3=" + gain3
				+ ", enable4=" + enable4 + ", frequency4=" + frequency4
				+ ", bandWidth4=" + bandWidth4 + ", gain4=" + gain4
				+ ", lowpassEnabled=" + lowpassEnabled + ", lowpassFrequency="
				+ lowpassFrequency + ", lowpassBandWidth=" + lowpassBandWidth
				+ ", highpassEnabled=" + highpassEnabled
				+ ", highpassFrequency=" + highpassFrequency
				+ ", highpassBandWidth=" + highpassBandWidth + "]";
	}

	public void setEQFilter(int frequency, float bandWidth, float gain) {
		this.enable4 = false;
		if(!this.enable1){
			this.enable1 = true;
			this.frequency1 = frequency;
			this.bandWidth1 = bandWidth;
			this.gain1 = gain;
		} else if(!this.enable2){
			this.enable2 = true;
			this.frequency2 = frequency;
			this.bandWidth2 = bandWidth;
			this.gain2 = gain;
		} else if(!this.enable3){
			this.enable3 = true;
			this.frequency3 = frequency;
			this.bandWidth3 = bandWidth;
			this.gain3 = gain;
		} else if(!this.enable4){
			this.enable4 = true;
			this.frequency4 = frequency;
			this.bandWidth4 = bandWidth;
			this.gain4 = gain;
		}
	}

	public void setLowPassFilter(int frequency, float bandWidth) {
		this.lowpassEnabled = true;
		this.lowpassFrequency = frequency;
		this.lowpassBandWidth = bandWidth;
	}

	public void setHighPassFilter(int frequency, float bandWidth) {
		this.highpassEnabled = true;
		this.highpassFrequency = frequency;
		this.highpassBandWidth = bandWidth;
	}

	public static SOXEqualizerParam buildDefaultParam() {
		return new SOXEqualizerParam(false, 500, 1.0f, 0, 
				false, 500, 1.0f, 0, 
				false, 500, 1.0f, 0, 
				false, 500, 1.0f, 0, 
				false, 500, 1.0f, 
				false, 500, 1.0f);
	}

}
