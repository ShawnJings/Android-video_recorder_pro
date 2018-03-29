#ifndef AUDIOEFFECT_VOCAL_DETECT_FILTER_H_
#define AUDIOEFFECT_VOCAL_DETECT_FILTER_H_

#include "../../audio_effect_filter/audio_effect_filter.h"
#include "./vocal_detect.h"
#include "../../audio_effect_processor/audio_effect_context.h"
#include "../agc/vocal_agc_stat_effect_filter.h"

#define VOCAL_STATUS_LIST 	"vocal_status_list"
#define VOCAL_CURRENT_DB	"vocal_current_db" //人声音量检测
#define VOCAL_WAVE_WIDTH	"vocal_wave_width" //人声波形图宽度
#define VOCAL_WAVE			"vocal_wave" //人声波形图

static int VOCAL_DETECT_STATE_STATR=0;
static int VOCAL_DETECT_STATE_PAUSE=1;
static int VOCAL_DETECT_STATE_CONTINUE=2;
#define VOCAL_DETECT_STATE	"vocal_detect_state" //人声检测状态：开始，暂停，恢复
#define VOCAL_DETECT_CONTINUE_TIME	"vocal_detect_continue_time" //人声检测恢复时间

class VocalDetectEffectFilter: public AudioEffectFilter{
private:
	VocalDetect* vocalDetector;

	void updateVocalCurrentDB(double db);

	int vocalState;
	int vocalContinueTimeMills;
	int audioSampleRate;

public:
	VocalDetectEffectFilter();
	~VocalDetectEffectFilter();

	virtual int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	virtual void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_VOCAL_DETECT_FILTER_H_ */
