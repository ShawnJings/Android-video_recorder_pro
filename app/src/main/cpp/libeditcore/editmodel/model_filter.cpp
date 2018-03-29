#include "./model_filter.h"

#define LOG_TAG "MODEL_FILTER"

/* FilterCallback class */
FilterCallback::FilterCallback() {
	mParentFilter = NULL;
}

FilterCallback::~FilterCallback() {
	mParentFilter = NULL;
}

void FilterCallback::setParamValue(string paramName, ParamVal val){
	if (mParentFilter){
		mParentFilter->setFilterParamValue((char*)(paramName.c_str()), val);
	}
}
bool FilterCallback::getParamValue(string paramName, ParamVal & val) {
	if (!mParentFilter)
		return false;

	return mParentFilter->getFilterParamValue(paramName, val);
}

void FilterCallback::setParentFilter(ModelFilter * parent) {
	mParentFilter = parent;
}

/* ModelFiler class */
ModelFilter::ModelFilter() {
	init();
}

ModelFilter::ModelFilter(int index, int64_t sequenceIn, int64_t sequenceOut, char* filterName) {
	init();

	this->id = index;
	this->sequenceIn = sequenceIn;
	this->sequenceOut = sequenceOut;

	int length = 0;
	if (filterName) {
		length = strlen(filterName);
	}

	if (length > 0) {
		this->name = new char[length+1];
		memcpy(this->name, filterName, sizeof(char) * (length+1));
	}
}

ModelFilter::~ModelFilter() {
	clear();
}

void ModelFilter::init() {
	// default the whole video duration
	sequenceIn = -1;
	sequenceOut = -1;
	name = NULL;
	mMapParamValue.clear();
	pthread_rwlock_init(&mRwlock, NULL);
	mFilterCallback.setParentFilter(this);
}

void ModelFilter::clear(){
	if (name) {
		delete [] name;
		name = NULL;
	}
	pthread_rwlock_destroy(&mRwlock);
}

void ModelFilter::onRenderPre(float pos) {

}

bool ModelFilter::isAvailable(float pos, bool (*pfnDetectGPUSurpportedEffect)(void* context), void* context) {
	int64_t position = pos * 1000000;
	if (position >= sequenceIn && position <= sequenceOut) {
		return true;
	} else {
		return false;
	}
}

// if the reading and writing operation is not atomic, must use read and write lock
void ModelFilter::setFilterParamValue(char * paramName, ParamVal value) {
	pthread_rwlock_wrlock(&mRwlock);
	mMapParamValue[string(paramName)] = value;
	pthread_rwlock_unlock(&mRwlock);
}

bool ModelFilter::getFilterParamValue(string paramName, ParamVal & value) {
	bool isFounded = false;
	pthread_rwlock_rdlock(&mRwlock);
	map<string, ParamVal>::iterator itor;
	itor = mMapParamValue.find(paramName);
	if (itor != mMapParamValue.end()) {
		value = itor->second;
		isFounded = true;
	}
	pthread_rwlock_unlock(&mRwlock);
	return isFounded;
}

EffectCallback* ModelFilter::getFilterCallback() {
	return (EffectCallback*)(&mFilterCallback);
}
