#include "./theme_parser.h"

#define LOG_TAG "ThemeParser"

ThemeParser::ThemeParser() {
}

ThemeParser::~ThemeParser() {
}

//初始化静态成员
ThemeParser* ThemeParser::instance = new ThemeParser();
ThemeParser* ThemeParser::GetInstance() {
	return instance;
}

bool ThemeParser::readThemeProtocol(char* themePath, ModelTimeline* timeline, int frameWidth, int frameHeight, float durationInSec,
		bool (*pfnGetTextPixels)(TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		bool (*pfnGetOverlayPixels)(char* tailerLayerPath, TextCanvasParam* textCanvasParam, byte* buffer, void *context),
		void *context) {
	LOGI("openThemeProtocol called %s", themePath);
	this->getTextPixelsByPlatforms = pfnGetTextPixels;
	this->getOverlayPixelsByPlatforms = pfnGetOverlayPixels;
	this->getTextPixelsContext = context;

	std::string path(themePath);
	path.append(string("/config.xml"));

	LOGI("theme path is %s", path.c_str());

	CMarkup xmlParser;
	if (xmlParser.Load(path.data())) {
		timeline->clearFilters();
		LOGI("xmlParser.Load success");
		bool flag = false;
		xmlParser.ResetPos();
		flag = xmlParser.FindElem("theme");
		if (flag) {
			LOGI("FindElem theme success");
		}
		std::string str = xmlParser.GetAttrib("themeName");	// get the theme name
		LOGI("name is %s", str.c_str());
		xmlParser.IntoElem();	// into theme
		xmlParser.FindElem("filterList");
		xmlParser.IntoElem();	// into filterList
		bool ret = true;
		while (xmlParser.FindElem("filter")) {
			ret = readFilter(xmlParser, timeline, themePath, frameWidth, frameHeight, durationInSec);
			if(!ret){
				break;
			}
		}
		xmlParser.OutOfElem();	// out of filterList
		xmlParser.OutOfElem();	// out of theme
		LOGI("model data complete");
		return ret;
	} else {
		LOGE("Can't open the theme protocol file");
		return false;
	}
}

bool ThemeParser::readFilter(CMarkup& xmlParser, ModelTimeline* timeline, char* themePath, int frameWidth, int frameHeight, float durationInSec) {
	bool ret = true;
	std::string str = xmlParser.GetAttrib("filterName");	// get filter name
	LOGI("filter name is %s", str.c_str());
	char* filterName = (char*)str.data();

	char *endptr;
	std::string str1 = xmlParser.GetAttrib("sequenceIn");	// get filter sequence in
	int64_t sequenceIn = strtoll(str1.c_str(), &endptr, 10);
	LOGI("sequenceIn is %lld", sequenceIn);
	str1 = xmlParser.GetAttrib("sequenceOut");	// get filter sequence out
	int64_t sequenceOut = strtoll(str1.c_str(), &endptr, 10);
	LOGI("sequenceOut is %lld", sequenceOut);
	if(sequenceIn < 0 && sequenceOut <= 0){
		sequenceIn = durationInSec * 1000000 + sequenceIn;
		sequenceOut = durationInSec * 1000000 + sequenceOut;
	}
	if(sequenceOut <= 0 && sequenceIn > 0){
		sequenceOut = durationInSec * 1000000 + sequenceOut;
	}
	bool isAppendThemePath = true;
	if (0 == strcmp(filterName, PNG_SEQUENCE_FILTER_NAME)){
		isAppendThemePath = false;
	}
	if (timeline) {
		int videoTrackIndex = 0;
		int filterIndex = timeline->addFilter(videoTrackIndex, sequenceIn, sequenceOut, filterName);

		// read params
		xmlParser.IntoElem();	// into filter
		while (xmlParser.FindElem("param")) {
			std::string strParamName = xmlParser.GetAttrib("id");		// get param name
			std::string strParamValue = xmlParser.GetAttrib("value");	// get param value
			std::string strParamType = xmlParser.GetAttrib("type");		// get param type
			ParamVal value;
			fillParamValue(strParamValue, strParamType, value, isAppendThemePath, themePath);
			if(value.type == EffectParamTypePath){
				const char* filePath = value.strVal.c_str();
				if (isFileSuffix(filePath)) {
					if ((access(filePath, F_OK)) == -1) {
						ret = false;
						return ret;
					}
				}
			}
			timeline->setFilterParamValue(videoTrackIndex, filterIndex, (char*)strParamName.c_str(), value);
		}
		xmlParser.OutOfElem();	// out of filter

		if (0 == strcmp(filterName, "text_scene") && NULL != getTextPixelsByPlatforms){
			ret = this->buildTextSceneByPlatforms(timeline, videoTrackIndex, filterIndex);
		} else if(0 == strcmp(filterName, "trailer_scene") && NULL != getOverlayPixelsByPlatforms) {
			ret = this->buildTrialerSceneByPlatforms(timeline, videoTrackIndex, filterIndex, frameWidth, frameHeight);
		}
		bool isSuccess = timeline->invokeFilterOnInit(videoTrackIndex, filterIndex);
		if(!isSuccess){
			//TODO: 处理失败情况
			LOGI("invokeFilter %s OnInit failed", filterName);
		}
	}

	LOGI("timeline->addFilter success");
	return ret;
}

bool ThemeParser::buildTrialerSceneByPlatforms(ModelTimeline* timeline, int videoTrackIndex, int filterIndex, int frameWidth, int frameHeight) {
	char* overlayPath = "";
	ParamVal pathValue;
	bool isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_PATH, pathValue);
	if(isSucc){
		overlayPath = (char*)pathValue.strVal.c_str();
		LOGI("in trailer scene process find overlayPath is %s", overlayPath);
	} else{
		LOGI("in trailer scene process find overlayPath failed");
	}
	ParamVal value;
	int width = frameWidth;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_WIDTH, value);
	if(isSucc){
		width = value.u.intVal;
		LOGI("in trailer scene process find width is %d", width);
	} else{
		LOGI("in trailer scene process find width failed");
	}
	int height = frameHeight;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_HEIGHT, value);
	if(isSucc){
		height = value.u.intVal;
		LOGI("in trailer scene process find height is %d", height);
	} else{
		LOGI("in trailer scene process find height failed");
	}
	int textLabelLeft = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_LEFT, value);
	if(isSucc){
		textLabelLeft = value.u.intVal;
		LOGI("in trailer scene process find textLabelLeft is %d", textLabelLeft);
	} else{
		LOGI("in trailer scene process find textLabelLeft failed");
	}
	int textLabelTop = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_TOP, value);
	if(isSucc){
		textLabelTop = value.u.intVal;
		LOGI("in trailer scene process find textLabelTop is %d", textLabelTop);
	} else{
		LOGI("in trailer scene process find textLabelTop failed");
	}
	int textLabelWidth = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_WIDTH, value);
	if(isSucc){
		textLabelWidth = value.u.intVal;
		LOGI("in trailer scene process find textLabelWidth is %d", textLabelWidth);
	} else{
		LOGI("in trailer scene process find textLabelWidth failed");
	}
	int textLabelHeight = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_HEIGHT, value);
	if(isSucc){
		textLabelHeight = value.u.intVal;
		LOGI("in trailer scene process find textLabelHeight is %d", textLabelHeight);
	} else{
		LOGI("in trailer scene process find textLabelHeight failed");
	}
	int textColor = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_COLOR, value);
	if(isSucc){
		Color color = value.u.colorVal;
		textColor = (((int)color.r) << 16) | (((int)color.g) << 8) | ((int)color.b);
		LOGI("in trailer scene process find textColor is %d", textColor);
	} else{
		LOGI("in trailer scene process find textColor failed");
	}
	float textShadowRadius = 0.0f;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_RADIUS, value);
	if (isSucc) {
		textShadowRadius = value.u.fltVal;
		LOGI("find text scene textShadowRadius success is %.3f", textShadowRadius);
	} else {
		LOGI("find text scene textShadowRadius failed use default value is %.3f", textShadowRadius);
	}
	float textShadowXOffset = 0.0f;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_XOFFSET, value);
	if (isSucc) {
		textShadowXOffset = value.u.fltVal;
		LOGI("find text scene textShadowXOffset success is %.3f", textShadowXOffset);
	} else {
		LOGI("find text scene textShadowXOffset failed use default value is %.3f", textShadowXOffset);
	}
	float textShadowYOffset = 0.0f;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_YOFFSET, value);
	if (isSucc) {
		textShadowYOffset = value.u.fltVal;
		LOGI("find text scene textShadowYOffset success is %.3f", textShadowYOffset);
	} else {
		LOGI("find text scene textShadowYOffset failed use default value is %.3f", textShadowYOffset);
	}
	int textShadowColor = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_COLOR, value);
	if (isSucc) {
		Color color = value.u.colorVal;
		textShadowColor = (((int)color.r) << 16) | (((int)color.g) << 8) | ((int)color.b);
		LOGI("find text scene textShadowColor success is %d", textShadowColor);
	} else {
		LOGI("find text scene textShadowColor failed use default value is %d", textShadowColor);
	}
	int textSize = 20;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_SIZE, value);
	if (isSucc) {
		textSize = value.u.intVal;
		LOGI("in trailer scene process find textSize success is %d", textSize);
	} else {
		LOGI("in trailer scene process find textSize failed use default value is %d", textSize);
	}
	int textAlignment = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_ALIGNMENT, value);
	if (isSucc) {
		textAlignment = value.u.intVal;
		LOGI("in trailer scene process find textAlignment success is %d", textAlignment);
	} else {
		LOGI("in trailer scene process find textAlignment failed use default value is %d", textAlignment);
	}
	int textType = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_TEXT_TYPE, value);
	if (isSucc) {
		textType = value.u.intVal;
		LOGI("in trailer scene process find textType success is %d", textType);
	} else {
		LOGI("in trailer scene process find textType failed use default value is %d", textType);
	}
	TextCanvasParam* textCanvasParam = new TextCanvasParam(width, height, textLabelLeft, textLabelTop, textLabelWidth, textLabelHeight, textColor
			, textShadowRadius, textShadowXOffset, textShadowYOffset, textShadowColor, textSize, textAlignment, textType);

	ParamVal textPixelsValue;
	int length = width * height * 4;
	byte* textPixels = new byte[length];
	bool ret = getOverlayPixelsByPlatforms(overlayPath, textCanvasParam, textPixels, getTextPixelsContext);
	textPixelsValue.u.arbData = textPixels;
	timeline->setFilterParamValue(videoTrackIndex, filterIndex, TRAILER_SCENE_PARAM_ID_PIXELS, textPixelsValue);
	return ret;
}

