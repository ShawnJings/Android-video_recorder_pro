package com.timeapp.shawn.recorder.pro.audioeffect;

import android.content.Context;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.timeapp.shawn.recorder.pro.util.DesEncode;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Map;

public class AudioEffectParamController {

	private static final AudioEffectParamController instance = new AudioEffectParamController();
	public final static String DES_KEY = "236ebd59848e95c80468ac4f6ebab136";

	private static Map<Integer, SongStyleEffectParam> params;

	private AudioEffectParamController() {
	}

	public static AudioEffectParamController getInstance() {
		return instance;
	}

	public AudioEffect extractParam(AudioEffectStyleEnum audioStyle,
			AudioEffectEQEnum equalizer) {
		return extractParam(audioStyle, equalizer, null);
	}
	
	public AudioEffect extractParam(AudioEffectStyleEnum audioStyle,
			AudioEffectEQEnum equalizer, File melFile) {
		if (audioStyle == AudioEffectStyleEnum.LENOVO_EFFECT){
			return new LenovoAudioEffect(AudioEffect.buildDeafultAudioEffect());
		}
		
		if (audioStyle == AudioEffectStyleEnum.BABY
				|| audioStyle == AudioEffectStyleEnum.CAT
				|| audioStyle == AudioEffectStyleEnum.DOG
				|| audioStyle == AudioEffectStyleEnum.GRAMOPHONE
				|| audioStyle == AudioEffectStyleEnum.LIVE_SIGNER
				|| audioStyle == AudioEffectStyleEnum.LIVE_PROFFESSION
				|| audioStyle == AudioEffectStyleEnum.LIVE_ORIGINAL
				|| audioStyle == AudioEffectStyleEnum.LIVE_MAGIC
				|| audioStyle == AudioEffectStyleEnum.LIVE_GOD
				|| audioStyle == AudioEffectStyleEnum.ROBOT
				|| audioStyle == AudioEffectStyleEnum.RECORDING_ACCOMPANY_PITCHSIFT
				|| audioStyle == AudioEffectStyleEnum.HARMONIC) {
			equalizer = AudioEffectEQEnum.LIVE_STANDARD;
		}
		if (null != params) {
			SongStyleEffectParam styleEffectParam = params.get(audioStyle
					.getId());
			if (null != styleEffectParam) {
				AudioEffect specailEffect = styleEffectParam
						.getSpecailEffectBySubId(equalizer.getId());
				if (null != specailEffect) {
					return new AudioEffect(specailEffect);
				} else {
					return AudioEffect.buildDeafultAudioEffect();
				}
			}
		}
		return AudioEffect.buildDeafultAudioEffect();
	}

	private String checkMelFilePath(File file) {
		if (file == null || !file.exists()) {
			return "";
		}
		return file.getAbsolutePath();
	}
	
	public void loadParamFromResource(Context context) {
		BufferedReader reader = null;
		InputStream in = null;
		try {
			in = context.getAssets().open("align_22050.wav");
			reader = new BufferedReader(new InputStreamReader(in));
			StringBuilder contentBuilder = new StringBuilder();
			String line = null;
			try {
				while ((line = (reader.readLine())) != null) {
					contentBuilder.append(line);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			String content = contentBuilder.toString();
			String json;
			try {
				json = DesEncode.decode(AudioEffectParamController.DES_KEY,
						content);
				Gson gson = new Gson();
				AudioEffectParamController.params = gson.fromJson(json,
						new TypeToken<Map<Integer, SongStyleEffectParam>>() {
						}.getType());
				Log.i("songstudio", "init songstudio param success...");
			} catch (Exception e) {
				e.printStackTrace();
			}
		} catch (IOException e1) {
			e1.printStackTrace();
		} catch (Throwable t) {
			t.printStackTrace();
		} finally {
			if (null != reader) {
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

}
