#ifndef AUDIOEFFECT_AUDIO_REQUEST_H_
#define AUDIOEFFECT_AUDIO_REQUEST_H_

#include "CommonTools.h"
#include <map>
#include <string>

class AudioRequest {
private:
	short *vocalBuffer;
	int vocalBufferSize;
	int accompanyBufferSize;
	short *accompanyBuffer;
	/** unit is TimeMills **/
	float position;
	/**
	 * the vocal buffer's first sample in the whole PCM file's index
	 *
	 * |____________9_____....____________________________|    PCM File
	 *             |x___|									   Vocal Buffer
	 *  x should be assigned to 9
	 * **/
	long frameNum;

	std::map<std::string, void*>* extraData;

public:
	AudioRequest();
	~AudioRequest();

	void init(short *vocalBuffer, int vocalBufferSize, float position, long frameNum){
		this->vocalBuffer = vocalBuffer;
		this->vocalBufferSize = vocalBufferSize;
		this->position = position;
		this->frameNum = frameNum;
	}

	void init(short *vocalBuffer, int vocalBufferSize, short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum){
		this->vocalBuffer = vocalBuffer;
		this->accompanyBuffer = accompanyBuffer;
		this->vocalBufferSize = vocalBufferSize;
		this->accompanyBufferSize = accompanyBufferSize;
		this->position = position;
		this->frameNum = frameNum;
	}

	void initAccompany(short *accompanyBuffer, int accompanyBufferSize, float position, long frameNum){
		this->accompanyBuffer = accompanyBuffer;
		this->accompanyBufferSize = accompanyBufferSize;
		this->position = position;
		this->frameNum = frameNum;
	}

	long getFrameNum(){
		return frameNum;
	}

	short * getVocalBuffer() {
		return vocalBuffer;
	}
	short * getAccompanyBuffer() {
		return accompanyBuffer;
	}
	int getVocalBufferSize() {
		return vocalBufferSize;
	}
	void setVocalBufferSize(int vocalBufferSize) {
		this->vocalBufferSize = vocalBufferSize;
	}
	int getAccompanyBufferSize() {
		return accompanyBufferSize;
	}
	void setAccompanyBufferSize(int accompanyBufferSize) {
		this->accompanyBufferSize = accompanyBufferSize;
	}
	/** unit is TimeMills **/
	float getPosition() {
		return position;
	}

	void clear() {
//		std::map<std::string, void*>::iterator itr = extraData->begin();
//		for (; itr != extraData->end(); ++itr) {
//			void* value = itr->second;
//			delete value;
//		}
		extraData->clear();
	}

	void put(std::string key, void* value) {
		extraData->insert(std::pair<std::string, void*>(key, value));
	}

	void* get(std::string key) {
		std::map<std::string, void*>::iterator itr = extraData->find(key);
		if (itr != extraData->end()) {
			return itr->second;
		}
		return NULL;
	}
};

#endif /* AUDIOEFFECT_AUDIO_REQUEST_H_ */
