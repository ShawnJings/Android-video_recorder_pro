package com.timeapp.shawn.recorder.pro.audioeffect;

public enum AudioEffectType {
	BASE_AUDIO_EFFECT_TYPE(0, "原声|留声机|迷幻|流行|摇滚|舞曲|新世纪|R&B|LIVE_XXX"),
	AUTO_TUNE_EFFECT_TYPE(1, "电音"),
	ROBOT_EFFECT_TYPE(2, "机器人"),
	LENOVO_EFFECT_TYPE(3, "联想乐蒙特效"),
	HARMONIC_EFFECT_TYPE(4, "和声音效");
	
	private int id;
	private String name;
	
	AudioEffectType(int id, String name){
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
