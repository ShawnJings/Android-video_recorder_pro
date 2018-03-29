#ifndef MODEL_TRACK_H
#define MODEL_TRACK_H

#include <list>
#include <string>

#include "../common/video_filter_utils.h"
#include "../editmodel/model_filter.h"

using namespace std;

class ModelTrack {
public:
	ModelTrack();
	virtual ~ModelTrack();

public:
	void clear();
	void clearFilters();
	void clearTransitions();

	list<ModelFilter *> getAllFilter(){
		list<ModelFilter *> result;
		std::list<ModelFilter *>::iterator itor = transitions.begin();
		for (; itor != transitions.end(); itor++) {
			result.push_back(*itor);
		}
		itor = filters.begin();
		for (; itor != filters.end(); itor++) {
			result.push_back(*itor);
		}
		return result;
	};
	const list<ModelFilter *> getFilterList() {
		return filters;
	};

	int addTransition(int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	void deleteTransition(int sub);
	void setTransitionParamValue(int filterId, char * paramName, ParamVal value);
	bool invokeTransitionOnInit(int filterId);
	list<string> getAllTransitions();

	int addFilter(int64_t sequenceIn, int64_t sequenceOut, char* filterName);
	void deleteFilter(int sub);
	bool invokeFilterOnInit(int filterId);
	void setFilterParamValue(int filterId, char * paramName, ParamVal value);
	bool getFilterParamValue(int filterId, char * paramName, ParamVal& value);
	list<string> getAllFilters();

protected:
	int								index;
	list<ModelFilter *> 				filters;
	list<ModelFilter *>				transitions;

	// only add count never sub count
	int 							filterCount;
};

#endif // MODEL_TRACK_H
