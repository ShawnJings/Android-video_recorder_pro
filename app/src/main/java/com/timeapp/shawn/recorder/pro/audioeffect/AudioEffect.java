package com.timeapp.shawn.recorder.pro.audioeffect;

import java.io.Serializable;
import java.util.ArrayList;

/**
 * 
 * 注意:这个类千万不要动位置以及方法签名，并且一定不能混淆， 在很多JNI类里面写死了这个类了
 * 
 */
public class AudioEffect implements Serializable {
	private static final long serialVersionUID = -1108248334432067097L;
	/** 音效类型 **/
	private int songStyleId;
	private String songStyleName;
	/** EQ级别 **/
	private int subId;
	private String subName;

	/** 新增属性 **/
	private int type;
	/** 新增属性 **/
	private AudioInfo audioInfo;
	/** 机器人 变声（升调还是降调） **/
	private float vocalPitch;
	/** 伴奏音量 **/
	private float accompanyVolume;
	/** 人生音量 **/
	private float audioVolume;
	/** reverb的参数 **/
	private SOXReverbParam reverParam;
	/** equalizer的参数 **/
	private SOXEqualizerParam equalizerParam;
	/** compressor的参数 **/
	private SOXCompressorParam compressorParam;
	/** output gain的参数 **/
	private OutputGainParam outputGainParam;
	/** 超级混响的参数 **/
	private AdjustEchoReverbParam adjustEchoReverbParam;
	/** filter_chain的顺序 **/
	private ArrayList<Integer> vocalEffectFilterChain = new ArrayList<Integer>();
	private ArrayList<Integer> accompanyEffectFilterChain = new ArrayList<Integer>();
	private ArrayList<Integer> mixPostEffectFilterChain = new ArrayList<Integer>();

	public AudioEffect() {
	}

	public AudioEffect(float pitch, float accompanyVolume,
                       float audioVolume, int songStyleId, int subId,
                       ArrayList<Integer> vocalEffectFilterChain, ArrayList<Integer> accompanyEffectFilterChain,
                       ArrayList<Integer> mixPostEffectFilterChain) {
		this.vocalPitch = pitch;
		this.accompanyVolume = accompanyVolume;
		this.audioVolume = audioVolume;
		this.songStyleId = songStyleId;
		this.songStyleName = AudioEffectStyleEnum.getEnum(songStyleId).getName();
		this.subId = subId;
		this.subName = AudioEffectEQEnum.getEnum(subId).getName();
		this.vocalEffectFilterChain = vocalEffectFilterChain;
		this.accompanyEffectFilterChain = accompanyEffectFilterChain;
		this.mixPostEffectFilterChain = mixPostEffectFilterChain;
		this.outputGainParam = OutputGainParam.buildDefaultOutputGainParam();
		this.adjustEchoReverbParam = AdjustEchoReverbParam.buildDefaultAdjustEchoReverbParam();
	}

	public AudioEffect(AudioEffect specailEffect) {
		this(specailEffect.getVocalPitch(),
				specailEffect.getAccompanyVolume(), specailEffect.getAudioVolume(), specailEffect.getSongStyleId(),
				specailEffect.getSubId(), specailEffect.getVocalEffectFilterChain(),
				specailEffect.getAccompanyEffectFilterChain(), specailEffect.getMixPostEffectFilterChain());
		this.type = specailEffect.getType();
		this.audioInfo = specailEffect.getAudioInfo();
		this.reverParam = new SOXReverbParam(specailEffect.getReverbParam());
		this.equalizerParam = new SOXEqualizerParam(specailEffect.getEqualizerParam());
		this.compressorParam = new SOXCompressorParam(specailEffect.getCompressorParam());
		if (specailEffect.getOutputGainParam() != null) {
			this.outputGainParam = new OutputGainParam(specailEffect.getOutputGainParam());
		}
		if (specailEffect.getAdjustEchoReverbParam() != null) {
			this.adjustEchoReverbParam = new AdjustEchoReverbParam(specailEffect.getAdjustEchoReverbParam());
		}
	}

	public void setVocalPitch(float vocalPitch) {
		this.vocalPitch = vocalPitch;
	}
	
	public float getVocalPitch() {
		return vocalPitch;
	}

	public float getAccompanyVolume() {
		return accompanyVolume;
	}

	public void setAccompanyVolume(float accompanyVolume) {
		this.accompanyVolume = accompanyVolume;
	}

	public float getAudioVolume() {
		return audioVolume;
	}

	public void setAudioVolume(float audioVolume) {
		this.audioVolume = audioVolume;
	}

	public void setReverbParam(SOXReverbParam reverParam) {
		this.reverParam = reverParam;
	}

