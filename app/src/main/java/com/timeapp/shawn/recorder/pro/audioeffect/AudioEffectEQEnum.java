package com.timeapp.shawn.recorder.pro.audioeffect;

public enum AudioEffectEQEnum {
	BASS_5(0, "低沉5"), BASS_4(1, "低沉4"), BASS_3(2, "低沉3"), BASS_2(3, "低沉2"), BASS_1(
			4, "低沉1"), STANDARD(5, "标准"), TREBLE_1(6, "明亮1"), TREBLE_2(7, "明亮2"), TREBLE_3(
			8, "明亮3"), TREBLE_4(9, "明亮4"), TREBLE_5(10, "明亮5"),
	LIVE_STANDARD(0, "直播EQ");

	private int id;
	private String name;

	private AudioEffectEQEnum(int id, String name) {
		this.id = id;
		this.name = name;
	}

	public int getId() {
		return id;
	}

	public String getName() {
		return name;
	}

	public static AudioEffectEQEnum getEnum(int eqaulizerId) {
		switch (eqaulizerId) {
		case 0:
			return BASS_5;
		case 1:
			return BASS_4;
		case 2:
			return BASS_3;
		case 3:
			return BASS_2;
		case 4:
			return BASS_1;
		case 5:
			return STANDARD;
		case 6:
			return TREBLE_1;
		case 7:
			return TREBLE_2;
		case 8:
			return TREBLE_3;
		case 9:
			return TREBLE_4;
		case 10:
			return TREBLE_5;
		default:
			return STANDARD;
		}
	}
}
