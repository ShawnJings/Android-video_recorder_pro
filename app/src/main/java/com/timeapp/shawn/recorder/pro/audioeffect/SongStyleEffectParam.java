package com.timeapp.shawn.recorder.pro.audioeffect;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;


public class SongStyleEffectParam {
	private int songStyleId;
	private String name;
	private Map<Integer, AudioEffect> subParams = new HashMap<Integer, AudioEffect>();
	public SongStyleEffectParam() {
	}
	public SongStyleEffectParam(int songStyleId){
		this.songStyleId = songStyleId;
		switch (songStyleId) {
		case 0:
			this.name = "R&B";
			break;
		case 1:
			this.name = "摇滚";
			break;
		case 2:
			this.name = "流行";
			break;
		case 3:
			this.name = "舞曲";
			break;
		case 4:
			this.name = "新世纪";
			break;
		case 5:
			this.name = "原声";
			break;
		case 6:
			this.name = "留声机";
			break;
		case 7:
			this.name = "萌猫";
			break;
		case 8:
			this.name = "狗宝宝";
			break;
		case 9:
			this.name = "娃娃音";
			break;
		default:
			break;
		}
	}
	public String getName() {
		return name;
	}
	public int getSongStyleId(){
		return songStyleId;
	}
	public AudioEffect getSpecailEffectBySubId(int subId){
		return subParams.get(subId);
	}
	
	public void addSpecailEffectWithSubId(int subId, AudioEffect effect){
		subParams.put(subId, effect);
	}
	public Set<Integer> getSubIds(){
		return subParams.keySet();
	}
}
