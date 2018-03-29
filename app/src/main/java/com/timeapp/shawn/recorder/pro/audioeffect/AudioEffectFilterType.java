package com.timeapp.shawn.recorder.pro.audioeffect;

public enum AudioEffectFilterType {

	VocalAGCEffectFilter(0, "人声自动音量控制"),
    AccompanyAGCEffectFilter(1, "伴奏自动音量控制"),
    CompressorFilter(2, "压缩效果器"),
    EqualizerFilter(3, "均衡效果器"),
	ReverbEchoFilter(4, "混响+Echo效果器"),
	Mono2StereoFilter(5, "单声道转双声道效果器"),
	AutoTuneEffectFilter(6, "电音效果器"),
	DoubleUEffectFilter(7, "迷幻效果器"),
	HarmonicEffectFilter(8, "唱诗班效果器"),
	VocalPitchShiftFilter(9, "人声升降调效果器"),
	AccompanyPitchShiftFilter(10, "伴奏升降调效果器"),
	VocalVolumeAdjustFilter(11, "人声音量增益效果器"),
	AccompanyVolumeAdjustFilter(12, "伴奏音量增益效果器"),
	FadeOutEffectFilter(13, "淡出效果器"),
	VocalAGCStatEffectFilterType(14, "人声自动音量控制的统计Filter"),
	AccompanyAGCStatEffectFilterType(15, "伴奏自动音量控制的统计Filter"),
	VocalDetectEffectFilterType(16, "人声段落检测的统计Filter"),
	AccompanyDelayOutputEffectFilterType(17, "为了电音做的伴奏的延迟输出效果器");
	
	private int id;
	private String name;
	
	AudioEffectFilterType(int id, String name){
		this.id = id;
		this.name = name;
	}

	public Integer getId() {
		return id;
	}	
	
	public String getName() {
		return name;
	}

}