bool ThemeParser::buildTextSceneByPlatforms(ModelTimeline* timeline, int videoTrackIndex, int filterIndex) {
	int width = 480;
	ParamVal value;
	bool isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_WIDTH, value);
	if (isSucc) {
		width = value.u.intVal;
		LOGI("find text scene width success is %d", width);
	} else {
		LOGI("find text scene width failed use default value is %d", width);
	}
	int height = 480;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_HEIGHT, value);
	if (isSucc) {
		height = value.u.intVal;
		LOGI("find text scene height success is %d", height);
	} else {
		LOGI("find text scene height failed use default value is %d", height);
	}
	int textLabelLeft = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_LEFT, value);
	if (isSucc) {
		textLabelLeft = value.u.intVal;
		LOGI("find text scene textLabelLeft success is %d", textLabelLeft);
	} else {
		LOGI("find text scene textLabelLeft failed use default value is %d", textLabelLeft);
	}
	int textLabelTop = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_TOP, value);
	if (isSucc) {
		textLabelTop = value.u.intVal;
		LOGI("find text scene textLabelTop success is %d", textLabelTop);
	} else {
		LOGI("find text scene textLabelTop failed use default value is %d", textLabelTop);
	}
	int textLabelWidth = 480;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_WIDTH, value);
	if (isSucc) {
		textLabelWidth = value.u.intVal;
		LOGI("find text scene textLabelWidth success is %d", textLabelWidth);
	} else {
		LOGI("find text scene textLabelWidth failed use default value is %d", textLabelWidth);
	}
	int textLabelHeight = 480;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_HEIGHT, value);
	if (isSucc) {
		textLabelHeight = value.u.intVal;
		LOGI("find text scene textLabelHeight success is %d", textLabelHeight);
	} else {
		LOGI("find text scene textLabelHeight failed use default value is %d", textLabelHeight);
	}
	int textColor = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_COLOR, value);
	if (isSucc) {
		Color color = value.u.colorVal;
		textColor = (((int)color.r) << 16) | (((int)color.g) << 8) | ((int)color.b);
		LOGI("find text scene textColor success is %d", textColor);
	} else {
		LOGI("find text scene textColor failed use default value is %d", textColor);
	}
	float textShadowRadius = 0.0f;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_RADIUS, value);
	if (isSucc) {
		textShadowRadius = value.u.fltVal;
		LOGI("find text scene textShadowRadius success is %.3f", textShadowRadius);
	} else {
		LOGI("find text scene textShadowRadius failed use default value is %.3f", textShadowRadius);
	}
	float textShadowXOffset = 0.0f;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_XOFFSET, value);
	if (isSucc) {
		textShadowXOffset = value.u.fltVal;
		LOGI("find text scene textShadowXOffset success is %.3f", textShadowXOffset);
	} else {
		LOGI("find text scene textShadowXOffset failed use default value is %.3f", textShadowXOffset);
	}
	float textShadowYOffset = 0.0f;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_YOFFSET, value);
	if (isSucc) {
		textShadowYOffset = value.u.fltVal;
		LOGI("find text scene textShadowYOffset success is %.3f", textShadowYOffset);
	} else {
		LOGI("find text scene textShadowYOffset failed use default value is %.3f", textShadowYOffset);
	}
	int textShadowColor = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SHADOW_COLOR, value);
	if (isSucc) {
		Color color = value.u.colorVal;
		textShadowColor = (((int)color.r) << 16) | (((int)color.g) << 8) | ((int)color.b);
		LOGI("find text scene textShadowColor success is %d", textShadowColor);
	} else {
		LOGI("find text scene textShadowColor failed use default value is %d", textShadowColor);
	}
	int textSize = 20;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_SIZE, value);
	if (isSucc) {
		textSize = value.u.intVal;
		LOGI("find text scene textSize success is %d", textSize);
	} else {
		LOGI("find text scene textSize failed use default value is %d", textSize);
	}
	int textAlignment = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_ALIGNMENT, value);
	if (isSucc) {
		textAlignment = value.u.intVal;
		LOGI("find text scene textAlignment success is %d", textAlignment);
	} else {
		LOGI("find text scene textAlignment failed use default value is %d", textAlignment);
	}
	int textType = 0;
	isSucc = timeline->getFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_TEXT_TYPE, value);
	if (isSucc) {
		textType = value.u.intVal;
		LOGI("find text scene textType success is %d", textType);
	} else {
		LOGI("find text scene textType failed use default value is %d", textType);
	}
	TextCanvasParam* textCanvasParam = new TextCanvasParam(width, height, textLabelLeft, textLabelTop, textLabelWidth, textLabelHeight, textColor,
			textShadowRadius, textShadowXOffset, textShadowYOffset, textShadowColor, textSize, textAlignment, textType);
	ParamVal textPixelsValue;
	int length = width * height * 4;
	byte* textPixels = new byte[length];
	bool ret = getTextPixelsByPlatforms(textCanvasParam, textPixels, getTextPixelsContext);
	textPixelsValue.u.arbData = textPixels;
	timeline->setFilterParamValue(videoTrackIndex, filterIndex, TEXT_SCENE_PARAM_ID_PIXELS, textPixelsValue);
	return ret;
}

