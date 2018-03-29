#include "./model_track.h"

#define LOG_TAG "MODEL_TRACK"

ModelTrack::ModelTrack() {
	filterCount = 0;
}

ModelTrack::~ModelTrack() {
	clear();
}

void ModelTrack::clearFilters() {
	std::list<ModelFilter *>::iterator itor = filters.begin();
	for (; itor != filters.end();) {
		delete *itor;
		itor = filters.erase(itor);
	}
}

void ModelTrack::clearTransitions() {
	std::list<ModelFilter *>::iterator itor = transitions.begin();
	for (; itor != transitions.end();) {
		delete *itor;
		itor = transitions.erase(itor);
	}
}

void ModelTrack::clear() {
	this->clearFilters();
	this->clearTransitions();
}

int ModelTrack::addFilter(int64_t sequenceIn, int64_t sequenceOut, char* filterName) {
	ModelFilter* filter = createModelFilter(filterCount, sequenceIn, sequenceOut, filterName);
	filters.push_back(filter);
	return filterCount++;
}

bool ModelTrack::invokeFilterOnInit(int filterId) {
	bool ret = false;
	std::list<ModelFilter *>::iterator itor = filters.begin();
	for (; itor != filters.end(); itor++) {
		if ((*itor)->getId() == filterId) {
			LOGI("find");
			ret = (*itor)->onInit();
			break;
		}
	}
	return ret;
}

void ModelTrack::setFilterParamValue(int filterId, char * paramName, ParamVal value) {
	std::list<ModelFilter *>::iterator itor = filters.begin();

	for (; itor != filters.end(); itor++) {
		if ((*itor)->getId() == filterId) {
			LOGI("find");
			(*itor)->setFilterParamValue(paramName, value);
			break;
		}
	}
}

bool ModelTrack::getFilterParamValue(int filterId, char * paramName, ParamVal& value) {
	bool ret = false;
	std::list<ModelFilter *>::iterator itor = filters.begin();

	for (; itor != filters.end(); itor++) {
		if ((*itor)->getId() == filterId) {
			LOGI("find");
			ret = (*itor)->getFilterParamValue(paramName, value);
			break;
		}
	}
	return ret;
}

void ModelTrack::deleteFilter(int sub) {
	std::list<ModelFilter *>::iterator itor = filters.begin();
	for (; itor != filters.end(); ) {
		if ((*itor)->getId() == sub) {
			delete *itor;
			itor = filters.erase(itor);
			break;
		} else {
			itor++;
		}
	}
}

list<string> ModelTrack::getAllFilters() {
	list<string> result;
	result.clear();
	std::list<ModelFilter *>::iterator itor = filters.begin();
	for (; itor != filters.end(); itor++) {
		result.push_back(string((*itor)->name));
	}
	return result;
}

list<string> ModelTrack::getAllTransitions() {
	list<string> result;
	std::list<ModelFilter *>::iterator itor = transitions.begin();
	for (; itor != transitions.end(); itor++) {
		result.push_back(string((*itor)->name));
	}
	return result;
}

int ModelTrack::addTransition(int64_t sequenceIn, int64_t sequenceOut, char* filterName) {
	ModelFilter* transition = createModelFilter(filterCount, sequenceIn, sequenceOut, filterName);
	transitions.push_back(transition);
	return filterCount++;
}

bool ModelTrack::invokeTransitionOnInit(int filterId) {
	bool ret = false;
	std::list<ModelFilter *>::iterator itor = transitions.begin();
	for (; itor != transitions.end(); itor++) {
		if ((*itor)->getId() == filterId) {
			LOGI("find");
			ret = (*itor)->onInit();
			break;
		}
	}
	return ret;
}

void ModelTrack::setTransitionParamValue(int filterId, char * paramName, ParamVal value) {
	std::list<ModelFilter *>::iterator itor = transitions.begin();
	for (; itor != transitions.end(); itor++) {
		if ((*itor)->getId() == filterId) {
			LOGI("find");
			(*itor)->setFilterParamValue(paramName, value);
			break;
		}
	}
}

void ModelTrack::deleteTransition(int sub) {
	std::list<ModelFilter *>::iterator itor = transitions.begin();
	for (; itor != transitions.end();) {
		if ((*itor)->getId() == sub) {
			delete *itor;
			itor = transitions.erase(itor);
			break;
		} else {
			itor++;
		}
	}
}
