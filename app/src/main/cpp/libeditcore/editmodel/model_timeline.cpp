#include "./model_timeline.h"

#define LOG_TAG "MODEL_TIMELINE"

ModelTimeline::ModelTimeline() {

}

ModelTimeline::~ModelTimeline() {
	clear();
}

void ModelTimeline::init() {
	// first delete the old tracks
	clear();

	// currently, only have one video track
	ModelTrack* track = new ModelTrack();
	videoTracks.push_back(track);
}

void ModelTimeline::clear() {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	for (; itor != videoTracks.end(); ) {
		delete *itor;
		itor = videoTracks.erase(itor);
	}
}

void ModelTimeline::clearFilters() {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	(*itor)->clearFilters();
}

int ModelTimeline::addFilter(int trackIndex, int64_t sequenceIn, int64_t sequenceOut, char* filterName) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	return (*itor)->addFilter(sequenceIn, sequenceOut, filterName);
}

void ModelTimeline::removeFilter(int trackIndex, int filterId) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	(*itor)->deleteFilter(filterId);
}

bool ModelTimeline::invokeFilterOnInit(int trackIndex, int filterId) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	return (*itor)->invokeFilterOnInit(filterId);
}

void ModelTimeline::setFilterParamValue(int trackIndex, int filterId, char * paramName, ParamVal value) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	(*itor)->setFilterParamValue(filterId, paramName, value);
}

bool ModelTimeline::getFilterParamValue(int trackIndex, int filterId, char * paramName, ParamVal& value) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	return (*itor)->getFilterParamValue(filterId, paramName, value);
}

int ModelTimeline::addTransition(int trackIndex, int64_t sequenceIn, int64_t sequenceOut, char* filterName) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	return (*itor)->addTransition(sequenceIn, sequenceOut, filterName);
}

void ModelTimeline::setTransitionParamValue(int trackIndex, int filterId, char * paramName, ParamVal value){
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	(*itor)->setTransitionParamValue(filterId, paramName, value);
}

bool ModelTimeline::invokeTransitionOnInit(int trackIndex, int filterId){
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	return (*itor)->invokeTransitionOnInit(filterId);
}

list<string> ModelTimeline::getAllFilters(int trackIndex) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();

	return (*itor)->getAllFilters();
}

list<string> ModelTimeline::getAllTransitions(int trackIndex) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();

	return (*itor)->getAllTransitions();
}

list<ModelFilter *> ModelTimeline::getAllTranAndFilters(int trackIndex) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();
	return (*itor)->getAllFilter();
}

list<ModelFilter *> ModelTimeline::getFilterList(int trackIndex) {
	std::list<ModelTrack *>::iterator itor = videoTracks.begin();

	return (*itor)->getFilterList();
}
