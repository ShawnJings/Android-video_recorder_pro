#ifndef AUDIOEFFECT_VOLUME_VOCAL_DETECT_FILTER_H_
#define AUDIOEFFECT_VOLUME_VOCAL_DETECT_FILTER_H_

#include "volume_monitor.h"
#include "../vocal_detector/vocal_detect_effect_filter.h"

class VocalVolumeDetectEffectFilter: public AudioEffectFilter{
private:
	VolumeMonitor* volumeMonitor;
	int audioSampleRate;

	void updateVocalCurrentDB(double db);

public:
	VocalVolumeDetectEffectFilter();
	~VocalVolumeDetectEffectFilter();

	virtual int init(AudioEffect* audioEffect);
	void doFilter(AudioRequest* request, AudioResponse* response);
	virtual void destroy(AudioResponse* response);
};

#endif /* AUDIOEFFECT_VOLUME_VOCAL_DETECT_FILTER_H_ */
