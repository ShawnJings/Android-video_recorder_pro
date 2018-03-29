#ifndef MODEL_FILTER_H
#define MODEL_FILTER_H

#include "../common/video_effect_def.h"
#include "../platform_dependent/platform_common.h"
#include "../common/video_effect_params.h"
#include <pthread.h>
#include <map>
#include <string>

using namespace std;

class ModelFilter;

class FilterCallback : public EffectCallback
{
public:
	FilterCallback();
	~FilterCallback();

public:
    virtual bool getParamValue(string paramName, ParamVal & val);
    virtual void setParamValue(string paramName, ParamVal val);
    void setParentFilter(ModelFilter * parent);

protected:
    ModelFilter * mParentFilter;
};

class ModelFilter {
public:
	ModelFilter();
	ModelFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	virtual ~ModelFilter();

public:
	void init();

	virtual bool onInit(){
		return true;
	};

	virtual void onRenderPre(float pos);

	virtual void clear();

	int getId() {
		return id;
	}

	virtual bool isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context);

	void setFilterParamValue(char * paramName, ParamVal value);
	bool getFilterParamValue(string paramName, ParamVal & value);

	EffectCallback* getFilterCallback();

public:
	char* 	name;			// filter name or id

protected:
	int 	id;				// if two filters have the same parameters, index will identify them
	int64_t sequenceIn;		// sequence in
	int64_t sequenceOut;	// sequence out

	pthread_rwlock_t 		mRwlock;
	map<string, ParamVal>	mMapParamValue;	// effect params value

	FilterCallback			mFilterCallback;
};

#endif // MODEL_FILTER_H