void ThemeParser::fillParamValue(string paramValue, string paramType, ParamVal & param, bool isAppendThemePath, char* themePath) {
	int type = atoi(paramType.c_str());

	switch (type) {
	case 1: {	// int
		param.type = EffectParamTypeInt;
		param.u.intVal = atoi(paramValue.c_str());

		LOGI("int value is %d", param.u.intVal);
	}
		break;

	case 2: {	// float
		param.type = EffectParamTypeFloat;
		param.u.fltVal = atof(paramValue.c_str());

		LOGI("float value is %f", param.u.fltVal);
	}
		break;

	case 3: {	// bool
		param.type = EffectParamTypeBoolean;
		param.u.boolVal = bool(atoi(paramValue.c_str()));

		LOGI("bool value is %d", (int)param.u.boolVal);
	}
		break;

	case 4: {	// string
		param.type = EffectParamTypeString;
		param.strVal = paramValue;
	}
		break;

	case 5: {	// color
		param.type = EffectParamTypeColor;

		Color value;

		size_t pos = paramValue.find(',');
		if (pos != string::npos) {
			string r = paramValue.substr(0, pos);
			value.r = atof(r.c_str());
		} else
			return;

		size_t pos1 = paramValue.find(',', pos + 1);
		if (pos1 != string::npos) {
			string g = paramValue.substr(pos + 1, pos1 - pos - 1);
			value.g = atof(g.c_str());
		} else
			return;

		size_t pos2 = paramValue.find(',', pos1 + 1);
		if (pos2 != string::npos) {
			string b = paramValue.substr(pos1 + 1, pos2 - pos1 - 1);
			value.b = atof(b.c_str());
		} else
			return;

		string a = paramValue.substr(pos2 + 1, paramValue.length() - pos2 - 1);
		value.a = atof(a.c_str());

		LOGI("color r:%f g:%f b:%f a:%f", value.r, value.g, value.b, value.a);
		param.u.colorVal = value;
	}
		break;

	case 6: {	// position2d
		param.type = EffectParamTypePosition2D;

		Position2D value;

		size_t pos = paramValue.find(',');
		if (pos != string::npos) {
			string x = paramValue.substr(0, pos);
			value.x = atof(x.c_str());
		} else
			return;

		string y = paramValue.substr(pos + 1, paramValue.length() - pos - 1);
		value.y = atof(y.c_str());

		param.u.pos2dVal = value;
	}
		break;

	case 7: {	// position3d
		param.type = EffectParamTypePosition3D;

		Position3D value;

		size_t pos = paramValue.find(',');
		if (pos != string::npos) {
			string x = paramValue.substr(0, pos);
			value.x = atof(x.c_str());
		} else
			return;

		size_t pos1 = paramValue.find(',', pos + 1);
		if (pos1 != string::npos) {
			string y = paramValue.substr(pos + 1, pos1 - pos - 1);
			value.y = atof(y.c_str());
		} else
			return;

		string z = paramValue.substr(pos1 + 1, paramValue.length() - pos1 - 1);
		value.z = atof(z.c_str());

		param.u.pos3dVal = value;
	}
		break;

	case 8: {	// path
		param.type = EffectParamTypePath;
		if(isAppendThemePath){
			string value(themePath);
			value.append(string("/"));
			value.append(paramValue);
			param.strVal = value;
		} else{
			string value(paramValue);
			param.strVal = value;
		}
		LOGI("path value is %s", param.strVal.c_str());
	}
		break;

	default: {
		LOGE("unknow param type");
	}
		break;
	}
}