	public void setEqualizerParam(SOXEqualizerParam equalizerParam) {
		this.equalizerParam = equalizerParam;
	}

	public void setCompressorParam(SOXCompressorParam compressorParam) {
		this.compressorParam = compressorParam;
	}

	public SOXReverbParam getReverbParam() {
		return reverParam;
	}

	public SOXEqualizerParam getEqualizerParam() {
		return equalizerParam;
	}

	public SOXCompressorParam getCompressorParam() {
		return compressorParam;
	}

	public OutputGainParam getOutputGainParam() {
		return outputGainParam;
	}
	
	public void setOutputGainParam(OutputGainParam outputGainParam) {
		this.outputGainParam = outputGainParam;
	}
	
	public int getSongStyleId() {
		return songStyleId;
	}

	public String getSongStyleName() {
		return songStyleName;
	}

	public int getSubId() {
		return subId;
	}

	public String getSubName() {
		return subName;
	}

	public static AudioEffect buildDeafultAudioEffect() {
		AudioEffect audioEffect = new AudioEffect(1.0f, 1.0f, 1.0f, 3, 5, new ArrayList<Integer>(), new ArrayList<Integer>(), new ArrayList<Integer>());
		audioEffect.setReverbParam(SOXReverbParam.buildDefaultReverbParam());
		audioEffect.setCompressorParam(SOXCompressorParam.buildDefaultParam());
		audioEffect.setEqualizerParam(SOXEqualizerParam.buildDefaultParam());
		// 人声的-Filter chain
		ArrayList<Integer> vocalEffectFilterChain = new ArrayList<Integer>();
		vocalEffectFilterChain.add(AudioEffectFilterType.VocalAGCEffectFilter.getId());
		vocalEffectFilterChain.add(AudioEffectFilterType.CompressorFilter.getId());
		vocalEffectFilterChain.add(AudioEffectFilterType.EqualizerFilter.getId());
		vocalEffectFilterChain.add(AudioEffectFilterType.ReverbEchoFilter.getId());
		vocalEffectFilterChain.add(AudioEffectFilterType.VocalVolumeAdjustFilter.getId());
		audioEffect.setVocalEffectFilterChain(vocalEffectFilterChain);
		// 伴奏的-Filter chain
		ArrayList<Integer> accompanyEffectFilterChain = new ArrayList<Integer>();
		accompanyEffectFilterChain.add(AudioEffectFilterType.AccompanyAGCEffectFilter.getId());
		accompanyEffectFilterChain.add(AudioEffectFilterType.AccompanyVolumeAdjustFilter.getId());
		audioEffect.setAccompanyEffectFilterChain(accompanyEffectFilterChain);
		// mix之后的-Filter chain
		ArrayList<Integer> mixPostEffectFilterChain = new ArrayList<Integer>();
		mixPostEffectFilterChain.add(AudioEffectFilterType.FadeOutEffectFilter.getId());
		audioEffect.setMixPostEffectFilterChain(mixPostEffectFilterChain);
		return audioEffect;
	}

	public void setVocalEffectFilterChain(ArrayList<Integer> vocalEffectFilterChain) {
		this.vocalEffectFilterChain = vocalEffectFilterChain;
	}

	public void setAccompanyEffectFilterChain(ArrayList<Integer> accompanyEffectFilterChain) {
		this.accompanyEffectFilterChain = accompanyEffectFilterChain;
	}

	public void setMixPostEffectFilterChain(ArrayList<Integer> mixPostEffectFilterChain) {
		this.mixPostEffectFilterChain = mixPostEffectFilterChain;
	}

	public ArrayList<Integer> getVocalEffectFilterChain() {
		return vocalEffectFilterChain;
	}

	public ArrayList<Integer> getAccompanyEffectFilterChain() {
		return accompanyEffectFilterChain;
	}

	public ArrayList<Integer> getMixPostEffectFilterChain() {
		return mixPostEffectFilterChain;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public AudioInfo getAudioInfo() {
		return audioInfo;
	}

	public void setAudioInfo(AudioInfo audioInfo) {
		this.audioInfo = audioInfo;
	}
	
	public AdjustEchoReverbParam getAdjustEchoReverbParam() {
		return adjustEchoReverbParam;
	}
	
	public float getAccompanyDefault() { //7.0 gt设置完成页面音量参数
		return 1.0f;
	}
	
	public float getAccompanyVolumeDefault() { //7.0 gt设置完成页面音量参数
		return 0.95f;
	}
	
	public float getVocalVolumeDefault() { //7.0 gt设置完成页面音量参数
		return 0.95f;
	}
	
}
