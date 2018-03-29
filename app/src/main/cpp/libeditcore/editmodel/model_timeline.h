#ifndef MODEL_TIMELINE_H
#define MODEL_TIMELINE_H

#include <list>
#include <string>

#include "../editmodel/model_track.h"

using namespace std;

class ModelTimeline {
public:
	ModelTimeline();
	virtual ~ModelTimeline();

public:
	void init();
	void clear();
	void clearFilters();

	list<string> getAllFilters(int trackIndex);
	list<string> getAllTransitions(int trackIndex);

	list<ModelFilter *> getAllTranAndFilters(int trackIndex);
	list<ModelFilter *> getFilterList(int trackIndex);

	void removeFilter(int trackIndex, int filterId);
	int addTransition(int trackIndex, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	void setTransitionParamValue(int trackIndex, int filterId, char * paramName, ParamVal value);
	bool invokeTransitionOnInit(int trackIndex, int filterId);
	int addFilter(int trackIndex, int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	void setFilterParamValue(int trackIndex, int filterId, char * paramName, ParamVal value);
	bool getFilterParamValue(int trackIndex, int filterId, char * paramName, ParamVal& value);
	bool invokeFilterOnInit(int trackIndex, int filterId);

protected:
	list<ModelTrack *>		videoTracks;
};

#endif // MODEL_TIMELINE_H
