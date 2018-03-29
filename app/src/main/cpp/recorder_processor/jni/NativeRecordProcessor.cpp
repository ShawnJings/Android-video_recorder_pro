#include "com_timeapp_shawn_recorder_pro_recorder_NativeRecordProcessor.h"

#define LOG_TAG "NativeRecordProcessor"
/** static WaveFileHandle array **/
static RecordProcessor* handles[100];
/**
 * Seeks a free handle in the handles array and returns its index or -1 if no handle could be found
 */
static int findFreeHandle() {
	for (int i = 0; i < 100; i++) {
		if (handles[i] == 0)
			return i;
	}
	return -1;
}

JNIEXPORT jint JNICALL Java_com_timeapp_shawn_recorder_pro_recorder_NativeRecordProcessor_init(JNIEnv * env, jobject obj,
		jint sampleRate, jint audioBufferSize, jobject audioEffect) {
	int index = findFreeHandle();
	if (index == -1)
		return -1;
	RecordProcessor* recorder = new RecordProcessor();
	AudioEffect* cbAudioEffect = AudioEffectAdapter::GetInstance()->buildAudioEffect(audioEffect, env);
	recorder->initAudioBufferSize(sampleRate, audioBufferSize, cbAudioEffect);
	handles[index] = recorder;
	return index;
}

JNIEXPORT void JNICALL Java_com_timeapp_shawn_recorder_pro_recorder_NativeRecordProcessor_setAudioEffect(JNIEnv * env, jobject obj, jint index, jobject audioEffect){
	if (handles[index] != 0) {
		RecordProcessor* recorder = handles[index];
		AudioEffect* cbAudioEffect = AudioEffectAdapter::GetInstance()->buildAudioEffect(audioEffect, env);
		recorder->setAudioEffect(cbAudioEffect);
	}
}

JNIEXPORT jint JNICALL Java_com_timeapp_shawn_recorder_pro_recorder_NativeRecordProcessor_pushAudioBufferToQueue(JNIEnv * env, jobject obj, jint index, jshortArray audioSamples,
		jint audioSampleSize) {
	if (handles[index] != 0) {
		RecordProcessor* recorder = handles[index];
		jshort* samples = env->GetShortArrayElements(audioSamples, 0);
		int result = recorder->pushAudioBufferToQueue(samples, audioSampleSize);
		env->ReleaseShortArrayElements(audioSamples, samples, 0);
		return result;
	}
	return -1;
}

JNIEXPORT void JNICALL Java_com_timeapp_shawn_recorder_pro_recorder_NativeRecordProcessor_flushAudioBufferToQueue(JNIEnv * env, jobject obj, jint index) {
	if(handles[index] != 0 ) {
		RecordProcessor* recorder = handles[index];
		recorder->flushAudioBufferToQueue();
		recorder->destroy();
		delete recorder;
		handles[index] = 0;
	}
}

JNIEXPORT void JNICALL Java_com_timeapp_shawn_recorder_pro_recorder_NativeRecordProcessor_destroy(JNIEnv * env, jobject obj, jint index) {
	if(handles[index] != 0 ) {
		RecordProcessor* recorder = handles[index];
		recorder->destroy();
		delete recorder;
		handles[index] = 0;
	}
}
