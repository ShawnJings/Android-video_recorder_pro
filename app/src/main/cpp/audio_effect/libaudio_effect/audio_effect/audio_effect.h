#ifndef BASE_AUDIOEFFECT_H_
#define BASE_AUDIOEFFECT_H_

#include "./sox_effect_param/sox_filter_chain_param.h"
#include <list>
#include "CommonTools.h"
#include "./audio_info.h"

class AudioEffect {
protected:
	AudioInfo* audioInfo;
	/** 这一些变量是用户手动可调节的 **/
	float accompanyVolume;
	float audioVolume;
	/** 下面这一些变量是从配置文件中读取出来的 **/
	SOXFilterChainParam* filterChainParam;
	std::list<int>* vocalEffectFilters;
	std::list<int>* accompanyEffectFilters;
	std::list<int>* mixPostEffectFilters;

	/** 从配置文件中读出，用来计算完成页面的人声伴奏音量 **/
	float outputGain;

	int type;
public:
	static float AccompanyDefault;
	static float AccompanyVolumeDefault;
	static float VocalVolumeDefault;

	AudioEffect();
	AudioEffect(AudioInfo* audioInfo, std::list<int>* vocalEffectFilters, std::list<int>* accompanyEffectFilters, std::list<int>* mixPostEffectFilters, float accompanyVolumeParam, float audioVolumeParam, SOXFilterChainParam* filterChainParam, float outputGainParam);
	~AudioEffect();
	float getAccompanyVolume() {
		return accompanyVolume;
	};
	float getAudioVolume() {
		return audioVolume;
	};
	AudioInfo* getAudioInfo(){
		return audioInfo;
	};
	SOXFilterChainParam* getFilterChainParam() {
		return filterChainParam;
	};

	std::list<int>* getVocalEffectFilters(){
		return vocalEffectFilters;
	};

	std::list<int>* getAccompanyEffectFilters(){
		return accompanyEffectFilters;
	};

	std::list<int>* getMixPostEffectFilters(){
		return mixPostEffectFilters;
	};
	void setType(int type){
		this->type = type;
	};
	int getType(){
		return this->type;
	};
	float getOutputGain() {
		return outputGain;
	}
};

#endif /* BASE_AUDIOEFFECT_H_ */
