#include "com_changba_playrecord_NativeSoundTouch.h"
#include "SoundTouch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>

using namespace soundtouch;

#ifdef __cplusplus  
extern "C"  
{  
#endif 

	/** static WaveFileHandle array **/
	static SoundTouch* handles[100];
	 
	/**
	 * Seeks a free handle in the handles array and returns its index or -1 if no handle could be found
	 */
	static int findFreeHandle( )
	{
		for( int i = 0; i < 100; i++ )
		{
			if( handles[i] == 0 )
				return i;
		}
	 
		return -1;
	}
	

	static inline void closeHandle( SoundTouch* soundTouch )
	{
		delete soundTouch;
	}

	JNIEXPORT jint JNICALL Java_com_changba_playrecord_NativeSoundTouch_init
  		(JNIEnv * env, jobject thiz){
  		int index = findFreeHandle( );
		if( index == -1 )
			return -1;
		SoundTouch * soundTouch = new SoundTouch();
		handles[index] = soundTouch;
		return index;
  	}

	JNIEXPORT void JNICALL Java_com_changba_playrecord_NativeSoundTouch_destroy
  		(JNIEnv * env, jobject thiz, jint index){
  		if( handles[index] != 0 )
		{
			closeHandle(handles[index]);
			handles[index] = 0;
		}	
  	}
	
	JNIEXPORT void JNICALL Java_com_changba_playrecord_NativeSoundTouch_setSampleRate
  		(JNIEnv * env, jobject thiz, jint index, jfloat srate){
  		float sampleRate = srate; 
  		SoundTouch* soundTouch = handles[index];
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","setSampleRate");
  		soundTouch -> setSampleRate(sampleRate);
  	}
  	
  	JNIEXPORT void JNICALL Java_com_changba_playrecord_NativeSoundTouch_setPitch
  		(JNIEnv * env, jobject thiz, jint index, jfloat pitch){
  		float samplePitch = pitch; 
  		SoundTouch* soundTouch = handles[index];
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","setPitch");
  		soundTouch -> setPitch(samplePitch);  	
  		soundTouch -> setSetting(SETTING_SEEKWINDOW_MS, 15);
        soundTouch -> setSetting(SETTING_SEQUENCE_MS, 20);
        soundTouch -> setSetting(SETTING_OVERLAP_MS, 8);
        soundTouch -> setSetting(SETTING_USE_QUICKSEEK, 1);		
  	}
  	
  	
	JNIEXPORT void JNICALL Java_com_changba_playrecord_NativeSoundTouch_setChannels
	  (JNIEnv * env, jobject thiz, jint index, jint channels){
		int sampleChannels = channels; 
  		SoundTouch* soundTouch = handles[index];
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","setChannels");
  		soundTouch -> setChannels(sampleChannels);
	}
	
	JNIEXPORT void JNICALL Java_com_changba_playrecord_NativeSoundTouch_putSamples
  		(JNIEnv* env, jobject thiz, jint index, jshortArray preProcessData){
		jshort* target = env -> GetShortArrayElements(preProcessData, 0);
		if (target == NULL) {
  			//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","target == NULL");
			return;
		}
  		int bufferSize = env -> GetArrayLength(preProcessData);
		//char buffer[80];
		//sprintf(buffer, "%s%d", "call putSamples prePostdata Size is ", bufferSize);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
  		SoundTouch* soundTouch = handles[index];
  		if(soundTouch == NULL) {
  			//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","soundTouch == NULL");
			return;
  		}
		//sprintf(buffer, "%s%d", "SAMPLETYPE size : ", sizeof(SAMPLETYPE));
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
  		
		//sprintf(buffer, "%s%d", "prePostdata at index 300 value is : ", target[bufferSize/3]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "prePostdata at index 500 value is : ", target[bufferSize/20]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "prePostdata at index 899 value is : ", target[bufferSize/80]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "prePostdata at index 1899 value is : ", target[bufferSize/10]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "prePostdata at last index value is : ", target[bufferSize - 1]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
  		
        soundTouch -> putSamples((SAMPLETYPE *)target, bufferSize/2);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","after call soundTouch -> putSamples");
		env->ReleaseShortArrayElements(preProcessData, target, 0);
		
        //SAMPLETYPE *samples = (SAMPLETYPE *)malloc(sizeof(SAMPLETYPE) * bufferSize*2);
        //soundTouch -> receiveSamples(samples, bufferSize);
	    //新建一个长度为len的jintArray数组   
	    //jshortArray array = env -> NewShortArray(bufferSize);  
	    //if(array == NULL) {  
        //	return NULL;  
    	//}  
    	 //给需要返回的数组赋值   
    	//env -> SetShortArrayRegion(array, 0, bufferSize, (short*)samples);  
    	//free(samples);
    	//return array; 
  	}
  	
  	
  	JNIEXPORT jint JNICALL Java_com_changba_playrecord_NativeSoundTouch_receiveSamples
  		(JNIEnv* env, jobject thiz, jint index, jshortArray responseData){
		jshort* samples = env->GetShortArrayElements(responseData, 0);
  		int bufferSize = env -> GetArrayLength(responseData);
		//SAMPLETYPE *samples = (SAMPLETYPE *)malloc(sizeof(SAMPLETYPE) * bufferSize * 2);
		if (samples == NULL) {
  			//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","target == NULL");
			return -1;
		}
		//char buffer[80];
		//sprintf(buffer, "%s%d", "call receiveSamples response data Size is ", bufferSize);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
  		SoundTouch* soundTouch = handles[index];
  		if(soundTouch == NULL) {
  			//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","soundTouch == NULL");
			return -1;
  		}
        int size = soundTouch -> receiveSamples((SAMPLETYPE *)samples, bufferSize/2);
		//char buffer[80];
		//sprintf(buffer, "%s%d", "receiveSamples size : ", size);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch","after call soundTouch -> receiveSamples");
  		
		//sprintf(buffer, "%s%d", "responseData at index 300 value is : ", samples[bufferSize/3]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "responseData at index 500 value is : ", samples[bufferSize/20]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "responseData at index 899 value is : ", samples[bufferSize/80]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "responseData at index 1899 value is : ", samples[bufferSize/10]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
		//sprintf(buffer, "%s%d", "responseData at last index value is : ", samples[bufferSize - 1]);
  		//__android_log_write(ANDROID_LOG_ERROR,"native sound touch",buffer);
  		//env -> SetShortArrayRegion(responseData, 0, bufferSize, (short*)samples);  
		env->ReleaseShortArrayElements(responseData, samples, 0);
		//free(samples);
		return size;
  	}
  	
#ifdef __cplusplus  
}  
#endif  